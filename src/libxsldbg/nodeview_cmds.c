
/***************************************************************************
                          nodeview_cmds.c  - node viewing commands for xsldbg
                             -------------------
    begin                : Wed Nov 21 2001
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

#ifdef VERSION
#undef VERSION
#endif

#include <libxml/xpathInternals.h>
#include <libxml/HTMLparser.h>
#include <libxml/HTMLtree.h>
#include "xsldbg.h"
#include "debugXSL.h"
#include "arraylist.h"
#include "breakpoint.h"
#include "xsldbgmsg.h"
#include "xsldbgthread.h"       /* for getThreadStatus */
#include "files.h"
#include "options.h"


/* -----------------------------------------
   Private function declarations for nodeview_cmds.c
 -------------------------------------------*/
static xmlChar buffer[500];

/*
 * xslDbgShellPrintNames:
 * Print a name of variable found by scanning variable table
 * It is used by print_variable function.
 * @payload : not used
 * @data : not used
 * @name : the variable name 
 */
void *xslDbgShellPrintNames(void *payload ATTRIBUTE_UNUSED,
                            void *data ATTRIBUTE_UNUSED, xmlChar * name);

/**
 * xslShellCat:
 * @node : Is valid
 * @file : Is valid
 *
 * Send the results of cat command in @node to @file
 */
void xslShellCat(xmlNodePtr node, FILE * file);

/* ------------------------------------- 
    End private functions
---------------------------------------*/


