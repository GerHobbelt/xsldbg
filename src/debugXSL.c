
/***************************************************************************
                          debugXSL.c  - debugger commands to use
                             -------------------
    begin                : Sun Sep 16 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 * Orinal file : debugXML.c : This is a set of routines used for 
 *              debugging the tree produced by the XML parser.
 *
 * New file : debugXSL.c : Debug support version
 *
 * See Copyright for the status of this software.
 *
 * Daniel Veillard <daniel@veillard.com>
 * 
 * Permission obtained to modify the LGPL'd code and extend to include breakpoints, inspections of
 * stylesheet source, xml data, stylesheet variables
 */

#ifdef VERSION
#undef VERSION
#endif
#include "config.h"

#include "xsldbg.h"
#include "files.h"
#include "debugXSL.h"
#include "options.h"
#include "breakpointInternals.h"
#include "help.h"
#include <stdlib.h>
#include <libxslt/transform.h>  /* needed by source command */
#include <libxslt/xsltInternals.h>




/* current template being processed */
xsltTemplatePtr rootCopy;

/* how may items have been printed */
int printCount;

/* valid commands of xslDbgShell */
const char *commandNames[] = {
    "help",

    "bye",
    "exit",
    "quit",

    "step",
    "stepup",
    "stepdown",
    "continue",
    "run",

    "templates",
    "where",
    "frame",
    "stylesheets",

    "break",
    "showbreak",
    "delete",
    "enable",
    "disable",

    "ls",
    "dir",
    "du",
    "cat",
    "print",
    "pwd",
    "dump",
    "base",

    "globals",
    "locals",
    /* "cat", already listed */
    "source",
    "data",
    "cd",

    /* file related */
    "validate",
    "load",
    "save",
    "write",
    "free",

    /* Operating system related */
    "chdir",
    "shell",
    "tty",

    /* libxslt parameter related */
    "addparam",
    "delparam",
    "showparam",

    /* extra options */
    "trace",
    "walk",

    /* searching */
    "search",

    NULL                        /* Indicate the end of list */
};

/* valid commands of xslShell in there alternative|shorter format */
const char *shortCommandNames[] = {
    "h",

    "bye",
    "exit",
    "q",                        /*quit */

    "s",                        /* step */
    "up",                       /*stepup */
    "down",                     /* stepdown */
    "c",                        /* cont */
    "r",                        /* run */

    "t",                        /* templates */
    "w",                        /* where */
    "f",
    "style",

    "b",                        /* break */
    "show",
    "d",                        /* delete */
    "e",                        /* enabled */
    "disable",

    "ls",
    "dir",
    "du",
    "cat",
    "print",
    "pwd",
    "dump",
    "base",

    "globals",
    "locals",
    /* "cat", already listed */
    "source",
    "data",
    "cd",

    /* file related */
    "validate",
    "load",
    "save",
    "write",
    "free",

    /* Operating system related */
    "chdir",
    "shell",
    "tty",

    /* libxslt parameter related */
    "addparam",
    "delparam",
    "showparam",

    /* extra options/commands */
    "trace",
    "walk",

    /* searching */
    "search",

    NULL                        /* Indicate the end of list */
};


enum {                          /* id's for commands of xslDbgShell */
    DEBUG_HELP_CMD = 100,
    DEBUG_BYE_CMD,
    DEBUG_EXIT_CMD,
    DEBUG_QUIT_CMD,

    DEBUG_STEP_CMD,
    DEBUG_STEPUP_CMD,
    DEBUG_STEPDOWN_CMD,
    DEBUG_CONT_CMD,
    DEBUG_RUN_CMD,

    DEBUG_TEMPLATES_CMD,
    DEBUG_WHERE_CMD,
    DEBUG_FRAME_CMD,
    DEBUG_STYLESHEETS_CMD,

    DEBUG_BREAK_CMD,
    DEBUG_SHOWBREAK_CMD,
    DEBUG_DELETE_CMD,
    DEBUG_ENABLE_CMD,
    DEBUG_DISABLE_CMD,

    DEBUG_LS_CMD,
    DEBUG_DIR_CMD,
    DEBUG_DU_CMD,
    DEBUG_CAT_CMD,
    DEBUG_PRINT_CMD,            /* cat alternative */
    DEBUG_PWD_CMD,
    DEBUG_DUMP_CMD,
    DEBUG_BASE_CMD,

    DEBUG_GLOBALS_CMD,
    DEBUG_LOCALS_CMD,
    /*     DEBUG_CAT_CMD, already listed */
    DEBUG_SOURCE_CMD,
    DEBUG_DATA_CMD,
    DEBUG_CD_CMD,

    /* file related */
    DEBUG_VALIDATE_CMD,
    DEBUG_LOAD_CMD,
    DEBUG_SAVE_CMD,
    DEBUG_WRITE_CMD,
    DEBUG_FREE_CMD,

    /* Operating system related */
    DEBUG_CHDIR_CMD,
    DEBUG_SHELL_EXEC_CMD,
    DEBUG_TTY_CMD,

    /* libxslt parameter related */
    DEBUG_ADDPARAM_CMD,
    DEBUG_DELPARAM_CMD,
    DEBUG_SHOWPARAM_CMD,

    /* extra options/commands */
    DEBUG_TRACE_CMD,
    DEBUG_WALK_CMD,

    /* searching */
    DEBUG_SEARCH_CMD,
    /*NULL */
};


/* some convenient short cuts when using  cd command*/
const char *cdShortCuts[] = {
    "<<",
    ">>",
    "<-",
    "->",
    NULL                        /* indicate end of list */
};

/* what to replace shortcuts with */
const char *cdAlternative[] = {
    "preceding-sibling::node()",
    "following-sibling::node()",
    "ancestor::node()",
    "descendant::node()"
};

/* what enum to use for shortcuts */
enum {
    DEBUG_PREV_SIBLING = 200,
    DEBUG_NEXT_SIBLING,
    DEBUG_ANCESTOR_NODE,
    DEBUG_DESCENDANT_NODE
};

