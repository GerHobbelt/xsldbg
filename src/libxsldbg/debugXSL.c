
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

#ifdef WIN32
#include <wtypes.h>
#include <winbase.h>            /* needed fort the sleep function */
#endif

#include "xsldbg.h"
#include "files.h"
#include "cmds.h"               /* list of command Id's */
#include "debugXSL.h"
#include "options.h"
#include "xslbreakpoint.h"
#include "help.h"
#include <stdlib.h>
#include <libxslt/transform.h>  /* needed by source command */
#include <libxslt/xsltInternals.h>
#include <libxml/debugXML.h>
#include <stdio.h>

#include <libxsldbg/xsldbgmsg.h>
#include <libxsldbg/xsldbgio.h>

/* current template being processed */
xsltTemplatePtr rootCopy;

/* how may items have been printed */
int printCount;

/* used to sending small amounts data when xsldbg runs as a thread */
xmlChar messageBuffer[2000];

extern FILE *terminalIO;

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
    "output", /* output file name */
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
    
    /*variable value change */
    "set",

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
    "c",                        /* continue */
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
    "o", /* output file name */
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
    
    /*variable value change */
    "set",

    NULL                        /* Indicate the end of list */
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
enum ShortcutsEnum {
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


#include <libxml/xmlmemory.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/valid.h>
#include <libxml/debugXML.h>
#include <libxml/HTMLtree.h>
#include <libxml/HTMLparser.h>
#include <libxml/xmlerror.h>


/* -----------------------------------------
   Private function declarations for debugXSL.c
 -------------------------------------------*/


/* xslDbgCd :
 * Change directories
 * @styleCtxt : current stylesheet context
 * @ctxt : current shell context
 * @name : path to change to
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int xslDbgCd(xsltTransformContextPtr styleCtxt, xmlShellCtxtPtr ctxt,
             xmlChar * arg, xmlNodePtr source);


/**
 * xslDbgPrintCallStack:
 * @arg : the number of frame to print, NULL if all items 
 * 
 * Print all items found on the callStack
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int xslDbgPrintCallStack(const xmlChar * arg);


/**
 * xslDbgSleep:
 * @delay : the number of microseconds to delay exection by
 *
 * Delay execution by a specified number of microseconds. On some system 
 *      this will not be at all accurate.
 */
void xslDbgSleep(long delay);


/**
 * xslDbgWalkContinue:
 *
 * Delay execution for time as indicated by OPTION_WALK_SPEED
 * Can only be called from within xslDbgShell!
 * OPTION_WALK_SPEED != WALKSPEED_STOP
 *
 * Returns 1 if walk is to continue,
 *         0 otherwise
 */
int xslDbgWalkContinue(void);


/**
 * lookupName:
 * @name : is valid
 * @matchList : a NULL terminated list of names to use as lookup table
 *
 * Returns The id of name found in @matchList
 *        0 otherwise
*/
int lookupName(xmlChar * name, xmlChar ** matchList);


/**
 * addBreakPointNode:
 * @payload : valid xslBreakPointPtr
 * @data : not used
 * @name : not used
 * 
 * Convert payload into format needed, and add to search dataBase 
 */
void
  addBreakPointNode(void *payload, void *data ATTRIBUTE_UNUSED,
                    xmlChar * name ATTRIBUTE_UNUSED);


/**
 * addSourceNode:
 * @payload : valid xsltStylesheetPtr
 * @data : not used
 * @name : not used
 * 
 * Convert payload into format needed, and add to search dataBase 
 */
void
  addSourceNode(void *payload, void *data ATTRIBUTE_UNUSED,
                xmlChar * name ATTRIBUTE_UNUSED);


/**
 * addTemplateNode:
 * @payload : valid xsltTemplatePtr
 * @data : not used
 * @name : not used
 * 
 * Convert payload into format needed, and add to search dataBase 
 */
void
  addTemplateNode(void *payload, void *data ATTRIBUTE_UNUSED,
                  xmlChar * name ATTRIBUTE_UNUSED);

/**
 * addGlobalNode:
 * @payload : valid xmlNodePtr of global variable
 * @data : not used
 * @name : not used
 * 
 * Convert payload into format needed, and add to search dataBase 
 */
void
  addGlobalNode(void *payload, void *data ATTRIBUTE_UNUSED,
                xmlChar * name ATTRIBUTE_UNUSED);

/**
 * addLocalNode:
 * @payload : valid xmlNodePtr of local variable
 * @data : not used
 * @name : not used
 * 
 * Convert payload into format needed, and add to search dataBase 
 */
void
  addLocalNode(void *payload, void *data ATTRIBUTE_UNUSED,
               xmlChar * name ATTRIBUTE_UNUSED);


/**
 * addIncludeNode:
 * @payload : valid xmlNodePtr of include instuction
 * @data : not used
 * @name : not used
 * 
 * Convert payload into format needed, and add to search dataBase 
 */
void
  addIncludeNode(void *payload, void *data ATTRIBUTE_UNUSED,
                 xmlChar * name ATTRIBUTE_UNUSED);



/**
 * addCallStackItems:
 * 
 * Convert call stack items into format needed, and add to search dataBase 
 */
void
  addCallStackItems(void);


/**
 * xsldbgUpdateFileDetails:
 * @node : A valid node
 * 
 * Update the URL and  line number that we stoped at 
 */
static void xsldbgUpdateFileDetails(xmlNodePtr node);

/* ------------------------------------- 
    End private functions
---------------------------------------*/



/**
 * getTemplate:
 * 
 * Return the last template node found, if an
 *
 * Returns The last template node found, if any
 */
xsltTemplatePtr
getTemplate(void)
{
    return rootCopy;
}

static int currentLineNo = -1;
static xmlChar *currentUrl = NULL;

/**
 * xsldbgUpdateFileDetails:
 * @node : A valid node
 * 
 * Update the URL and  line number that we stoped at 
 */
static void
xsldbgUpdateFileDetails(xmlNodePtr node)
{
    if ((node != NULL) && (node->doc != NULL) && (node->doc->URL != NULL)) {
        if (currentUrl != NULL)
            xmlFree(currentUrl);
        currentUrl = (xmlChar *) xmlMemStrdup((char *) node->doc->URL);
        currentLineNo = xmlGetLineNo(node);
    }
}

/**
 * xsldbgLineNo:
 *
 * What line number are we at 
 *
 * Returns The current line number of xsldbg, may be -1
 **/
int
xsldbgLineNo(void)
{
    return currentLineNo;
}


/**
 * xsldbgUrl:
 * 
 * What URL did we stop at
 *
 * Returns A NEW copy of URL stopped at. Caller must free memory for URL.
 *  May be NULL  
 */
xmlChar *
xsldbgUrl(void)
{
    if (currentUrl != NULL)
        return (xmlChar *) xmlMemStrdup((char *) currentUrl);
    else
        return NULL;
}

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
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
xslDbgCd(xsltTransformContextPtr styleCtxt, xmlShellCtxtPtr ctxt,
         xmlChar * arg, xmlNodePtr source)
{
    xmlXPathObjectPtr list = NULL;
    int result = 0;
    int offset = 2;             /* in some cases I'm only interested after first two chars  */

    if (!ctxt) {
        xsltGenericError(xsltGenericErrorContext,
                         "Debuger has no files loaded, try reloading files\n");
        return result;
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
                        findTemplateNode(styleCtxt->style, &arg[offset]);
                    if (!templateNode) {
                        xsltGenericError(xsltGenericErrorContext,
                                         "Template '%s' not found\n",
                                         &arg[offset]);
                        return result;
                    } else {
                        xsltGenericError(xsltGenericErrorContext,
                                         " template :\"%s\"\n",
                                         &arg[offset]);
                        ctxt->node = templateNode;
                        result++;
                        return result;
                    }
                } else if (arg[1] == 's') {
                    /*quickly switch to another stylesheet node */
                    xmlXPathContextPtr pctxt =
                        xmlXPathNewContext(source->doc);
                    if (pctxt == NULL) {
                        xmlFree(ctxt);
                        /* xslDebugStatus = DEBUG_QUIT; */
                        return result;
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
                            result++;
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
    return result;
}


/**
 * xslDbgPrintCallStack:
 * @arg : the number of frame to print, NULL if all items 
 * 
 * Print all items found on the callStack
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
xslDbgPrintCallStack(const xmlChar * arg)
{
    int depth;
    int result = 0;
    xslCallPointPtr callPoint;

    if (arg == NULL) {
        for (depth = 1; depth <= callDepth(); depth++) {
            callPoint = getCall(depth);
            if (callPoint && callPoint->info) {
                if (depth == 0)
                    xsltGenericError(xsltGenericErrorContext,
                                     "Call stack contains:\n");
                xsltGenericError(xsltGenericErrorContext,
                                 "#%d template :\"%s\"", depth - 1,
                                 callPoint->info->templateName);
                if (callPoint->info->url)
                    xsltGenericError(xsltGenericErrorContext,
                                     " in file %s : line %ld \n",
                                     callPoint->info->url,
                                     callPoint->lineNo);
                else
                    xsltGenericError(xsltGenericErrorContext, "\n");
            } else {
#ifdef WITH_XSLT_DEBUG_PROCESS
                xsltGenericError(xsltGenericErrorContext,
                                 "Call stack item not found at depth %d :"
                                 " xslDbgPrintCallStack\n", depth);
#endif
                break;
            }
        }
        if (callDepth() == 0)
            xsltGenericError(xsltGenericErrorContext,
                             "No items on call stack\n");
        else
            xsltGenericError(xsltGenericErrorContext, "\n");
    } else {
        long templateDepth = atol((char *) arg);

        if (templateDepth >= 0) {
            callPoint = getCall(templateDepth + 1);
            if (callPoint && callPoint->info) {
                xsltGenericError(xsltGenericErrorContext,
                                 "#%d template :\"%s\"", templateDepth,
                                 callPoint->info->templateName);
                /* should alays be present but .. */
                if (callPoint->info->url)
                    xsltGenericError(xsltGenericErrorContext,
                                     " in file %s : line %ld \n",
                                     callPoint->info->url,
                                     callPoint->lineNo);
                else
                    xsltGenericError(xsltGenericErrorContext, "\n");
            } else {
#ifdef WITH_XSLT_DEBUG_PROCESS
                xsltGenericError(xsltGenericErrorContext,
                                 "Call stack item not found at templateDepth %d :"
                                 " xslDbgPrintCallStack\n", depth);
#endif
            }
        }
    }
    result++;
    return result;
}