/**
 * xslDbgShellPrintList: 
 * @ctxt: The current shell context
 * @arg: What xpath to display and in UTF-8
 * @dir: If 1 print in dir mode?, 
 *        otherwise ls mode
 *
 * Print list of nodes in either ls or dir format
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
xslDbgShellPrintList(xmlShellCtxtPtr ctxt, xmlChar * arg, int dir)
{
    xmlXPathObjectPtr list;
    int result = 0;

    if (!ctxt || !arg) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL arguments provided\n");
        return result;
    }

    if (arg[0] == 0) {
        if (dir)
            xmlShellDir(ctxt, NULL, ctxt->node, NULL);
        else
            xmlShellList(ctxt, NULL, ctxt->node, NULL);
        result = 1;             /*assume that this worked */
    } else {
        ctxt->pctxt->node = ctxt->node;
        ctxt->pctxt->node = ctxt->node;
        if (!xmlXPathNsLookup(ctxt->pctxt, (xmlChar *) "xsl"))
            xmlXPathRegisterNs(ctxt->pctxt, (xmlChar *) "xsl",
                               XSLT_NAMESPACE);
        list = xmlXPathEval(arg, ctxt->pctxt);
        if (list != NULL) {
            switch (list->type) {
                case XPATH_NODESET:{
                        int indx;

                        for (indx = 0;
                             indx < list->nodesetval->nodeNr; indx++) {
                            if (dir)
                                xmlShellList(ctxt, NULL,
                                             list->nodesetval->
                                             nodeTab[indx], NULL);
                            else
                                xmlShellList(ctxt, NULL,
                                             list->nodesetval->
                                             nodeTab[indx], NULL);
                        }
                        result = 1;
                        break;
                    }
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
 * xslShellCat:
 * @node : Is valid
 * @file : Is valid
 *
 * Send the results of cat command in @node to @file
 */
void
xslShellCat(xmlNodePtr node, FILE * file)
{
    if (!node || !file)
        return;

    /* assume that HTML usage is enabled */
    if (node->doc->type == XML_HTML_DOCUMENT_NODE) {
        if (node->type == XML_HTML_DOCUMENT_NODE)
            htmlDocDump(file, (htmlDocPtr) node);
        else
            htmlNodeDumpFile(file, node->doc, node);
    } else if (node->type == XML_DOCUMENT_NODE) {
        /* turn off encoding for the moment and just dump UTF-8 
         * which will be converted by xsldbgGeneralErrorFunc */
        xmlDocPtr doc = (xmlDocPtr) node;
        const xmlChar *encoding = doc->encoding;

        if (encoding) {
            xsltGenericError(xsltGenericErrorContext,
                             "Information: Temporarily setting document's"
                             " encoding to UTF-8, was previously %s\n",
                             encoding);
        }
        doc->encoding = (xmlChar *) "UTF-8";
        xmlDocDump(file, (xmlDocPtr) node);
        doc->encoding = encoding;
    } else {
        xmlElemDump(file, node->doc, node);
    }
}


/** 
 * xslDbgShellCat:
 * @styleCtxt: the current stylesheet context
 * @ctxt: The current shell context
 * @arg: The xpath to print (in UTF-8)
 *
 * Print the result of an xpath expression. This can include variables
 *        if styleCtxt is not NULL
 *
 * Returns 1 on success,
 *         0 otherwise
 */

int
xslDbgShellCat(xsltTransformContextPtr styleCtxt, xmlShellCtxtPtr ctxt,
               xmlChar * arg)
{
    xmlXPathObjectPtr list;
    int result = 0;

    if (!styleCtxt || !ctxt) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Unable to cat/print expression, No stylesheet properly loaded\n");
        return result;
    }
    if (arg == NULL)
        arg = (xmlChar *) "";

    if (arg[0] == 0) {
        /* do a cat of the current node */
        const char *fileName = filesTempFileName(0);
        FILE *file;

        if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
            /* send it to the application as an UTF-8 message */
            xmlShellCat(ctxt, NULL, ctxt->node, NULL);
            result = 1;
            return result;
        }

        if (!fileName) {
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Can't create temporary file for xslDbgCat\n");
            return result;
        }

        file = fopen(fileName, "w+");
        if (file) {
            xslShellCat(ctxt->node, file);
            fflush(file);
            rewind(file);
            while (!feof(file)
                   && fgets((char *) buffer, sizeof(buffer), file)) {
                xsltGenericError(xsltGenericErrorContext, "%s", buffer);
            }
            xsltGenericError(xsltGenericErrorContext, "\n");
            fclose(file);
            result = 1;
        } else
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Can't open temporary file %s for xslDbgCat\n",
                             fileName);
    } else {
        ctxt->pctxt->node = ctxt->node;
        if (!styleCtxt) {
            list = xmlXPathEval((xmlChar *) arg, ctxt->pctxt);
        } else {
            xmlNodePtr savenode = styleCtxt->xpathCtxt->node;

            ctxt->pctxt->node = ctxt->node;
            styleCtxt->xpathCtxt->node = ctxt->node;
            if (!xmlXPathNsLookup(styleCtxt->xpathCtxt, (xmlChar *) "xsl"))
                xmlXPathRegisterNs(styleCtxt->xpathCtxt, (xmlChar *) "xsl",
                                   XSLT_NAMESPACE);
            list = xmlXPathEval((xmlChar *) arg, styleCtxt->xpathCtxt);
            styleCtxt->xpathCtxt->node = savenode;
        }
        if (list != NULL) {
            switch (list->type) {
                case XPATH_NODESET:{
                        int indx;

                        if (list->nodesetval) {
                            const char *fileName = filesTempFileName(0);
                            FILE *file;

                            if (!fileName)
                                break;
                            file = fopen(fileName, "w+");
                            if (!file) {
                                xsltGenericError
                                    (xsltGenericErrorContext,
                                     "Error: Unable to save temporary"
                                     "results to %s\n", fileName);
                                break;
                            } else {
                                for (indx = 0;
                                     indx < list->nodesetval->nodeNr;
                                     indx++) {
                                    xslShellCat(list->nodesetval->
                                                nodeTab[indx], file);
                                }

                            }
                            if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
                                fclose(file);
                                /* send the data to application */
                                notifyXsldbgApp(XSLDBG_MSG_FILEOUT,
                                                fileName);
                            } else {
                                int lineCount = 0, gdbModeEnabled = 0;

                                /* save the value of option to speed things up
                                 * a bit */
                                gdbModeEnabled =
                                    optionsGetIntOption(OPTIONS_GDB);
                                rewind(file);

                                /* when gdb mode is enable then only print the first
                                 * GDB_LINES_TO_PRINT lines */
                                while (!feof(file)) {
                                    if (fgets
                                        ((char *) buffer, sizeof(buffer),
                                         file))
                                        xsltGenericError
                                            (xsltGenericErrorContext, "%s",
                                             buffer);
                                    if (gdbModeEnabled) {
                                        lineCount++;
                                        if (lineCount ==
                                            GDB_LINES_TO_PRINT) {
                                            xsltGenericError
                                                (xsltGenericErrorContext,
                                                 "...");
                                            break;
                                        }
                                    }
                                }
                                xsltGenericError
                                    (xsltGenericErrorContext, "\n");
                            }
                        } else {
                            xsltGenericError(xmlGenericErrorContext,
                                             "Error: xpath %s results an "
                                             "in empty set\n", arg);
                        }
                        result = 1;
                        break;
                    }

                case XPATH_BOOLEAN:
                    xsltGenericError(xsltGenericErrorContext,
                                     "%s\n", xmlBoolToText(list->boolval));
                    result = 1;
                    break;

                case XPATH_NUMBER:
                    xsltGenericError(xsltGenericErrorContext,
                                     "%0g\n", list->floatval);
                    result = 1;
                    break;

                case XPATH_STRING:
                    if (list->stringval) {
                        xsltGenericError(xsltGenericErrorContext,
                                         "%s\n", list->stringval);
                        result = 1;
                    }
                    break;

                default:
                    xmlShellPrintXPathError(list->type, (char *) arg);
            }
            xmlXPathFreeObject(list);
        } else {
            xsltGenericError(xsltGenericErrorContext,
                             "Error: %s no such node\n", arg);
        }
        ctxt->pctxt->node = NULL;
    }
    return result;
}

