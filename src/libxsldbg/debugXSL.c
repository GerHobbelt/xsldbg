
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
 * New file : shell.c : Debug support version
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
#include "debug.h"
#include "debugXSL.h"
#include "options.h"
#include "breakpoint.h"
#include "help.h"
#include <stdlib.h>
#include <libxslt/transform.h>  /* needed by source command */
#include <libxslt/xsltInternals.h>
#include <libxml/debugXML.h>
#include <stdio.h>

#include "xsldbgmsg.h"
#include "xsldbgthread.h"       /* for get thread status */
#include "xsldbgio.h"

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
    "output",                   /* output file name */
    "cd",

    /* file related */
    /* "output", already listed */
    "entities",
    "system",
    "public",
    "encoding",
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
    "setoption",
    "options",

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
    "o",                        /* output file name */
    "cd",

    /* file related */
    /* "output", already listed */
    "ent",                      /* entities command */
    "sys",                      /* sytem command */
    "pub",                      /* public command */
    "encoding",
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
    "setoption",

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
    "descendant::node()",
    NULL                        /* indicate end of list */
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
 * @arg : path to change to
 * @source : is valid
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
 * Can only be called from within shellPrompt!
 * OPTION_WALK_SPEED != WALKSPEED_STOP
 *
 * Returns 1 if walk is to continue,
 *         0 otherwise
 */
int xslDbgWalkContinue(void);



/**
 * addBreakPointNode:
 * @payload : valid breakPointPtr
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
 *  shellPrompt:
 * @source: The current stylesheet instruction being executed
 * @doc: The current document node being processed
 * @filename: Not used
 * @input: The function to call to when reading commands from stdio
 * @output: Where to put the results
 * @styleCtxt: Is valid 
 *
 * Present to the user the xsldbg shell
 */
void shellPrompt(xmlNodePtr source, xmlNodePtr doc,
                 xmlChar * filename,
                 xmlShellReadlineFunc input,
                 FILE * output, xsltTransformContextPtr styleCtxt);

/* ------------------------------------- 
    End private functions
---------------------------------------*/



/**
 * debugXSLGetTemplate:
 * 
 * Return the last template node found, if an
 *
 * Returns The last template node found, if any
 */