/**
 * xslDbgSleep:
 * @delay : the number of microseconds to delay exection by
 *
 * Delay execution by a specified number of microseconds. On some system 
 *      this will not be at all accurate.
 */
void
xslDbgSleep(long delay)
{
#ifdef HAVE_USLEEP
    usleep(delay);
#else
#ifdef WIN32
    Sleep(delay / 1000);
#else
    /* try to delay things by doing a lot of floating point 
     * multiplication   
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
 * xslDbgWalkContinue:
 *
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
            xslDbgSleep(speed * WALKDELAY);
            result++;
            break;

        default:               /* stop walking */
            setIntOption(OPTIONS_WALK_SPEED, WALKSPEED_STOP);
            xslDebugStatus = DEBUG_STOP;
            break;
    }

    return result;
}


/**
 * lookupName:
 * @name : is valid
 * @matchList : a NULL terminated list of names to use as lookup table
 *
 * Returns The id of name found in @matchList
 *        0 otherwise
*/
int
lookupName(xmlChar * name, xmlChar ** matchList)
{
    int result = -1, nameIndex;

    for (nameIndex = 0; matchList[nameIndex]; nameIndex++) {
        if (!xmlStrCmp(name, matchList[nameIndex])) {
            result = nameIndex;
            break;
        }
    }

    return result;
}