/* only used by xslDbgPrintNames and xslDbgPrintVariable cound number of variables */
static int varCount;

/*
 * xslDbgShellPrintNames:
 * Print a name of variable found by scanning variable table
 * It is used by print_variable function.
 * @payload : not used
 * @data : not used
 * @name : the variable name 
 */
void *
xslDbgShellPrintNames(void *payload ATTRIBUTE_UNUSED,
                      void *data ATTRIBUTE_UNUSED, xmlChar * name)
{
    if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
        notifyListQueue(payload);
    } else if (name) {
        xsltGenericError(xsltGenericErrorContext, " Global %s\n", name);
        varCount++;
    }
    return NULL;
}



/**
 * xslDbgShellPrintVariable:
 * @styleCtxt: The current stylesheet context 
 * @arg: The name of variable to look for '$' prefix is optional and in UTF-8
 * @type: A valid VariableTypeEnum
 *
 *  Print the value variable specified by args.
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
xslDbgShellPrintVariable(xsltTransformContextPtr styleCtxt, xmlChar * arg,
                         VariableTypeEnum type)
{
    int result = 0;

    varCount = 0;
    if (!styleCtxt) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Debugger has no files loaded or libxslt has not reached "
                         "a template.\nTry reloading files or taking more steps.\n");
        return result;
    }

    if (!arg) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
        return result;
    }

    if (arg[0] == 0) {
        /* list variables of type requested */
        if (type == DEBUG_GLOBAL_VAR) {
            if (styleCtxt->globalVars) {
                if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
                    notifyListStart(XSLDBG_MSG_GLOBALVAR_CHANGED);
                    /* list global variables */
                    xmlHashScan(styleCtxt->globalVars,
                                (xmlHashScanner) xslDbgShellPrintNames,
                                NULL);
                    notifyListSend();
                } else
                    /* list global variables */
                    xmlHashScan(styleCtxt->globalVars,
                                (xmlHashScanner) xslDbgShellPrintNames,
                                NULL);
                result = 1;
                /* ensure that the locals follow imediately after the 
                 * globals when in gdb mode */
                if (optionsGetIntOption(OPTIONS_GDB) == 0)
                    xsltGenericError(xsltGenericErrorContext, "\n");
            } else {
                if (getThreadStatus() != XSLDBG_MSG_THREAD_RUN) {
                    /* Don't show this message when running as a thread as it 
                     * is annoying */
                    xsltGenericError(xsltGenericErrorContext,
                                     "Error: Libxslt has not initialize variables yet"
                                     " try stepping to a template");
                } else {
                    /* send an empty list */
                    notifyListStart(XSLDBG_MSG_GLOBALVAR_CHANGED);
                    notifyListSend();
                    result = 1;
                }
            }
        } else {
            /* list local variables */
            if (styleCtxt->varsBase) {
                xsltStackElemPtr item =
                    styleCtxt->varsTab[styleCtxt->varsBase];
                if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
                    notifyListStart(XSLDBG_MSG_LOCALVAR_CHANGED);
                    while (item) {
                        notifyListQueue(item);
                        item = item->next;
                    }
                    notifyListSend();
                } else {
                    while (item) {
                        if (item->name) {
                            xsltGenericError(xsltGenericErrorContext,
                                             " Local %s \n", item->name);
                        }
                        item = item->next;
                    }
                }
                result = 1;
                xsltGenericError(xsltGenericErrorContext, "\n");
            } else {
                if (getThreadStatus() != XSLDBG_MSG_THREAD_RUN) {
                    /* Don't show this message when running as a thread as it 
                     * is annoying */
                    xsltGenericError(xsltGenericErrorContext,
                                     "Error: Libxslt has not initialize variables yet"
                                     " try stepping past the xsl:param elements in template");
                } else {
                    /* send an empty list */
                    notifyListStart(XSLDBG_MSG_LOCALVAR_CHANGED);
                    notifyListSend();
                    result = 1;
                }
            }
        }
    } else {
        /* Display the value of variable */
        if (arg[0] == '$')
            xmlShellPrintXPathResult(xmlXPathEval
                                     (arg, styleCtxt->xpathCtxt));
        else {
            xmlChar tempbuff[100];

            xmlStrCpy(tempbuff, "$");
            xmlStrCat(tempbuff, arg);
            xmlShellPrintXPathResult(xmlXPathEval(tempbuff,
                                                  styleCtxt->xpathCtxt));
        }
        result = 1;
    }
    return result;
}