#include <libxml/xpathInternals.h>

#include <libxslt/extra.h>
#include <string.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

/* Added readline and history support KPI */
#ifdef HAVE_READLINE
#include <readline/readline.h>
#ifdef HAVE_HISTORY
#include <readline/history.h>
#endif
#endif

#include <libxml/xmlmemory.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/valid.h>
#include <libxml/debugXML.h>
#include <libxml/HTMLtree.h>
#include <libxml/HTMLparser.h>
#include <libxml/xmlerror.h>




/* our private functions */
xmlChar *xslDbgShellReadline(xmlChar * prompt);

/**
 * getTemplate:
 * 
 * Returns the last template node found, if any
 */
xsltTemplatePtr getTemplate(){
  return rootCopy;
}

/**
 * trimString:
 * @text : valid string with leading or trailing spaces
 *
 * Remove leading and trailing spaces off @text
 *         stores result back into @text
 */
int trimString(xmlChar * text);

/*
 * splitString:
 * @textIn : the string to split
 * @maxStrings : max number of strings to put into @out
 * @out: 
 * Spit string by white space and put into @out
 * 
 * Return 1 on success,
 *        0 otherwise
 */
int splitString(xmlChar * textIn, int maxStrings, xmlChar ** out);

void xslDbgCd(xsltTransformContextPtr styleCtxt, xmlShellCtxtPtr ctxt,
              xmlChar * arg, xmlNodePtr source);
int xslDbgPrintTemplateHelper(xsltTemplatePtr templ, int verbose,
                              int templateCount);
void xslDbgPrintCallStack(const xmlChar* arg);
void xsldbgSleep(long delay);
int xslDbgWalkContinue(void);
int lookupName(xmlChar * name, xmlChar ** matchList);






/****************************************************************
 *								*
 *	 	The XSL shell related functions			*
 *								*
 ****************************************************************/


/* xslDbgCd :
 * Change directories
 * @styleCtxt : current stylesheet context
 * @ctxt : current shell context
 * @name : path to change to 
 */
void
xslDbgCd(xsltTransformContextPtr styleCtxt, xmlShellCtxtPtr ctxt,
         xmlChar * arg, xmlNodePtr source)
{
    xmlXPathObjectPtr list = NULL;
    int offset = 2;             /* in some cases I'm only interested after first two chars  */

    if (!ctxt) {
        xsltGenericError(xsltGenericErrorContext,
                         "Debuger has no files loaded, try reloading files\n");
        return;
    }
    if (arg == NULL)
        arg = (xmlChar *) "";
    if (arg[0] == 0) {
        ctxt->node = (xmlNodePtr) ctxt->doc;
    } else {
        if ((arg[0] == '-') && (xmlStrLen(arg) > 2)) {
            if (styleCtxt) {
                if (arg[1] == 't') {
                    xmlNodePtr templateNode;

                    /* quickly find a template */

                    /* skip any white spaces */
                    while (IS_BLANK(arg[offset]))
                        offset++;

                    templateNode =
                        xslFindTemplateNode(styleCtxt->style,
                                            &arg[offset]);
                    if (!templateNode) {
                        xsltGenericError(xsltGenericErrorContext,
                                         "Template '%s' not found\n",
                                         &arg[offset]);
                        return;
                    } else {
                        xsltGenericError(xsltGenericErrorContext,
                                         " template :\"%s\"\n",
                                         &arg[offset]);
                        return;
                    }
                } else if (arg[1] == 's') {
                    /*quickly switch to another stylesheet node */
                    xmlXPathContextPtr pctxt =
                        xmlXPathNewContext(source->doc);
                    if (pctxt == NULL) {
                        xmlFree(ctxt);
                        xslDebugStatus = DEBUG_QUIT;
                        return;
                    }
                    if (!xmlXPathNsLookup(pctxt, (xmlChar *) "xsl"))
                        xmlXPathRegisterNs(pctxt, (xmlChar *) "xsl",
                                           XSLT_NAMESPACE);
                    list = xmlXPathEval((xmlChar *) & arg[offset], pctxt);
                    if (pctxt) {
                        xmlFree(pctxt);
                    }
                } else {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Unknown option to cd\n");
                }
            } else
                xsltGenericError(xsltGenericErrorContext,
                                 "Unable to cd, No stylesheet properly parsed\n");
        } else {
            xmlNodePtr savenode;

            if (styleCtxt) {
                savenode = styleCtxt->xpathCtxt->node;
                ctxt->pctxt->node = ctxt->node;
                styleCtxt->xpathCtxt->node = ctxt->node;
                if (!xmlXPathNsLookup(ctxt->pctxt, (xmlChar *) "xsl"))
                    xmlXPathRegisterNs(ctxt->pctxt, (xmlChar *) "xsl",
                                       XSLT_NAMESPACE);
                list = xmlXPathEval((xmlChar *) arg, styleCtxt->xpathCtxt);
                styleCtxt->xpathCtxt->node = savenode;
            } else if (ctxt->pctxt) {
                if (!xmlXPathNsLookup(ctxt->pctxt, (xmlChar *) "xsl"))
                    xmlXPathRegisterNs(ctxt->pctxt, (xmlChar *) "xsl",
                                       XSLT_NAMESPACE);
                list = xmlXPathEval((xmlChar *) arg, ctxt->pctxt);
            } else {
                xsltGenericError(xsltGenericErrorContext,
                                 "Invalid parameters to xslDbgCd\n");
            }
        }

        if (list != NULL) {
            switch (list->type) {
                case XPATH_NODESET:
                    if (list->nodesetval) {
                        if (list->nodesetval->nodeNr == 1) {
                            ctxt->node = list->nodesetval->nodeTab[0];
                        } else
                            xmlGenericError(xmlGenericErrorContext,
                                            "%s is a %d Node Set\n",
                                            arg, list->nodesetval->nodeNr);
                    } else {
                        xmlGenericError(xmlGenericErrorContext,
                                        "%s is a empty Node Set\n ", arg);
                    }
                    break;

                default:
                    xmlShellPrintXPathError(list->type, (char *) arg);
            }
            xmlXPathFreeObject(list);
        } else {
            xmlGenericError(xmlGenericErrorContext,
                            "%s: no such node\n", arg);
        }
        ctxt->pctxt->node = NULL;
    }
}







