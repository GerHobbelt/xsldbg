
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
#include "xsldbg.h"
#include "debugXSL.h"
#include "arraylist.h"
#include "xslbreakpoint.h"


/* -----------------------------------------
   Private function declarations for nodeview_cmds.c
 -------------------------------------------*/

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

/* ------------------------------------- 
    End private functions
---------------------------------------*/


/**
 * xslDbgShellPrintList: 
 * @ctxt: The current shell context
 * @arg: What xpath to display
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
                         "Debugger has no files loaded, try reloading files\n");
        return result;
    }

    if (arg[0] == 0) {
        if (dir)
            xmlShellDir(ctxt, NULL, ctxt->node, NULL);
        else
            xmlShellList(ctxt, NULL, ctxt->node, NULL);
			result++; /*assume that this worked */
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

                        for (indx = 0; indx < list->nodesetval->nodeNr;
                             indx++) {
                            if (dir)
                                xmlShellDir(ctxt, NULL,
                                            list->nodesetval->
                                            nodeTab[indx], NULL);
                            else
                                xmlShellList(ctxt, NULL,
                                             list->nodesetval->
                                             nodeTab[indx], NULL);
                        }
												result++;
                        break;
                    }
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
 * xslDbgShellCat:
 * @styleCtxt: the current stylesheet context
 * @ctxt: The current shell context
 * @arg: The xpath to print
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
    int i = 0;
		int result =0;

    if (!ctxt) {
        xsltGenericError(xsltGenericErrorContext,
                         "Debuger has no files loaded, try reloading files\n");
        return result;
    }
    if (arg == NULL)
        arg = (xmlChar *) "";
    if (arg[0] == 0) {
        xmlShellCat(ctxt, NULL, ctxt->node, NULL);
				result++;
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
                            for (indx = 0; indx < list->nodesetval->nodeNr;
                                 indx++) {
                                if (i > 0)
                                    xsltGenericError
                                        (xsltGenericErrorContext,
                                         " -------\n");
                                xmlShellCat(ctxt, NULL,
                                            list->nodesetval->
                                            nodeTab[indx], NULL);
                            }
                        } else {
                            xmlGenericError(xmlGenericErrorContext,
                                            "xpath %s: results an in empty set\n",
                                            arg);
                        }
                        break;
                    }

                case XPATH_BOOLEAN:
                    xmlGenericError(xmlGenericErrorContext,
                                    "%s is a Boolean:%s\n", arg,
                                    xmlBoolToText(list->boolval));
                    break;
                case XPATH_NUMBER:
                    xmlGenericError(xmlGenericErrorContext,
                                    "%s is a number:%0g\n", arg,
                                    list->floatval);
                    break;
                case XPATH_STRING:
                    xmlGenericError(xmlGenericErrorContext,
                                    "%s is a string:%s\n", arg,
                                    list->stringval);
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
    if (varCount)
        xsltGenericError(xsltGenericErrorContext, ", %s", name);
    else
        xsltGenericError(xsltGenericErrorContext, "%s", name);
    varCount++;
    return NULL;
}



/**
 * xslDbgShellPrintVariable:
 * @styleCtxt: The current stylesheet context 
 * @arg: The name of variable to look for '$' prefix is optional
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
                         "Debuger has no files loaded or libxslt has not reached "
                         "a template.\nTry reloading files or taking more steps.\n");
        return result;
    }

    if (arg[0] == 0) {
        /* list variables of type requested */
        if (type == DEBUG_GLOBAL_VAR) {
            if (styleCtxt->globalVars) {
                /* list global variables */
                xsltGenericError(xsltGenericErrorContext,
                                 "\nGlobal variables found: ");
                xmlHashScan(styleCtxt->globalVars, xslDbgShellPrintNames,
                            NULL);
								result++;
            } else {
                xsltGenericError(xsltGenericErrorContext,
                                 "Libxslt has not initialize variables yet"
                                 " try stepping to a template");
            }
        } else {
            /* list local variables */
            if (styleCtxt->varsBase) {
                xsltStackElemPtr item =
                    styleCtxt->varsTab[styleCtxt->varsBase];
                xsltGenericError(xsltGenericErrorContext,
                                 "\nLocal variables found: ");
                while (item) {
                    xsltGenericError(xsltGenericErrorContext, "%s ",
                                     item->name);
                    item = item->next;
                }
								result++;
            } else {
                xsltGenericError(xsltGenericErrorContext,
                                 "Libxslt has not initialize variables yet"
                                 " try stepping to a template");
            }
        }
        xsltGenericError(xsltGenericErrorContext, "\n");
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
				result++;
    }
	return result;
}