/**
 * trimString:
 * @text : A valid string with leading or trailing spaces
 *
 * Remove leading and trailing spaces off @text
 *         stores result back into @text
 * Returns 1 on success,
 *         0 otherwise
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


/**
 * splitString:
 * @textIn: The string to split
 * @maxStrings: The max number of strings to put into @out
 * @out: Is valid and at least the size of @maxStrings
 *
 * Split string by white space and put into @out
 *
 * Returns 1 on success,
 *         0 otherwise
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

	if (*textIn != '\0')
	  {
	    *textIn = '\0';
	    textIn++;
	    result++;
	  }
	else{
	  *textIn = '\0';
	  textIn++;
	  result++;
	  break;
	}
    }
    return result;
}


/**
 * addBreakPointNode:
 * @payload : valid xslBreakPointPtr
 * @data : not used
 * @name : not used
 * 
 * Convert payload into format needed, and add to search dataBase 
 */
void
addBreakPointNode(void *payload, void *data ATTRIBUTE_UNUSED,
                  xmlChar * name ATTRIBUTE_UNUSED)
{
    xmlNodePtr node = searchBreakPointNode((xslBreakPointPtr) payload);

    searchAdd(node);
}


/**
 * addSourceNode:
 * @payload : valid xsltStylesheetPtr
 * @data : not used
 * @name : not used
 * 
 * Convert payload into format needed, and add to search dataBase 
 */
void
addSourceNode(void *payload, void *data ATTRIBUTE_UNUSED,
              xmlChar * name ATTRIBUTE_UNUSED)
{
    xmlNodePtr node = searchSourceNode((xsltStylesheetPtr) payload);

    searchAdd(node);
}


/**
 * addTemplateNode:
 * @payload : valid xsltTemplatePtr
 * @data : not used
 * @name : not used
 * 
 * Convert payload into format needed, and add to search dataBase 
 */
void
addTemplateNode(void *payload, void *data ATTRIBUTE_UNUSED,
                xmlChar * name ATTRIBUTE_UNUSED)
{
    xmlNodePtr node =
        searchTemplateNode(((xsltTemplatePtr) payload)->elem);
    searchAdd(node);
}


/**
 * addGlobalNode:
 * @payload : valid xmlNodePtr of global variable
 * @data : not used
 * @name : not used
 * 
 * Convert payload into format needed, and add to search dataBase 
 */
void
addGlobalNode(void *payload, void *data ATTRIBUTE_UNUSED,
              xmlChar * name ATTRIBUTE_UNUSED)
{
    xmlNodePtr node = searchGlobalNode((xmlNodePtr) payload);

    searchAdd(node);
}


/**
 * addLocalNode:
 * @payload : valid xmlNodePtr of local variable
 * @data : not used
 * @name : not used
 * 
 * Convert payload into format needed, and add to search dataBase 
 */
void
addLocalNode(void *payload, void *data ATTRIBUTE_UNUSED,
             xmlChar * name ATTRIBUTE_UNUSED)
{
    xmlNodePtr node = searchLocalNode((xmlNodePtr) payload);

    searchAdd(node);
}


/**
 * addIncludeNode:
 * @payload : valid xmlNodePtr of include instuction
 * @data : not used
 * @name : not used
 * 
 * Convert payload into format needed, and add to search dataBase 
 */