xsltTemplatePtr
debugXSLGetTemplate(void)
{
    return rootCopy;
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
 * @arg : path to change to and in UTF-8
 * @source : is valid 
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
                         "Error: Debugger has no files loaded, try reloading files\n");
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
                                         "Error: Template '%s' not found\n",
                                         &arg[offset]);
                        return result;
                    } else {
                        xsltGenericError(xsltGenericErrorContext,
                                         " template :\"%s\"\n",
                                         &arg[offset]);
                        ctxt->node = templateNode;
                        result = 1;
                        return result;
                    }
                } else if (arg[1] == 's') {
                    /*quickly switch to another stylesheet node */
                    xmlXPathContextPtr pctxt;

                    if (source) {
                        pctxt = xmlXPathNewContext(source->doc);
                        if (pctxt == NULL) {
                            xmlFree(ctxt);
                            /* xslDebugStatus = DEBUG_QUIT; */
                            return result;
                        }
                        if (!xmlXPathNsLookup(pctxt, (xmlChar *) "xsl"))
                            xmlXPathRegisterNs(pctxt, (xmlChar *) "xsl",
                                               XSLT_NAMESPACE);
                        list =
                            xmlXPathEval((xmlChar *) & arg[offset], pctxt);
                        if (pctxt) {
                            xmlFree(pctxt);
                        }
                    } else {
                        xsltGenericError(xsltGenericErrorContext,
                                         "Error: Unable to cd, no stylesheet loaded\n");
                    }
                } else {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Error: Unknown option to cd\n");
                }
            } else
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: Unable to cd, no stylesheet loaded\n");
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
                                 "Error: Invalid parameters to xslDbgCd\n");
            }
        }

        if (list != NULL) {
            switch (list->type) {
                case XPATH_NODESET:
                    if (list->nodesetval) {
                        if (list->nodesetval->nodeNr == 1) {
                            ctxt->node = list->nodesetval->nodeTab[0];
                            /* tell the application about the new line
                             * number we are looking at */
                            if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
                                int breakpoint = 0;

                                xsldbgUpdateFileDetails(ctxt->node);
                                notifyXsldbgApp(XSLDBG_MSG_LINE_CHANGED,
                                                &breakpoint);
                            }
                            result = 1;
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
                            "Error: %s no such node\n", arg);
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
    callPointPtr callPointItem;

    if (arg == NULL) {
        if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
            notifyListStart(XSLDBG_MSG_CALLSTACK_CHANGED);
            /* we send the oldest frame stack first */
            for (depth = 1; depth <= callStackGetDepth(); depth++) {
                callPointItem = callStackGet(depth);
                if (callPointItem && callPointItem->info) {
                    notifyListQueue(callPointItem);
                }
            }
            notifyListSend();
        } else {

            for (depth = callStackGetDepth(); depth >= 1; depth--) {
                callPointItem = callStackGet(depth);
                if (callPointItem && callPointItem->info) {
                    if (depth == callStackGetDepth()) {
                        xmlChar *curUrl = xsldbgUrl();
                        long curLine = xsldbgLineNo();

                        /* if possible list the current location */
                        if (rootCopy && (rootCopy->match || rootCopy->name)
                            && curUrl) {

                            if (rootCopy->match)
                                xsltGenericError(xsltGenericErrorContext,
                                                 "#%d template :\"%s\"",
                                                 depth, rootCopy->match);
                            else
                                xsltGenericError(xsltGenericErrorContext,
                                                 "#%d template :\"%s\"",
                                                 depth, rootCopy->name);
                            xsltGenericError(xsltGenericErrorContext,
                                             " in file %s : line %ld\n",
                                             curUrl, curLine);
                        } else if (curUrl) {
                            xsltGenericError(xsltGenericErrorContext,
                                             "#%d template :\"LIBXSLT_DEFAULT\"",
                                             depth);
                            xsltGenericError(xsltGenericErrorContext,
                                             " in file %s : line %ld\n",
                                             curUrl, curLine);
                        }

                        if (curUrl)
                            xmlFree(curUrl);

                    }
                    xsltGenericError(xsltGenericErrorContext,
                                     "#%d template :\"%s\"", depth - 1,
                                     callPointItem->info->templateName);
                    if (callPointItem->info->url)
                        xsltGenericError(xsltGenericErrorContext,
                                         " in file %s : line %ld\n",
                                         callPointItem->info->url,
                                         callPointItem->lineNo);
                    else
                        xsltGenericError(xsltGenericErrorContext, "\n");
                } else {
#ifdef WITH_XSLT_DEBUG_PROCESS
                    xsltGenericError(xsltGenericErrorContext,
                                     "Error: Call stack item not found at depth %d :"
                                     " xslDbgPrintCallStack\n", depth);
#endif
                    break;
                }
            }
            if (callStackGetDepth() == 0)
                xsltGenericError(xsltGenericErrorContext,
                                 "No items on call stack\n");
            else
                xsltGenericError(xsltGenericErrorContext, "\n");
        }
    } else {
        long templateDepth = atol((char *) arg);


        if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
            /* should never happen but just in case, when running as a 
             * thread always provide NO params to the where command */
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Notification of a frame not supported\n");
            return result;
        }

        if (templateDepth >= 0) {
            callPointItem = callStackGet(templateDepth + 1);
            if (callPointItem && callPointItem->info) {
                xsltGenericError(xsltGenericErrorContext,
                                 "#%d template :\"%s\"", templateDepth,
                                 callPointItem->info->templateName);
                /* should alays be present but .. */
                if (callPointItem->info->url)
                    xsltGenericError(xsltGenericErrorContext,
                                     " in file %s : line %ld \n",
                                     callPointItem->info->url,
                                     callPointItem->lineNo);
                else
                    xsltGenericError(xsltGenericErrorContext, "\n");
            } else {
#ifdef WITH_XSLT_DEBUG_PROCESS
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: Call stack item not found at templateDepth %d :"
                                 " xslDbgPrintCallStack\n", depth);
#endif
            }
        }
    }
    result = 1;
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
 * Can only be called from within shellPrompt!
 * OPTION_WALK_SPEED != WALKSPEED_STOP
 *
 * Returns 1 if walk is to continue,
 *         0 otherwise
 */
int
xslDbgWalkContinue(void)
{
    int result = 0, speed = optionsGetIntOption(OPTIONS_WALK_SPEED);

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
            result = 1;
            break;

        default:               /* stop walking */
            optionsSetIntOption(OPTIONS_WALK_SPEED, WALKSPEED_STOP);
            xslDebugStatus = DEBUG_STOP;
            break;
    }

    return result;
}