/**
 * xslDbgPrintCallStack:
 * @arg : the number of frame to print, NULL if all items 
 * 
 * Print all items found on the callStack
 */
void
xslDbgPrintCallStack(const xmlChar* arg)
{
  int callDepth;
  xslCallPointPtr callPoint;
  if (arg == NULL){
    for (callDepth = 1; callDepth <= xslCallDepth(); callDepth++) {
      callPoint = xslGetCall(callDepth);
      if (callPoint && callPoint->info) {
	if (callDepth == 0)
	  xsltGenericError(xsltGenericErrorContext,
			   "Call stack contains:\n");
	xsltGenericError(xsltGenericErrorContext, "#%d template :\"%s\"",
			 callDepth - 1, callPoint->info->templateName);
	if (callPoint->info->url)
	  xsltGenericError(xsltGenericErrorContext,
			   " in file %s : line %ld \n",
			   callPoint->info->url, callPoint->lineNo);
	else
	  xsltGenericError(xsltGenericErrorContext, "\n");
      } else {
#ifdef WITH_XSLT_DEBUG_PROCESS
	xsltGenericError(xsltGenericErrorContext,
			 "Call stack item not found at depth %d :"
			 " xslDbgPrintCallStack\n", callDepth);
#endif
	break;
      }
    }
    if (xslCallDepth() == 0)
      xsltGenericError(xsltGenericErrorContext,
		       "No items on call stack\n");
    else
      xsltGenericError(xsltGenericErrorContext, "\n");
  }else{
    long depth = atoi(arg);
    if (depth >=0 ){
      callPoint = xslGetCall(depth + 1);
      if (callPoint && callPoint->info) {
	xsltGenericError(xsltGenericErrorContext, "#%d template :\"%s\"",
			 depth, callPoint->info->templateName);
	/* should alays be present but .. */
	if (callPoint->info->url)
	  xsltGenericError(xsltGenericErrorContext,
			   " in file %s : line %ld \n",
			   callPoint->info->url, callPoint->lineNo);
	else
	  xsltGenericError(xsltGenericErrorContext, "\n");
      } else {
#ifdef WITH_XSLT_DEBUG_PROCESS
	xsltGenericError(xsltGenericErrorContext,
			 "Call stack item not found at depth %d :"
			 " xslDbgPrintCallStack\n", depth);
#endif
      }
    }
  }
}

/**
 * xsldbgSleep:
 * @delay : the number of microseconds to delay exection by
 *
 * Delay execution by a specified number of microseconds. On some system 
 *      this will not be at all accurate.
 */
void
xsldbgSleep(long delay)
{
#ifdef HAVE_USLEEP
    usleep(delay);
#else
#ifdef WIN32
    sleep(delay / 1000);
#else
    /* try to delay things by doing a lot of floating point 
     * multiplipation   
     */
    long loop1, loop2;
    float f1 = 1.0000001, f2;

    for (loop1 = 0; loop1 < 100000 * delay; loop1++)
        for (loop2 = 0; loop2 < 100000; loop2++) {
            f2 = f1 * f1;
        }
#endif
#endif
}


/**
 * Delay execution for time as indicated by OPTION_WALK_SPEED
 * Can only be called from within xslDbgShell!
 * OPTION_WALK_SPEED != WALKSPEED_STOP
 *
 * Returns 1 if walk is to continue,
 *         0 otherwise
 */
int
xslDbgWalkContinue(void)
{
    int result = 0, speed = getIntOption(OPTIONS_WALK_SPEED);

    switch (speed) {
        case WALKSPEED_1:
        case WALKSPEED_2:
        case WALKSPEED_3:
        case WALKSPEED_4:
        case WALKSPEED_5:
        case WALKSPEED_6:
        case WALKSPEED_7:
        case WALKSPEED_8:
        case WALKSPEED_9:
            /* see options.h for defintion of WALKDAY */
            xsldbgSleep(speed * WALKDELAY);
            result++;
            break;

        default:               /* stop walking */
            setIntOption(OPTIONS_WALK_SPEED, WALKSPEED_STOP);
	    xslDebugStatus = DEBUG_STOP;
            break;
    }

    return result;
}



/* Look up a name in a list  
   Stop looking when you reach a null entry in matchList
*/
int
lookupName(xmlChar * name, xmlChar ** matchList)
{
    int result = -1, nameIndex;

    for (nameIndex = 0; matchList[nameIndex]; nameIndex++) {
        if (!strcmp(name, matchList[nameIndex])) {
            result = nameIndex;
            break;
        }
    }

    return result;
}


/**
 * xslShellReadline:
 * @prompt:  the prompt value
 *
 * Read a string
 *
 * Returns a pointer to it or NULL on EOF the caller is expected to
 *     free the returned string.
 */