void
addIncludeNode(void *payload, void *data ATTRIBUTE_UNUSED,
               xmlChar * name ATTRIBUTE_UNUSED)
{
    xmlNodePtr node = searchIncludeNode((xmlNodePtr) payload);

    searchAdd(node);
}


/**
 * addCallStackItems:
 * 
 * Convert call stack items into format needed, and add to search dataBase 
 */
void
addCallStackItems(void)
{
    xslCallPointPtr item;
    xmlNodePtr node;
    int depth;

    for (depth = callDepth(); depth > 0; depth--) {
        item = getCall(depth);
        if (item) {
            node = searchCallStackNode(item);
            if (node)
                searchAdd(node);
        }
    }
}


/**
 * updateSearchData:
 * @styleCtxt: Not used
 * @style: Is valid
 * @data: Not used but MUST be NULL for the moment
 * @variableTypes: What types of variables to look
 *
 * Update the searchDatabase
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
updateSearchData(xsltTransformContextPtr styleCtxt ATTRIBUTE_UNUSED,
                 xsltStylesheetPtr style,
                 void *data, VariableTypeEnum variableTypes)
{
    int result = 0;

    searchEmpty();
    xsltGenericError(xsltGenericErrorContext,
                     "Updating search database, this may take a while ..\n");
    /* add items in the call stack to the search dataBase */
    addCallStackItems();
    xsltGenericError(xsltGenericErrorContext,
                     "  Looking for breakpoints \n");
    walkBreakPoints((xmlHashScanner) addBreakPointNode, data);

    xsltGenericError(xsltGenericErrorContext,
                     "  Looking for imports and top level stylesheets \n");
    walkStylesheets((xmlHashScanner) addSourceNode, data, style);
    xsltGenericError(xsltGenericErrorContext,
                     "  Looking for xsl:includes \n");
    walkIncludeInst((xmlHashScanner) addIncludeNode, data, style);
    xsltGenericError(xsltGenericErrorContext,
                     "  Looking for templates \n");
    walkTemplates((xmlHashScanner) addTemplateNode, data, style);
    xsltGenericError(xsltGenericErrorContext,
                     "  Looking for global variables \n");
    walkGlobals((xmlHashScanner) addGlobalNode, data, style);
    xsltGenericError(xsltGenericErrorContext,
                     "  Looking for local variables \n");
    walkLocals((xmlHashScanner) addLocalNode, data, style);
    xsltGenericError(xsltGenericErrorContext, "  Formatting output \n");
    searchSave((xmlChar *) "search.data");
    result++;
    return result;
}


/**
 * debugBreak:
 * @templ: The source node being executed
 * @node: The data node being processed
 * @root: The template being applied to "node"
 * @ctxt: The transform context for stylesheet being processed
 *
 * A break point has been found so pass control to user
 */
void
debugBreak(xmlNodePtr templ, xmlNodePtr node, xsltTemplatePtr root,
           xsltTransformContextPtr ctxt)
{
    xmlDocPtr tempDoc = NULL;
    xmlNodePtr tempNode = NULL;

    rootCopy = root;

    /* select normal input output streams */
    selectNormalIO();

    if (templ == NULL) {
        tempDoc = xmlNewDoc((xmlChar *) "1.0");
        if (!tempDoc)
            return;
        tempNode = xmlNewNode(NULL, (xmlChar *) "xsldbg_default_node");
        if (!tempNode) {
            xmlFreeDoc(tempDoc);
            return;
        }
        xmlAddChild((xmlNodePtr) tempDoc, tempNode);
        templ = tempNode;
    }
    if (node == NULL) {
        tempDoc = xmlNewDoc((xmlChar *) "1.0");
        if (!tempDoc)
            return;
        tempNode = xmlNewNode(NULL, (xmlChar *) "xsldbg_default_node");
        if (!tempNode) {
            xmlFreeDoc(tempDoc);
            return;
        }
        xmlAddChild((xmlNodePtr) tempDoc, tempNode);
        node = tempNode;
    }
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
        }
    }

    xslDbgShell(templ, node, (xmlChar *) "index.xsl",
                (xmlShellReadlineFunc) xslDbgShellReadline, stdout, ctxt);
    if (tempDoc)
        xmlFreeDoc(tempDoc);
}


/* Highly modified function based on xmlShell */

/**
 *  xslDbgShell:
 * @source: The current stylesheet instruction being executed
 * @doc: The current document node being processed
 * @filename: Not used
 * @input: The function to call to when reading commands from stdio
 * @output: Where to put the results
 * @styleCtxt: Is valid 
 *
 * Present to the user the xsldbg shell
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
    int cmdResult;              /* result of last command */
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
    debugGotControl(1);
    ctxt->loaded = 0;
    lastSourceNode = source;
    lastDocNode = doc;
    /* show the doc or source first? */
    if (showSource) {
        ctxt->doc = source->doc;
        ctxt->node = source;
#ifdef USE_XSLDBG_AS_THREAD
        xsldbgUpdateFileDetails(source);
#endif
    } else {
        ctxt->doc = doc->doc;
        ctxt->node = (xmlNodePtr) doc;
#ifdef USE_XSLDBG_AS_THREAD
        xsldbgUpdateFileDetails((xmlNodePtr) doc);
#endif
    }

    /* let any listener know that we got to a new line */