/**
 * addBreakPointNode:
 * @payload : valid breakPointPtr
 * @data : not used
 * @name : not used
 * 
 * Convert payload into format needed, and add to search dataBase 
 */
void
addBreakPointNode(void *payload, void *data ATTRIBUTE_UNUSED,
                  xmlChar * name ATTRIBUTE_UNUSED)
{
    xmlNodePtr node = searchBreakPointNode((breakPointPtr) payload);

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
    callPointPtr item;
    xmlNodePtr node;
    int depth;

    for (depth = callStackGetDepth(); depth > 0; depth--) {
        item = callStackGet(depth);
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

    if (!style) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Unable to update search database no stylesheet loaded\n");
        return result;
    }
    searchEmpty();
    xsltGenericError(xsltGenericErrorContext,
                     "Information: Updating search database, this may take a while ..\n");
    /* add items to the search dataBase */
    addCallStackItems();
    xsltGenericError(xsltGenericErrorContext,
                     "Information: Looking for breakpoints \n");
    walkBreakPoints((xmlHashScanner) addBreakPointNode, data);
    xsltGenericError(xsltGenericErrorContext,
                     "Information: Looking for imports and top level stylesheets \n");
    walkStylesheets((xmlHashScanner) addSourceNode, data, style);
    xsltGenericError(xsltGenericErrorContext,
                     "Information: Looking for xsl:includes \n");
    walkIncludeInst((xmlHashScanner) addIncludeNode, data, style);
    xsltGenericError(xsltGenericErrorContext,
                     "Information: Looking for templates \n");
    walkTemplates((xmlHashScanner) addTemplateNode, data, style);
    xsltGenericError(xsltGenericErrorContext,
                     "Information: Looking for global variables \n");
    walkGlobals((xmlHashScanner) addGlobalNode, data, style);
    xsltGenericError(xsltGenericErrorContext,
                     "Information: Looking for local variables \n");
    walkLocals((xmlHashScanner) addLocalNode, data, style);
    xsltGenericError(xsltGenericErrorContext,
                     "Information: Formatting output \n");

    searchSave(NULL);
    result = 1;
    return result;
}


/**
 * debugXSLBreak:
 * @templ: The source node being executed
 * @node: The data node being processed
 * @root: The template being applied to "node"
 * @ctxt: The transform context for stylesheet being processed
 *
 * A break point has been found so pass control to user
 */
void
debugXSLBreak(xmlNodePtr templ, xmlNodePtr node, xsltTemplatePtr root,
              xsltTransformContextPtr ctxt)
{
    xmlDocPtr tempDoc = NULL;
    xmlNodePtr tempNode = NULL;

    rootCopy = root;

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

    if (node == NULL)
        node = (xmlNodePtr) filesGetMainDoc();

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
        if (terminalIO == NULL) {
            if (root->match)
                xsltGenericError(xsltGenericErrorContext,
                                 "\nReached template :\"%s\"\n",
                                 root->match);
            else
                xsltGenericError(xsltGenericErrorContext,
                                 "\nReached template :\"%s\"\n",
                                 root->name);
        } else {
            if ((xslDebugStatus == DEBUG_TRACE) ||
                (xslDebugStatus == DEBUG_WALK)) {
                if (root->match)
                    fprintf(terminalIO,
                            "\nReached template :\"%s\"\n", root->match);
                else
                    fprintf(terminalIO,
                            "\nReached template :\"%s\"\n", root->name);
            }
        }
    }

    shellPrompt(templ, node, (xmlChar *) "index.xsl",
                (xmlShellReadlineFunc) xslDbgShellReadline, stdout, ctxt);
    if (tempDoc)
        xmlFreeDoc(tempDoc);
}


/* Highly modified function based on xmlShell */