xmlChar *
xslDbgShellReadline(xmlChar * prompt)
{
    static char last_read[DEBUG_BUFFER_SIZE] = { '\0' };

#ifdef HAVE_READLINE
    xmlChar *line_read;

    /* Get a line from the user. */
    line_read = (xmlChar *) readline((char *) prompt);

    /* If the line has any text in it, save it on the history. */
    if (line_read && *line_read) {
        add_history((char *) line_read);
        strncpy(last_read, line_read, DEBUG_BUFFER_SIZE - 1);
    } else {
        /* if only <Enter>is pressed then try last saved command line */
        line_read = (xmlChar *) xmlMemStrdup(last_read);
    }
    return (line_read);
#else
    char line_read[DEBUG_BUFFER_SIZE];

    if (prompt != NULL)
        xsltGenericError(xsltGenericErrorContext, "%s", prompt);
    if (!fgets(line_read, DEBUG_BUFFER_SIZE - 1, stdin))
        return (NULL);
    line_read[DEBUG_BUFFER_SIZE - 1] = 0;
    /* if only <Enter>is pressed then try last saved command line */
    if ((strlen(line_read) == 0) || (line_read[0] == '\n')) {
        strcpy(line_read, last_read);
    } else {
        strcpy(last_read, line_read);
    }
    return (xmlChar *) xmlMemStrdup(line_read);
#endif
}

/**
 * trimString:
 * @text : valid string with leading or trailing spaces
 *
 * Remove leading and trailing spaces off @text
 *         stores result back into @text
 */
int
trimString(xmlChar * text)
{
    int result = 0;
    xmlChar *start, *end;

    if (text) {
        start = text;
        end = text + xmlStrLen(text) - 1;
        while (IS_BLANK(*start) && (start <= end))
            start++;

        while (IS_BLANK(*end) && (end >= start))
            end--;

        /* copy  to @text */
        while (start <= end) {
            *text = *start;
            text++;
            start++;
        }

        *text = '\0';
        result++;
    }
    return result;
}

/*
 * splitString:
 * @textIn : the string to split
 * @maxStrings : max number of strings to put into @out
 * @out: 
 * Spit string by white space and put into @out
 * 
 * Return 1 on success,
 *        0 otherwise
 */
int
splitString(xmlChar * textIn, int maxStrings, xmlChar ** out)
{
    int result = 0;

    while ((*textIn != '\0') && (result < maxStrings)) {
        /*skip the first spaces ? */
        while (IS_BLANK(*textIn))
            textIn++;
        out[result] = textIn;

        /* no word found only spaces ? */
        if (xmlStrLen(textIn) == 0)
            return result;

        /* look for end of word */
        while (!IS_BLANK(*textIn) && (*textIn != '\0'))
            textIn++;
        *textIn = '\0';
        textIn++;
        result++;
    }
    return result;
}


void
addBreakPointNode(void *payload, void *data ATTRIBUTE_UNUSED,
                  xmlChar * name ATTRIBUTE_UNUSED)
{
    xmlNodePtr node = searchBreakPointNode((xslBreakPointPtr) payload);
    xslSearchAdd(node);
}

void 
addSourceNode(void *payload, void *data ATTRIBUTE_UNUSED,
	      xmlChar * name ATTRIBUTE_UNUSED){
  xmlNodePtr node = searchSourceNode((xsltStylesheetPtr)payload);
  xslSearchAdd(node);
}

void 
addTemplateNode(void *payload, void *data ATTRIBUTE_UNUSED,
	      xmlChar * name ATTRIBUTE_UNUSED){
  xmlNodePtr node = searchTemplateNode(((xsltTemplatePtr)payload)->elem);
  xslSearchAdd(node);
}

void 
addGlobalNode(void *payload, void *data ATTRIBUTE_UNUSED,
	      xmlChar * name ATTRIBUTE_UNUSED){
  xmlNodePtr node = searchGlobalNode((xmlNodePtr)payload);
  xslSearchAdd(node);
}

void 
addLocalNode(void *payload, void *data ATTRIBUTE_UNUSED,
	      xmlChar * name ATTRIBUTE_UNUSED){
  xmlNodePtr node = searchLocalNode((xmlNodePtr)payload);
  xslSearchAdd(node);
}

void 
addIncludeNode(void *payload, void *data ATTRIBUTE_UNUSED,
	      xmlChar * name ATTRIBUTE_UNUSED){
  xmlNodePtr node = searchIncludeNode((xmlNodePtr)payload);
  xslSearchAdd(node);
}

void
addCallStackItems(void){
  xslCallPointPtr item;
  xmlNodePtr node;
  int depth;
  for (depth = xslCallDepth(); depth >0; depth--){
    item = xslGetCall(depth);
    if (item){
      node = searchCallStackNode(item);
      if (node)
	xslSearchAdd(node);
    }
  }
}

void
  updateSearchData(xsltTransformContextPtr styleCtxt, xsltStylesheetPtr style,
		 void *data, int variableTypes)
{
    xslSearchEmpty();
    xsltGenericError(xsltGenericErrorContext, 
		     "Updating search database, this may take a while ..\n");
    /* add items in the call stack to the search dataBase*/
    addCallStackItems();
    xsltGenericError(xsltGenericErrorContext,
		     "  Looking for breakpoints \n");
    walkBreakPoints((xmlHashScanner) addBreakPointNode, data);

    xsltGenericError(xsltGenericErrorContext,
		     "  Looking for imports and top level stylesheets \n");
    walkStylesheets((xmlHashScanner) addSourceNode, data, style);
    xsltGenericError(xsltGenericErrorContext,
		    "  Looking for includes \n");
    walkIncludes((xmlHashScanner) addIncludeNode, data, style);
    xsltGenericError(xsltGenericErrorContext,
		    "  Looking for templates \n");
    walkTemplates((xmlHashScanner) addTemplateNode, data, style);
    xsltGenericError(xsltGenericErrorContext,
		    "  Looking for global variables \n");
    walkGlobals((xmlHashScanner) addGlobalNode, data, style);
    xsltGenericError(xsltGenericErrorContext,
		     "  Looking for local variables \n");
    walkLocals((xmlHashScanner) addLocalNode, data, style);
    xsltGenericError(xsltGenericErrorContext,
		     "  Formatting output \n");    
    xslSearchSave("search.data");
}

/*
 * A break point has been found so pass control to user
 * @templ : The source node being executed
 * @node : The data node being processed
 * @root : The template being applied to "node"
 * @ctxt :
 * 
 * Start the xsldbg command prompt
 */