#ifdef USE_XSLDBG_AS_THREAD
    notifyXsldbgApp(XSLDBG_MSG_LINE_CHANGED, NULL);
#endif

    ctxt->input = input;
    ctxt->output = output;
    ctxt->filename = (char *) xmlStrdup((xmlChar *) filename);

    if (ctxt->node && ctxt->node && ctxt->node->doc
        && ctxt->node->doc->URL) {
        if (activeBreakPoint() != NULL) {
            xslBreakPointPtr breakPtr = activeBreakPoint();

            xsltGenericError(xsltGenericErrorContext,
                             "Breakpoint in file %s : line %ld \n",
                             breakPtr->url, breakPtr->lineNo);
        } else {
            if (xmlGetLineNo(ctxt->node) != -1)
                xsltGenericError(xsltGenericErrorContext,
                                 "Breakpoint at file %s : line %ld \n",
                                 ctxt->node->doc->URL,
                                 xmlGetLineNo(ctxt->node));
            else
                xsltGenericError(xsltGenericErrorContext,
                                 "Breakpoint @ text node in file %s\n",
                                 ctxt->node->doc->URL);
        }
    }

    if (xslDebugStatus == DEBUG_TRACE) {
        xmlFree(ctxt);
        return;
    }
    if (xslDebugStatus == DEBUG_WALK) {
        if (xslDbgWalkContinue()) {
            xmlFree(ctxt);
            return;
        }
    }

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
        cmdline = (xmlChar *) ctxt->input((char *) prompt);
        if (cmdline == NULL)
            break;

#ifdef USE_XSLDBG_AS_THREAD
        notifyXsldbgApp(XSLDBG_MSG_PROCESSING_INPUT, NULL);