/**
 *  shellPrompt:
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
shellPrompt(xmlNodePtr source, xmlNodePtr doc, xmlChar * filename,
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
    xmlChar *baseUri = NULL;    /* for used for included xml entities */
    const xmlChar *breakUri;

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
        xsldbgUpdateFileDetails(source);
    } else {
        ctxt->doc = doc->doc;
        ctxt->node = (xmlNodePtr) doc;
        xsldbgUpdateFileDetails((xmlNodePtr) doc);
    }

    ctxt->input = input;
    ctxt->output = output;
    ctxt->filename = (char *) xmlStrdup((xmlChar *) filename);

    /* let any listener know that we got to a new line */
    if (xslDebugStatus != DEBUG_TRACE) {
        /* don't send notify message if we are tracing stylesheet */
        int breakpoint = 1;

        notifyXsldbgApp(XSLDBG_MSG_LINE_CHANGED, &breakpoint);
    }


    /* If using a thread and the thread is running then we don't need to 
     * send this as the application will see the XSLDBG_MSG_LINE_CHANGED message */
    if ((getThreadStatus() == XSLDBG_MSG_THREAD_NOTUSED) ||
        (xslDebugStatus == DEBUG_TRACE)) {
        if (ctxt->node && ctxt->node && ctxt->node->doc
            && ctxt->node->doc->URL) {
            xmlStrCpy(messageBuffer, "");
            if (!showSource) {
                baseUri = filesGetBaseUri(ctxt->node);
                if (baseUri != NULL)
                    breakUri = baseUri;
                else
                    breakUri = ctxt->node->doc->URL;
            } else
                breakUri = ctxt->node->doc->URL;

            if (xmlGetLineNo(ctxt->node) != -1)
                snprintf((char *) messageBuffer, sizeof(messageBuffer),
                         "Breakpoint at file %s : line %ld \n",
                         breakUri, xmlGetLineNo(ctxt->node));
            else
                snprintf((char *) messageBuffer, sizeof(messageBuffer),
                         "Breakpoint @ text node in file %s\n", breakUri);
            if (baseUri != NULL) {
                xmlFree(baseUri);
                baseUri = NULL;
            }

            if (((xslDebugStatus == DEBUG_TRACE) ||
                 (xslDebugStatus == DEBUG_WALK)) && (terminalIO != NULL))
                fprintf(terminalIO, "%s", messageBuffer);
            else
                xsltGenericError(xsltGenericErrorContext,
                                 "%s", messageBuffer);

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

    while (!exitShell && (xslDebugStatus != DEBUG_QUIT)) {
        if (getThreadStatus() != XSLDBG_MSG_THREAD_RUN) {
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
            if (cmdline && (optionsGetIntOption(OPTIONS_UTF8_INPUT) == 0)) {
                /* we are getting encoded characters from the command line
                 * so decode them into UTF-8 */
                xmlChar *tempResult = filesDecode(cmdline);

                if (tempResult) {
                    xmlFree(cmdline);
                    cmdline = tempResult;
                }
            }
        } else {
            /* don't need a prompt for running as when running as a thread */
            xmlStrCpy(messageBuffer, "");
            cmdline = (xmlChar *) ctxt->input((char *) messageBuffer);
        }

        if (cmdline == NULL)
            break;

        notifyXsldbgApp(XSLDBG_MSG_PROCESSING_INPUT, NULL);

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
                        xslDbgShellPrintTemplateNames(styleCtxt, ctxt, arg,
                                                      verbose, allFiles);
                    break;
                }


            case DEBUG_WHERE_CMD:
                /* Print the current working directory as well */
                xslDbgPrintCallStack(NULL);
                if (getThreadStatus() != XSLDBG_MSG_THREAD_RUN) {
                    if (!xmlShellPwd(ctxt, (char *) dir, ctxt->node, NULL))
                        xsltGenericError(xsltGenericErrorContext, "%s\n",
                                         dir);
                }
                cmdResult = 1;

                break;

            case DEBUG_FRAME_CMD:
                cmdResult = xslDbgPrintCallStack(arg);
                break;

            case DEBUG_STYLESHEETS_CMD:
                cmdResult = xslDbgShellPrintStyleSheets(arg);
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
                                 filesGetBaseUri(ctxt->node),
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
                if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
                    notifyListStart(XSLDBG_MSG_BREAKPOINT_CHANGED);
                    walkBreakPoints((xmlHashScanner)
                                    xslDbgShellPrintBreakPoint, NULL);
                    notifyListSend();
                } else {
                    xsltGenericError(xsltGenericErrorContext, "\n");
                    printCount = 0;     /* printCount will get updated by
                                         * xslDbgShellPrintBreakPoint */

                    walkBreakPoints((xmlHashScanner)
                                    xslDbgShellPrintBreakPoint, NULL);
                    if (printCount == 0)
                        xsltGenericError(xsltGenericErrorContext,
                                         "\nNo file breakpoints set\n");
                    else
                        xsltGenericError(xsltGenericErrorContext,
                                         "\n\t Total of %d breakpoints present\n",
                                         printCount);
                }
                cmdResult = 1;
                break;

            case DEBUG_DELETE_CMD:
                if (xmlStrLen(arg))
                    cmdResult = xslDbgShellDelete((xmlChar *) arg);
                else {
                    breakPointPtr breakPtr = NULL;

                    if (ctxt->node->doc && ctxt->node->doc->URL)
                        breakPtr =
                            breakPointGet(filesGetBaseUri(ctxt->node),
                                          xmlGetLineNo(ctxt->node));
                    if (!breakPtr || !breakPointDelete(breakPtr)) {
                        xsltGenericError(xsltGenericErrorContext,
                                         "Error: Unable to delete point\n");
                        cmdResult = 0;
                    }
                }
                break;

            case DEBUG_ENABLE_CMD:
                if (xmlStrLen(arg))
                    cmdResult =
                        xslDbgShellEnable(arg, XSL_TOGGLE_BREAKPOINT);
                else {
                    breakPointPtr breakPtr = NULL;

                    if (ctxt->node->doc && ctxt->node->doc->URL)
                        breakPtr =
                            breakPointGet(filesGetBaseUri(ctxt->node),
                                          xmlGetLineNo(ctxt->node));
                    if (!breakPtr ||
                        (!breakPointEnable(breakPtr, !breakPtr->enabled)))
                    {
                        xsltGenericError(xsltGenericErrorContext,
                                         "Error: Unable to enable/disable point\n");
                        cmdResult = 0;
                    }
                }
                break;

            case DEBUG_DISABLE_CMD:
                if (xmlStrLen(arg))
                    cmdResult = xslDbgShellEnable(arg, 0);
                else {
                    breakPointPtr breakPtr = NULL;

                    if (ctxt->node->doc && ctxt->node->doc->URL)
                        breakPtr =
                            breakPointGet(filesGetBaseUri(ctxt->node),
                                          xmlGetLineNo(ctxt->node));
                    if (!breakPtr || !breakPointEnable(breakPtr, 0)) {
                        xsltGenericError(xsltGenericErrorContext,
                                         "Error: Unable to disable point\n");
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
                    if (ctxt->node && filesGetBaseUri(ctxt->node))
                        xsltGenericError(xsltGenericErrorContext,
                                         " in file %s : line %ld",
                                         filesGetBaseUri(ctxt->node),
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
                                     "Error: Need to use run command first\n");
                    cmdResult = 0;
                }
                break;

            case DEBUG_LOCALS_CMD:
                if (loadedFiles == 0)
                    /* if gdb compatability mode is enable print the globals then 
                     * the locals */
                    if (optionsGetIntOption(OPTIONS_GDB) == 1) {
                        cmdResult =
                            xslDbgShellPrintVariable(styleCtxt, arg,
                                                     DEBUG_GLOBAL_VAR);
                        if (cmdResult == 1)
                            cmdResult =
                                xslDbgShellPrintVariable(styleCtxt, arg,
                                                         DEBUG_LOCAL_VAR);
                    } else
                        cmdResult =
                            xslDbgShellPrintVariable(styleCtxt, arg,
                                                     DEBUG_LOCAL_VAR);
                else {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Error: Need to use run command first\n");
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
                    xsldbgUpdateFileDetails((xmlNodePtr) ctxt->node);
                    /* let any listener know that we got to a new line */
                    notifyXsldbgApp(XSLDBG_MSG_LINE_CHANGED, NULL);
                    if (ctxt->pctxt == NULL) {
                        xmlFree(ctxt);
                        xslDebugStatus = DEBUG_QUIT;
                        return;
                    } else
                        break;
                } else {
                    /* load new stylesheet file, actual loading happens later */
                    xmlChar *expandedName = filesExpandName(arg);

                    if (expandedName) {
                        xsltGenericError(xsltGenericErrorContext,
                                         "Load of source deferred use run command\n"
                                         "Removing all breakpoints\n");

                        optionsSetStringOption(OPTIONS_SOURCE_FILE_NAME,
                                               expandedName);
                        loadedFiles = 1;
                        /* clear all break points , what else makes sense? */
                        breakPointEmpty();
                        xmlFree(expandedName);
                        cmdResult = 1;
                    } else {
                        cmdResult = 0;
                    }
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
                    xsldbgUpdateFileDetails((xmlNodePtr) ctxt->node);
                    /* let any listener know that we got to a new line */
                    notifyXsldbgApp(XSLDBG_MSG_LINE_CHANGED, NULL);
                    if (ctxt->pctxt == NULL) {
                        xmlFree(ctxt);
                        xslDebugStatus = DEBUG_QUIT;
                        return;
                    } else
                        break;
                } else {
                    /* load new xml file actual loading hapens later */
                    xmlChar *expandedName = filesExpandName(arg);

                    if (expandedName) {
                        xsltGenericError(xsltGenericErrorContext,
                                         "Load of data file deferred use run command\n"
                                         "Removing all breakpoints\n");

                        optionsSetStringOption(OPTIONS_DATA_FILE_NAME,
                                               expandedName);
                        loadedFiles = 1;
                        /* clear all break points , what else makes sense? */
                        breakPointEmpty();
                        xmlFree(expandedName);
                        cmdResult = 1;
                    } else {
                        cmdResult = 0;
                    }
                }
                break;

            case DEBUG_OUTPUT_CMD:
                if (xmlStrLen(arg) > 0) {
                    if (xmlStrCmp(arg, "-") != 0) {
                        xmlChar *expandedName = filesExpandName(arg);

                        if (expandedName) {
                            optionsSetStringOption
                                (OPTIONS_OUTPUT_FILE_NAME, expandedName);
                            xmlFree(expandedName);
                        } else {
                            cmdResult = 0;
                        }
                    } else
                        optionsSetStringOption(OPTIONS_OUTPUT_FILE_NAME,
                                               NULL);
                    cmdResult = 1;
                } else {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Error: Missing file name\n");
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
                        cmdResult = xslDbgCd(styleCtxt, ctxt, (xmlChar *)
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
            case DEBUG_ENTITIES_CMD:
                cmdResult = xslDbgEntities();
                break;

            case DEBUG_SYSTEM_CMD:
                cmdResult = xslDbgSystem(arg);
                break;

            case DEBUG_PUBLIC_CMD:
                cmdResult = xslDbgPublic(arg);
                break;

            case DEBUG_ENCODING_CMD:
                cmdResult = xslDbgEncoding(arg);
                break;

            case DEBUG_VALIDATE_CMD:
                xsltGenericError(xsltGenericErrorContext,
                                 "validate disabled\n");
                cmdResult = 0;
                /*
                 * xmlShellValidate(ctxt, arg, NULL, NULL);
                 */
                break;

            case DEBUG_LOAD_CMD:
                cmdResult = optionsLoad();
                /* restart xsldbg and activate new configuration */
                if (cmdResult == 1) {
                    xslDebugStatus = DEBUG_RUN_RESTART;
                    exitShell++;
                }
                break;

            case DEBUG_SAVE_CMD:
                cmdResult = optionsSave();
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


                /* libxslt parameter related commands */
            case DEBUG_ADDPARAM_CMD:
                cmdResult = xslDbgShellAddParam(arg);
                break;

            case DEBUG_DELPARAM_CMD:
                cmdResult = xslDbgShellDelParam(arg);
                break;

            case DEBUG_SHOWPARAM_CMD:
                cmdResult = xslDbgShellShowParam(arg);
                break;



                /* option related commmands */
            case DEBUG_SETOPTION_CMD:
                cmdResult = xslDbgShellSetOption(arg);
                break;

            case DEBUG_OPTIONS_CMD:
                cmdResult = xslDbgShellOptions();
                break;



                /* misc commands */
            case DEBUG_TTY_CMD:
                if (openTerminal(arg)) {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Opening terminal %s\n", arg);
                    cmdResult = 1;
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
                    xslDbgShellSearch(styleCtxt, filesGetStylesheet(),
                                      arg);
                break;



            case DEBUG_SET_CMD:
                cmdResult = xslDbgShellSetVariable(styleCtxt, arg);
                break;

            default:
                xmlGenericError(xmlGenericErrorContext,
                                "Error: Unknown command %s, try help\n",
                                command);
                cmdResult = 0;
        }

        /* kdbgs like to get the marker after every command so here it is */
        if (optionsGetIntOption(OPTIONS_GDB)) {
            if (ctxt->node && ctxt->node && ctxt->node->doc
                && ctxt->node->doc->URL) {

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

        /* notify any listeners of that the command failed */
        if (cmdResult == 0) {
            snprintf((char *) messageBuffer, sizeof(messageBuffer),
                     "\nRequest to xsldbg failed:%s\n", cmdline);
            notifyTextXsldbgApp(XSLDBG_MSG_TEXTOUT,
                                (char *) messageBuffer);
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
}