void
xslDebugBreak(xmlNodePtr templ, xmlNodePtr node, xsltTemplatePtr root,
              xsltTransformContextPtr ctxt)
{
    rootCopy = root;
    if (root) {
        if (root->match)
            xsltGenericError(xsltGenericErrorContext,
                             "\nReached template :\"%s\"\n", root->match);
        else if (root->name)
            xsltGenericError(xsltGenericErrorContext,
                             "\nReached template :\"%s\"\n", root->name);
    } else
        xsltGenericError(xsltGenericErrorContext, "\n");

    if (ctxt && ctxt->node && ctxt->node &&
        ctxt->node->doc && ctxt->node->doc->URL) {
        if (activeBreakPoint()) {
            xsltGenericError(xsltGenericErrorContext,
                             "Breakpoint %d ", activeBreakPoint()->id);
        } else {
            xsltGenericError(xsltGenericErrorContext, "Breakpoint ");
        }
    }

    xslDbgShell(templ, node, (xmlChar *) "index.xsl",
                (xmlShellReadlineFunc) xslDbgShellReadline, stdout, ctxt);

}


/* Highly modified function based on xmlShell */

/**
 * xslDbgShell
 * @doc:  the initial document
 * @filename:  the output buffer
 * @input:  the line reading function
 * @output:  the output FILE*
 *
 * Implements the XSL shell 
 * Work with stylesheet and data in a similar way to Unix command shell 
 */