#endif
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

        cmdResult = -1;         /* flag that it contains no result */
        /*
         * start interpreting the command
         */
        switch (commandId + DEBUG_HELP_CMD) {
                /* --- Help related commands --- */
            case DEBUG_HELP_CMD:
                cmdResult = helpTop(arg);
                break;


                /* --- Running related commands --- */
            case DEBUG_BYE_CMD:
            case DEBUG_EXIT_CMD:
            case DEBUG_QUIT_CMD:
                /* allow the stylesheet to exit */
                xslDebugStatus = DEBUG_QUIT;
                exitShell++;
                cmdResult = 1;
                break;

            case DEBUG_STEP_CMD:
                xslDebugStatus = DEBUG_STEP;
                exitShell++;
                cmdResult = 1;
                break;

            case DEBUG_STEPUP_CMD:
                {
                    xmlChar *noOfFrames = arg;

                    /* skip until next space character */
                    while (*noOfFrames && (*noOfFrames != ' ')) {
                        noOfFrames++;
                    }
                    cmdResult = xslDbgShellFrameBreak(noOfFrames, 1);
                    exitShell++;
                }
                break;

            case DEBUG_STEPDOWN_CMD:
                {
                    xmlChar *noOfFrames = arg;

                    /* skip until next space character */
                    while (*noOfFrames && (*noOfFrames != ' ')) {
                        noOfFrames++;
                    }
                    cmdResult = xslDbgShellFrameBreak(noOfFrames, 0);
                    exitShell++;
                }
                break;

                /* continue to next break point */
            case DEBUG_CONT_CMD:
                xslDebugStatus = DEBUG_CONT;
                exitShell++;
                cmdResult = 1;
                break;

                /* restart */
            case DEBUG_RUN_CMD:
                xslDebugStatus = DEBUG_RUN_RESTART;
                exitShell++;
                cmdResult = 1;
                break;


                /* --- Template related commands --- */
            case DEBUG_TEMPLATES_CMD:
                {
                    int allFiles = 1, verbose = 1;

                    if (xmlStrLen(arg) && (xmlStrCmp(arg, "this") == 0)) {
                        allFiles = 0;
                    }

                    /* be verbose when printing template names */
                    /* if args is not empty then print names this stylesheet */
                    cmdResult =
                        xslDbgPrintTemplateNames(styleCtxt, ctxt, arg,
                                                 verbose, allFiles);
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
                        cmdResult = 1;
                    } else {
                        xsltGenericError(xsltGenericErrorContext,
                                         " at file %s : line %ld \n",
                                         ctxt->node->doc->URL,
                                         xmlGetLineNo(ctxt->node));
                        cmdResult = 1;
                } else {
                    xsltGenericError(xsltGenericErrorContext, "\n");
                    cmdResult = 0;
                }
                break;

            case DEBUG_FRAME_CMD:
                cmdResult = xslDbgPrintCallStack(arg);
                break;

            case DEBUG_STYLESHEETS_CMD:
                cmdResult = xslDbgPrintStyleSheets(arg);
                break;

                /* --- Break point related commands --- */
            case DEBUG_BREAK_CMD:
                if (xmlStrLen(arg)) {
                    if (styleCtxt)
                        cmdResult =
                            xslDbgShellBreak(arg, styleCtxt->style,
                                             styleCtxt);
                    else
                        cmdResult = xslDbgShellBreak(arg, NULL, styleCtxt);
                } else {
                    /* select current node to break at */
                    xmlChar buff[100];

                    if (ctxt->node->doc && ctxt->node->doc->URL)
                        snprintf((char *) buff, 99, "-l %s %ld",
                                 ctxt->node->doc->URL,
                                 xmlGetLineNo(ctxt->node));
                    if (styleCtxt)
                        cmdResult =
                            xslDbgShellBreak(buff, styleCtxt->style,
                                             styleCtxt);
                    else
                        cmdResult =
                            xslDbgShellBreak(buff, NULL, styleCtxt);
                }

                break;

            case DEBUG_SHOWBREAK_CMD:
                xsltGenericError(xsltGenericErrorContext, "\n");
                printCount = 0; /* printCount will get updated by
                                 * xslDbgPrintBreakPoint */
#ifdef USE_XSLDBG_AS_THREAD
		/* notify the app of the start of breakpoint list */
		notifyXsldbgApp(XSLDBG_MSG_BREAKPOINT_CHANGED, NULL);
#endif
                walkBreakPoints((xmlHashScanner) xslDbgPrintBreakPoint,
                                NULL);
                if (printCount == 0)
                    xsltGenericError(xsltGenericErrorContext,
                                     "\nNo file break points set:\n");
                else
                    xsltGenericError(xsltGenericErrorContext,
                                     "\n\t Total of %d break points present\n",
                                     printCount);
                cmdResult = 1;
                break;

            case DEBUG_DELETE_CMD:
                if (xmlStrLen(arg))
                    cmdResult = xslDbgShellDelete((xmlChar *) arg);
                else {
                    xslBreakPointPtr breakPoint = NULL;

                    if (ctxt->node->doc && ctxt->node->doc->URL)
                        breakPoint = getBreakPoint(ctxt->node->doc->URL,
                                                   xmlGetLineNo(ctxt->
                                                                node));
                    if (!breakPoint || !deleteBreakPoint(breakPoint)) {
                        xsltGenericError(xsltGenericErrorContext,
                                         "Unable to add delete point");
                        cmdResult = 0;
                    }
                }
                break;

            case DEBUG_ENABLE_CMD:
                if (xmlStrLen(arg))
                    cmdResult =
                        xslDbgShellEnable(arg, XSL_TOGGLE_BREAKPOINT);
                else {
                    xslBreakPointPtr breakPoint = NULL;

                    if (ctxt->node->doc && ctxt->node->doc->URL)
                        breakPoint = getBreakPoint(ctxt->node->doc->URL,
                                                   xmlGetLineNo(ctxt->
                                                                node));
                    if (!breakPoint
                        ||
                        (!enableBreakPoint
                         (breakPoint, !breakPoint->enabled))) {
                        xsltGenericError(xsltGenericErrorContext,
                                         "Unable to add enable/disable point\n");
                        cmdResult = 0;
                    }
                }
                break;

            case DEBUG_DISABLE_CMD:
                if (xmlStrLen(arg))
                    cmdResult = xslDbgShellEnable(arg, 0);
                else {
                    xslBreakPointPtr breakPoint = NULL;

                    if (ctxt->node->doc && ctxt->node->doc->URL)
                        breakPoint = getBreakPoint(ctxt->node->doc->URL,
                                                   xmlGetLineNo(ctxt->
                                                                node));
                    if (!breakPoint
                        ||
                        (!enableBreakPoint
                         (breakPoint, !!breakPoint->enabled))) {
                        xsltGenericError(xsltGenericErrorContext,
                                         "Unable to add enable/disable point\n");
                        cmdResult = 0;
                    }
                }
                break;



                /* --- Node view related commands --- */
            case DEBUG_LS_CMD:
                cmdResult = xslDbgShellPrintList(ctxt, arg, 0);
                break;

            case DEBUG_DIR_CMD:
                cmdResult = xslDbgShellPrintList(ctxt, arg, 1);
                break;

            case DEBUG_DU_CMD:
                xmlShellDu(ctxt, NULL, ctxt->node, NULL);
                cmdResult = 1;
                break;

            case DEBUG_CAT_CMD:
            case DEBUG_PRINT_CMD:
                cmdResult = xslDbgShellCat(styleCtxt, ctxt, arg);
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
                cmdResult = 1;

                break;

            case DEBUG_DUMP_CMD:
                xmlDebugDumpDocument(stdout, ctxt->doc);
                cmdResult = 1;
                break;

            case DEBUG_BASE_CMD:
                xmlShellBase(ctxt, NULL, ctxt->node, NULL);
                cmdResult = 1;
                break;


                /* ---  Variable related commands --- */
            case DEBUG_GLOBALS_CMD:
                if (loadedFiles == 0)
                    cmdResult = xslDbgShellPrintVariable(styleCtxt, arg,
                                                         DEBUG_GLOBAL_VAR);
                else {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Need to use run command first\n");
                    cmdResult = 0;
                }
                break;

            case DEBUG_LOCALS_CMD:
                if (loadedFiles == 0)
                    cmdResult = xslDbgShellPrintVariable(styleCtxt, arg,
                                                         DEBUG_LOCAL_VAR);
                else {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Need to use run command first\n");
                    cmdResult = 0;
                }
                break;


                /* It's difficult to put the following commands into 
                 * a separe file so they stay here! */
                /* --- Node selection related commands --- */
            case DEBUG_SOURCE_CMD:
                cmdResult = 1;  /* only one case where this will command fail */
                if (xmlStrLen(arg) == 0) {
                    if (ctxt->doc == doc->doc)
                        lastDocNode = ctxt->node;
                    ctxt->doc = source->doc;
                    ctxt->node = lastSourceNode;
                    ctxt->pctxt = xmlXPathNewContext(ctxt->doc);
                    showSource = 1;
#ifdef USE_XSLDBG_AS_THREAD
		    xsldbgUpdateFileDetails((xmlNodePtr) ctxt->node);
		    /* let any listener know that we got to a new line */
		    notifyXsldbgApp(XSLDBG_MSG_LINE_CHANGED, NULL);
#endif
                    if (ctxt->pctxt == NULL) {
                        xmlFree(ctxt);
                        xslDebugStatus = DEBUG_QUIT;
                        return;
                    } else
                        break;
                } else {
                    /* load new stylesheet file, actual loading happens later */
                    xmlChar *buff = dir;        /* use dir command temp buffer */

#ifndef __riscos                /* RISC OS has no concept of 'home' directory */
                    /* replace ~ with home path */
                    if ((arg[0] == '~') && getenv("HOME")) {
                        xmlStrCpy(buff, getenv("HOME"));
                        if (xmlStrLen(buff) + xmlStrLen(arg) <
                            DEBUG_BUFFER_SIZE) {
                            xmlStrCat(buff, &arg[1]);
                            setStringOption(OPTIONS_SOURCE_FILE_NAME,
                                            buff);
                        } else {
                            xsltGenericError(xsltGenericErrorContext,
                                             "File name too large\n");
                            cmdResult = 0;
                            break;
                        }
                    } else
#endif
                    {
                        setStringOption(OPTIONS_SOURCE_FILE_NAME, arg);
                    }
                    xsltGenericError(xsltGenericErrorContext,
                                     "Load of source deferred use run command\n"
                                     "Removing all breakpoints\n");
                    loadedFiles = 1;
                    /* clear all break points , what else makes sense? */
                    emptyBreakPoint();
                }
                break;

            case DEBUG_DATA_CMD:
                cmdResult = 1;  /* only one case where this will command fail */
                if (xmlStrLen(arg) == 0) {
                    if (ctxt->doc == source->doc)
                        lastSourceNode = ctxt->node;
                    ctxt->doc = doc->doc;
                    ctxt->node = lastDocNode;
                    ctxt->pctxt = xmlXPathNewContext(ctxt->doc);
                    showSource = 0;
#ifdef USE_XSLDBG_AS_THREAD
		    xsldbgUpdateFileDetails((xmlNodePtr) ctxt->node);
		    /* let any listener know that we got to a new line */
		    notifyXsldbgApp(XSLDBG_MSG_LINE_CHANGED, NULL);
#endif
                    if (ctxt->pctxt == NULL) {
                        xmlFree(ctxt);
                        xslDebugStatus = DEBUG_QUIT;
                        return;
                    } else
                        break;
                } else {
                    /* load new xml file actual loading hapens later */
                    xmlChar *buff = dir;        /* use dir command temp buffer */

#ifndef __riscos                /* RISC OS has no concept of 'home' directory */
                    /* replace ~ with home path */
                    if ((arg[0] == '~') && getenv("HOME")) {
                        xmlStrCpy(buff, getenv("HOME"));
                        if (xmlStrLen(buff) + xmlStrLen(arg) <
                            DEBUG_BUFFER_SIZE) {
                            xmlStrCat(buff, &arg[1]);
                            setStringOption(OPTIONS_DATA_FILE_NAME, buff);
                        } else {
                            xsltGenericError(xsltGenericErrorContext,
                                             "File name too large\n");
                            cmdResult = 0;
                            break;
                        }
                    } else
#endif
                    {
                        setStringOption(OPTIONS_DATA_FILE_NAME, arg);
                    }

                    loadedFiles = 1;
                    xsltGenericError(xsltGenericErrorContext,
                                     "Load of xml data deferred use run command\n"
                                     "Removing all breakpoints\n");
                    /* clear all break points , what else makes sense? */
                    emptyBreakPoint();
                }
                break;

	case DEBUG_OUTPUT_CMD:
	  if (xmlStrLen(arg) > 0){
	    if (xmlStrCmp(arg, "-") != 0)
	      setStringOption(OPTIONS_OUTPUT_FILE_NAME, arg);
	    else
	      setStringOption(OPTIONS_OUTPUT_FILE_NAME, NULL);
	    cmdResult = 1;
	  }else{
            xsltGenericError(xsltGenericErrorContext,
			     "Missing file name\n");
	    cmdResult = 0;
	  }
	  break;

            case DEBUG_CD_CMD:
                /* use dir as a working buffer */
                xmlStrnCpy(dir, arg, 2);
                dir[2] = '\0';
                shortCutId = lookupName(dir, (xmlChar **) cdShortCuts);
                if (shortCutId >= 0) {
                    if (xmlStrLen(arg) == 2) {
                        cmdResult = xslDbgCd(styleCtxt, ctxt,
                                             (xmlChar *)
                                             cdAlternative[shortCutId],
                                             source);
                    } else {

                        xmlStrCpy(dir, cdAlternative[shortCutId]);
                        xmlStrCat(dir, &arg[2]);
                        cmdResult = xslDbgCd(styleCtxt, ctxt, dir, source);
                    }

                } else
                    cmdResult = xslDbgCd(styleCtxt, ctxt, arg, source);
                break;


                /* --- File related commands --- */
            case DEBUG_VALIDATE_CMD:
                xsltGenericError(xsltGenericErrorContext,
                                 "validate disabled\n");
                cmdResult = 0;
                /*
                 * xmlShellValidate(ctxt, arg, NULL, NULL);
                 */
                break;

            case DEBUG_LOAD_CMD:
                xsltGenericError(xsltGenericErrorContext,
                                 "loading disabled\n");
                cmdResult = 0;
                /*
                 * xmlShellLoad(ctxt, arg, NULL, NULL);
                 */
                break;

            case DEBUG_SAVE_CMD:
                xsltGenericError(xsltGenericErrorContext,
                                 "saving disabled\n");
                cmdResult = 0;
                /*
                 * xmlShellSave(ctxt, arg, NULL, NULL);
                 */
                break;

            case DEBUG_WRITE_CMD:
                xsltGenericError(xsltGenericErrorContext,
                                 "write disabled\n");
                cmdResult = 0;
                /*
                 * xmlShellWrite(ctxt, arg, NULL, NULL);
                 */
                break;

            case DEBUG_FREE_CMD:
                xsltGenericError(xsltGenericErrorContext,
                                 "free disabled\n");
                cmdResult = 0;
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
                cmdResult = xslDbgShellChangeWd(arg);
                break;

            case DEBUG_SHELL_EXEC_CMD:
                cmdResult = xslDbgShellExecute(arg, 1);
                break;


                /* libxslt parameter related */
            case DEBUG_ADDPARAM_CMD:
                cmdResult = xslDbgShellAddParam(arg);
                break;

            case DEBUG_DELPARAM_CMD:
                cmdResult = xslDbgShellDelParam(arg);
                break;

            case DEBUG_SHOWPARAM_CMD:
                cmdResult = xslDbgShellShowParam(arg);
                break;

            case DEBUG_TTY_CMD:
                if (openTerminal(arg)) {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Opening terminal %s\n", arg);
                    cmdResult = 1;
                    selectTerminalIO();
                } else
                    cmdResult = 0;
                break;



                /* tracing related commands */
            case DEBUG_TRACE_CMD:
                if (xslDbgShellTrace(arg)) {
                    exitShell++;
                    cmdResult = 1;
                } else
                    cmdResult = 0;

                break;

            case DEBUG_WALK_CMD:
                if (xslDbgShellWalk(arg)) {
                    exitShell++;
                    cmdResult = 1;
                } else
                    cmdResult = 0;
                break;

                /* search related commands */
            case DEBUG_SEARCH_CMD:
                cmdResult =
                    xslDbgShellSearch(styleCtxt, getStylesheet(), arg);
		break;



            case DEBUG_SET_CMD:
                cmdResult =  xslDbgShellSetVariable(styleCtxt, arg);
                break;

            default:
                xmlGenericError(xmlGenericErrorContext,
                                "Unknown command %s, try help\n", command);
                cmdResult = 0;
        }

        /* kdbgs like to get the marker after every command so here it is */
        if (isOptionEnabled(OPTIONS_GDB)) {
            if (ctxt->node && ctxt->node && ctxt->node->doc
                && ctxt->node->doc->URL) {

                if (activeBreakPoint() != NULL) {
                    xslBreakPointPtr breakPtr = activeBreakPoint();

                    xsltGenericError(xsltGenericErrorContext,
                                     "Breakpoint in file %s : line %ld \n",
                                     breakPtr->url, breakPtr->lineNo);
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

        /* notify any listeners of that the command failed */
        if (cmdResult == 0) {
	  xmlStrnCpy(messageBuffer, cmdline, sizeof(messageBuffer));
	  notifyStateXsldbgApp(XSLDBG_MSG_PROCESSING_RESULT,
                   commandId + DEBUG_HELP_CMD, XSLDBG_COMMAND_FAILED, messageBuffer);

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

    /* send everything to the terminal if opened */
    selectTerminalIO();
}