void
xslDbgShell(xmlNodePtr source, xmlNodePtr doc, xmlChar * filename,
            xmlShellReadlineFunc input, FILE * output,
            xsltTransformContextPtr styleCtxt)
{
    xmlChar prompt[DEBUG_BUFFER_SIZE] = "/ > ";
    xmlChar *cmdline = NULL, *cur;
    int nbargs = 0;
    int loadedFiles = 0;
    int commandId = -1;         /* stores what was the last 
                                 * command id entered  by user */
    xmlChar command[DEBUG_BUFFER_SIZE]; /* holds the command user entered */
    xmlChar arg[DEBUG_BUFFER_SIZE];     /* holds any extra arguments to
                                         * command entered */
    xmlChar dir[DEBUG_BUFFER_SIZE];     /* temporary buffer used by where
                                         * and pwd commands */
    int shortCutId = -1;        /* used by cd command */
    int i;
    static int showSource = 1;  /* Do we first show source or data ? */

    /* for convenience keep track of which node was last 
     * selected of source and doc */
    xmlNodePtr lastSourceNode, lastDocNode;


    xmlShellCtxtPtr ctxt;
    int exitShell = 0;          /* Indicate when to exit xslShell */

    if (source == NULL)
        return;
    if (doc == NULL)
        return;
    if (filename == NULL)
        return;
    if (input == NULL)
        return;
    if (output == NULL)
        return;
    ctxt = (xmlShellCtxtPtr) xmlMalloc(sizeof(xmlShellCtxt));
    if (ctxt == NULL)
        return;

    /* flag that we've received control */
    xslDebugGotControl(1);
    ctxt->loaded = 0;
    lastSourceNode = source;
    lastDocNode = doc;
    /* show the doc or source first? */
    if (showSource) {
        ctxt->doc = source->doc;
        ctxt->node = source;
    } else {
        ctxt->doc = doc->doc;
        ctxt->node = (xmlNodePtr) doc;
    }
    ctxt->input = input;
    ctxt->output = output;
    ctxt->filename = (char *) xmlStrdup((xmlChar *) filename);

    if (ctxt->node && ctxt->node && ctxt->node->doc
        && ctxt->node->doc->URL) {
        if (activeBreakPoint() != NULL) {
            xslBreakPointPtr breakPtr = activeBreakPoint();

            xsltGenericError(xsltGenericErrorContext,
                             "in file %s : line %ld \n", breakPtr->url,
                             breakPtr->lineNo);
        } else {
            if (xmlGetLineNo(ctxt->node) != -1)
                xsltGenericError(xsltGenericErrorContext,
                                 "at file %s : line %ld \n",
                                 ctxt->node->doc->URL,
                                 xmlGetLineNo(ctxt->node));
            else
                xsltGenericError(xsltGenericErrorContext,
                                 "@ text node in file %s\n",
                                 ctxt->node->doc->URL);
        }
    }

    if (xslDebugStatus == DEBUG_TRACE) {
        xmlFree(ctxt);
        return;
    }
#ifdef HAVE_USLEEP
    if (xslDebugStatus == DEBUG_WALK) {
        if (xslDbgWalkContinue()) {
            xmlFree(ctxt);
            return;
        }
    }
#endif

    ctxt->pctxt = xmlXPathNewContext(ctxt->doc);
    if (ctxt->pctxt == NULL) {
        xmlFree(ctxt);
        return;
    }

    while (!exitShell) {
        if (ctxt->node == (xmlNodePtr) ctxt->doc)
            snprintf((char *) prompt, DEBUG_BUFFER_SIZE - 1,
                     "(xsldbg) %s > ", "/");
        else if ((ctxt->node->name) && (ctxt->node->ns))
            snprintf((char *) prompt, DEBUG_BUFFER_SIZE - 1,
                     "(xsldbg) %s:%s > ", ctxt->node->ns->prefix,
                     ctxt->node->name);
        else if (ctxt->node->name)
            snprintf((char *) prompt, DEBUG_BUFFER_SIZE - 1,
                     "(xsldbg) %s > ", ctxt->node->name);

        else
            snprintf((char *) prompt, DEBUG_BUFFER_SIZE - 1,
                     "(xsldbg) ? > ");
        prompt[sizeof(prompt) - 1] = 0;

	
        /*
         * Get a new command line
         */
	/*	cmdline =  xslDbgShellReadline(prompt);*/
	cmdline = (xmlChar *) ctxt->input((char *) prompt);
        if (cmdline == NULL)
            break;

        /* remove leading/trailing blanks */
        trimString(cmdline);

        /*
         * Parse the command itself
         */
        cur = cmdline;
        nbargs = 0;
        while ((*cur == ' ') || (*cur == '\t'))
            cur++;
        i = 0;
        while ((*cur != ' ') && (*cur != '\t') &&
               (*cur != '\n') && (*cur != '\r')) {
            if (*cur == 0)
                break;
            command[i++] = *cur++;
        }
        command[i] = 0;
        if (i == 0)
            continue;
        nbargs++;

        /*
         * Parse the argument
         */
        while ((*cur == ' ') || (*cur == '\t'))
            cur++;
        i = 0;
        while ((*cur != '\n') && (*cur != '\r') && (*cur != 0)) {
            if (*cur == 0)
                break;
            arg[i++] = *cur++;
        }
        arg[i] = 0;
        if (i != 0)
            nbargs++;

        commandId = lookupName(command, (xmlChar **) commandNames);
        /* try command shorts if command is not found */
        if (commandId < 0)
            commandId =
                lookupName(command, (xmlChar **) shortCommandNames);

        /*
         * start interpreting the command
         */
        switch (commandId + DEBUG_HELP_CMD) {
                /* --- Help related commands --- */
            case DEBUG_HELP_CMD:
                helpTop(arg);
                break;


                /* --- Running related commands --- */
            case DEBUG_BYE_CMD:
            case DEBUG_EXIT_CMD:
            case DEBUG_QUIT_CMD:
                /* allow the stylesheet to exit */
                xslDebugStatus = DEBUG_QUIT;
                exitShell++;
                break;

            case DEBUG_STEP_CMD:
                xslDebugStatus = DEBUG_STEP;
                exitShell++;
                break;

            case DEBUG_STEPUP_CMD:
                {
                    xmlChar *noOfFrames = arg;

                    // skip until next space character
                    while (*noOfFrames && (*noOfFrames != ' ')) {
                        noOfFrames++;
                    }
                    xslDbgShellFrameBreak(noOfFrames, 1);
                    exitShell++;
                }
                break;

            case DEBUG_STEPDOWN_CMD:
                {
                    xmlChar *noOfFrames = arg;

                    // skip until next space character
                    while (*noOfFrames && (*noOfFrames != ' ')) {
                        noOfFrames++;
                    }
                    xslDbgShellFrameBreak(noOfFrames, 0);
                    exitShell++;
                }
                break;

                /* continue to next break point */
            case DEBUG_CONT_CMD:
                xslDebugStatus = DEBUG_CONT;
                exitShell++;
                break;

                /* restart */
            case DEBUG_RUN_CMD:
                xslDebugStatus = DEBUG_RUN_RESTART;
                exitShell++;
                break;


                /* --- Template related commands --- */
            case DEBUG_TEMPLATES_CMD:
                {
                    int allFiles = 1, verbose = 1;

                    if (xmlStrLen(arg) && 
			(xmlStrCmp(arg, "this") == 0)) {
                            allFiles = 0;
                    }

                    /* be verbose when printing template names */
                    /* if args is not empty then print names this stylesheet */
                    xslDbgPrintTemplateNames(styleCtxt, ctxt, arg, verbose,
                                             allFiles);
                    break;
                }


            case DEBUG_WHERE_CMD:
                /* Print the current working directory as well */
                xslDbgPrintCallStack(NULL);
                if (!xmlShellPwd(ctxt, (char *) dir, ctxt->node, NULL))
                    xsltGenericError(xsltGenericErrorContext, "%s", dir);
                if (ctxt->node && ctxt->node && ctxt->node->doc
                    && ctxt->node->doc->URL)
                    if (activeBreakPoint() != NULL) {
                        xsltGenericError(xsltGenericErrorContext,
                                         " in file %s : line %ld \n",
                                         ctxt->node->doc->URL,
                                         xmlGetLineNo(ctxt->node));
                    } else {
                        xsltGenericError(xsltGenericErrorContext,
                                         " at file %s : line %ld \n",
                                         ctxt->node->doc->URL,
                                         xmlGetLineNo(ctxt->node));
                } else
                    xsltGenericError(xsltGenericErrorContext, "\n");
                break;

	case DEBUG_FRAME_CMD:
	  xslDbgPrintCallStack(arg);
	  break;

	case DEBUG_STYLESHEETS_CMD:
	  xslDbgPrintStyleSheets(arg);
	  break;

                /* --- Break point related commands --- */
            case DEBUG_BREAK_CMD:
                if (styleCtxt) {
                    if (xmlStrLen(arg))
                        xslDbgShellBreak(arg, styleCtxt->style);
                    else {
                        /* select current node to break at */
                        xmlChar buff[100];

                        if (ctxt->node->doc && ctxt->node->doc->URL)
                            snprintf((char *) buff, 99, "-l %s %ld",
                                     ctxt->node->doc->URL,
                                     xmlGetLineNo(ctxt->node));
                        xslDbgShellBreak(buff, styleCtxt->style);
                    }
                } else {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Files not loaded try using run command first\n");
                }
                break;

            case DEBUG_SHOWBREAK_CMD:
                xsltGenericError(xsltGenericErrorContext, "\n");
                printCount = 0; /* printCount will get updated by
                                 * xslDbgPrintBreakPoint */
                walkBreakPoints((xmlHashScanner) xslDbgPrintBreakPoint,
                                   NULL);
                if (printCount == 0)
                    xsltGenericError(xsltGenericErrorContext,
                                     "\nNo file break points set:\n");
                else
                    xsltGenericError(xsltGenericErrorContext,
                                     "\n\t Total of %d breakPoints present\n",
                                     printCount);
                break;

            case DEBUG_DELETE_CMD:
	      if (styleCtxt != NULL){
                if (xmlStrLen(arg))
                    xslDbgShellDelete((xmlChar *) arg);
                else {
                    xslBreakPointPtr breakPoint = NULL;

                    if (ctxt->node->doc && ctxt->node->doc->URL)
                        breakPoint = xslGetBreakPoint(ctxt->node->doc->URL,
                                                      xmlGetLineNo(ctxt->
                                                                   node));
                    if (!breakPoint || !xslDeleteBreakPoint(breakPoint))
                        xsltGenericError(xsltGenericErrorContext,
                                         "Unable to add delete point");
                }
	      }else {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Files not loaded try using run command first\n");
	      }
	      
                break;

            case DEBUG_ENABLE_CMD:
                if (xmlStrLen(arg))
                    xslDbgShellEnable(arg, XSL_TOGGLE_BREAKPOINT);
                else {
                    xslBreakPointPtr breakPoint = NULL;

                    if (ctxt->node->doc && ctxt->node->doc->URL)
                        breakPoint = xslGetBreakPoint(ctxt->node->doc->URL,
                                                      xmlGetLineNo(ctxt->
                                                                   node));
                    if (!breakPoint
                        ||
                        (!xslEnableBreakPoint
                         (breakPoint, !breakPoint->enabled)))
                        xsltGenericError(xsltGenericErrorContext,
                                         "Unable to add enable/disable point\n");
                }
                break;

            case DEBUG_DISABLE_CMD:
                if (xmlStrLen(arg))
                    xslDbgShellEnable(arg, 0);
                else {
                    xslBreakPointPtr breakPoint = NULL;

                    if (ctxt->node->doc && ctxt->node->doc->URL)
                        breakPoint = xslGetBreakPoint(ctxt->node->doc->URL,
                                                      xmlGetLineNo(ctxt->
                                                                   node));
                    if (!breakPoint
                        ||
                        (!xslEnableBreakPoint
                         (breakPoint, !!breakPoint->enabled)))
                        xsltGenericError(xsltGenericErrorContext,
                                         "Unable to add enable/disable point\n");
                }
                break;



                /* --- Node view related commands --- */
            case DEBUG_LS_CMD:
                xslDbgShellPrintList(ctxt, arg, 0);
                break;

            case DEBUG_DIR_CMD:
                xslDbgShellPrintList(ctxt, arg, 1);
                break;

            case DEBUG_DU_CMD:
                xmlShellDu(ctxt, NULL, ctxt->node, NULL);
                break;

            case DEBUG_CAT_CMD:
            case DEBUG_PRINT_CMD:
                xslDbgShellCat(styleCtxt, ctxt, arg);
                break;

            case DEBUG_PWD_CMD:
                if (!xmlShellPwd(ctxt, (char *) dir, ctxt->node, NULL)) {
                    xsltGenericError(xsltGenericErrorContext, "\n%s", dir);
                    if (ctxt->node && ctxt->node->doc
                        && ctxt->node->doc->URL)
                        xsltGenericError(xsltGenericErrorContext,
                                         " in file %s : line %ld",
                                         ctxt->node->doc->URL,
                                         xmlGetLineNo(ctxt->node));
                }
                xsltGenericError(xsltGenericErrorContext, "\n");

                break;

            case DEBUG_DUMP_CMD:
                xmlDebugDumpDocument(stdout, ctxt->doc);
                break;

            case DEBUG_BASE_CMD:
                xmlShellBase(ctxt, NULL, ctxt->node, NULL);
                break;


                /* ---  Variable related commands --- */
            case DEBUG_GLOBALS_CMD:
                if (loadedFiles == 0)
                    xslDbgShellPrintVariable(styleCtxt, arg,
                                        DEBUG_GLOBAL_VAR);
                else
                    xsltGenericError(xsltGenericErrorContext,
                                     "Need to use run command first\n");
                break;

            case DEBUG_LOCALS_CMD:
                if (loadedFiles == 0)
                    xslDbgShellPrintVariable(styleCtxt, arg,
                                        DEBUG_LOCAL_VAR);
                else
                    xsltGenericError(xsltGenericErrorContext,
                                     "Need to use run command first\n");
                break;


		/* It's difficult to put the following commands into 
		   a separe file so they stay here!*/
                /* --- Node selection related commands --- */
            case DEBUG_SOURCE_CMD:
                if (xmlStrLen(arg) == 0) {
                    if (ctxt->doc == doc->doc)
                        lastDocNode = ctxt->node;
                    ctxt->doc = source->doc;
                    ctxt->node = lastSourceNode;
                    ctxt->pctxt = xmlXPathNewContext(ctxt->doc);
                    showSource = 1;
                    if (ctxt->pctxt == NULL) {
                        xmlFree(ctxt);
                        xslDebugStatus = DEBUG_QUIT;
                        return;
                    } else
                        break;
                } else {
                    /* load new stylesheet file, actual loading happens later */
                     xmlChar *buff = dir;    /* use dir command temp buffer */

                        /* replace ~ with home path */
                        if ((arg[0] == '~') && getenv("HOME")) {
                            strcpy(buff, getenv("HOME"));
                            if (xmlStrLen(buff) + xmlStrLen(arg) <
                                DEBUG_BUFFER_SIZE) {
                                xmlStrCat(buff, &arg[1]);
                                setStringOption(OPTIONS_SOURCE_FILE_NAME,
                                                buff);
                            } else {
                                xsltGenericError(xsltGenericErrorContext,
                                                 "File name too large\n");
                                break;
                            }
                        } else {
                            setStringOption(OPTIONS_SOURCE_FILE_NAME, arg);
                        }
                        xsltGenericError(xsltGenericErrorContext,
                                         "Load of source deferred use run command\n");
                        loadedFiles = 1;
                }
                break;

            case DEBUG_DATA_CMD:
                if (xmlStrLen(arg) == 0) {
                    if (ctxt->doc == source->doc)
                        lastSourceNode = ctxt->node;
                    ctxt->doc = doc->doc;
                    ctxt->node = lastDocNode;
                    ctxt->pctxt = xmlXPathNewContext(ctxt->doc);
                    showSource = 0;
                    if (ctxt->pctxt == NULL) {
                        xmlFree(ctxt);
                        xslDebugStatus = DEBUG_QUIT;
                        return;
                    } else
                        break;
                } else {
                    /* load new xml file actual loading hapens later */
                        xmlChar *buff = dir;    /* use dir command temp buffer */

                        /* replace ~ with home path */
                        if ((arg[0] == '~') && getenv("HOME")) {
                            xmlStrCpy(buff, getenv("HOME"));
                            if (xmlStrLen(buff) + xmlStrLen(arg) <
                                DEBUG_BUFFER_SIZE) {
                                xmlStrCat(buff, &arg[1]);
                                setStringOption(OPTIONS_DATA_FILE_NAME,
                                                buff);
                            } else {
                                xsltGenericError(xsltGenericErrorContext,
                                                 "File name too large\n");
                                break;
                            }
                        } else {
                            setStringOption(OPTIONS_DATA_FILE_NAME, arg);
                        }

                        loadedFiles = 1;
                        xsltGenericError(xsltGenericErrorContext,
                                         "Load of xml data deferred use run command\n");
									}
                break;

            case DEBUG_CD_CMD:
                /* use dir as a working buffer */
                strncpy(dir, arg, 2);
                dir[2] = '\0';
                shortCutId = lookupName(dir, (xmlChar **) cdShortCuts);
                if (shortCutId >= 0) {
                    if (xmlStrLen(arg) == 2) {
                        xslDbgCd(styleCtxt, ctxt,
                                 (xmlChar *) cdAlternative[shortCutId],
                                 source);
                    } else {

                        xmlStrCpy(dir, cdAlternative[shortCutId]);
                        xmlStrCat(dir, &arg[2]);
                        xslDbgCd(styleCtxt, ctxt, dir, source);
                    }

                } else
                    xslDbgCd(styleCtxt, ctxt, arg, source);
                break;


                /* --- File related commands --- */
            case DEBUG_VALIDATE_CMD:
                xsltGenericError(xsltGenericErrorContext,
                                 "validate disabled\n");
                /*
                 * xmlShellValidate(ctxt, arg, NULL, NULL);
                 */
                break;

            case DEBUG_LOAD_CMD:
                xsltGenericError(xsltGenericErrorContext,
                                 "loading disabled\n");
                /*
                 * xmlShellLoad(ctxt, arg, NULL, NULL);
                 */
                break;

            case DEBUG_SAVE_CMD:
                xsltGenericError(xsltGenericErrorContext,
                                 "saving disabled\n");
                /*
                 * xmlShellSave(ctxt, arg, NULL, NULL);
                 */
                break;

            case DEBUG_WRITE_CMD:
                xsltGenericError(xsltGenericErrorContext,
                                 "write disabled\n");
                /*
                 * xmlShellWrite(ctxt, arg, NULL, NULL);
                 */
                break;

            case DEBUG_FREE_CMD:
                xsltGenericError(xsltGenericErrorContext,
                                 "free disabled\n");
                /*
                 * if (arg[0] == 0) {
                 * xmlMemShow(stdout, 0);
                 * } else {
                 * int len = 0;
                 * sscanf(arg, "%d", &len);
                 * xmlMemShow(stdout, len);
                 * }
                 */
                break;


                /* operating system related */
            case DEBUG_CHDIR_CMD:
                xslDbgShellChangeWd(arg);
                break;

            case DEBUG_SHELL_EXEC_CMD:
                xslDbgShellExecute(arg, 1);
                break;


                /* libxslt parameter related */
            case DEBUG_ADDPARAM_CMD:
                xslDbgShellAddParam(arg);
                break;

            case DEBUG_DELPARAM_CMD:
                xslDbgShellDelParam(arg);
                break;

            case DEBUG_SHOWPARAM_CMD:
	      xslDbgShellShowParam(arg);
                break;

	case	DEBUG_TTY_CMD:
	  redirectToTerminal(arg);
	  break;
	  


                /* tracing related commands */
            case DEBUG_TRACE_CMD:
                if (xslDbgShellTrace(arg))
                    exitShell++;
                break;

            case DEBUG_WALK_CMD:
                if (xslDbgShellWalk(arg))
                    exitShell++;
                break;

		/* search related commands */
            case DEBUG_SEARCH_CMD:
                xslDbgShellSearch(styleCtxt, getStylesheet(), arg);
                break;

            default:
                xmlGenericError(xmlGenericErrorContext,
                                "Unknown command %s, try help\n", command);
        }

	/* kdbg like to get the marker after every command so here it is */
	if (isOptionEnabled(OPTIONS_GDB)){
	  if (ctxt->node && ctxt->node && ctxt->node->doc
	      && ctxt->node->doc->URL) {
	    
	    if (activeBreakPoint() != NULL) {
	      xslBreakPointPtr breakPtr = activeBreakPoint();

	      xsltGenericError(xsltGenericErrorContext,
			       "Breakpoint in file %s : line %ld \n", breakPtr->url,
			       breakPtr->lineNo);
	    } else {
	      if (xmlGetLineNo(ctxt->node) != -1)
                xsltGenericError(xsltGenericErrorContext,
                                 "Breakpoint at file %s : line %ld \n",
                                 ctxt->node->doc->URL,
                                 xmlGetLineNo(ctxt->node));
	      else
                xsltGenericError(xsltGenericErrorContext,
                                 "BreakPoint @ text node in file %s\n",
                                 ctxt->node->doc->URL);
	    }
	  }	  
	}

        xmlFree(cmdline);
        cmdline = NULL;
    }

    xmlXPathFreeContext(ctxt->pctxt);

    if (ctxt->filename != NULL)
        xmlFree(ctxt->filename);
    xmlFree(ctxt);
    if (cmdline != NULL)
        xmlFree(cmdline);
    setActiveBreakPoint(0);

}


