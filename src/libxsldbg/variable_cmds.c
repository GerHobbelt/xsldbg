
/***************************************************************************
                          variable_cmds.c  -  description
                             -------------------
    begin                : Sun Dec 30 2001
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

#include <libxml/xpath.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/variables.h>  /* needed for xsltVariablesComp */
#include <libxml/valid.h>       /* needed for xmlSplitQName2 */
#include "xsldbg.h"
#include "debugXSL.h"
#include "xslsearch.h"

/*
void setGlobalVarHelper(void **payload, void *data ATTRIBUTE_UNUSED,
                xmlChar * name ATTRIBUTE_UNUSED);

void setGlobalVarHelper(void **payload, void *data ATTRIBUTE_UNUSED,
                xmlChar * name ATTRIBUTE_UNUSED)
{
   xsltStylesheetPtr style = (xsltStylesheetPtr)payload;
   int error = 0;
  searchInfoPtr info = (searchInfoPtr)data;
  if (style && info && (info->found == 0)){
    variableSearchDataPtr globalVarInfo = (variableSearchDataPtr)info->data;
    xsltStackElemPtr variable = style->variables;
    while (variable){
      if ((xmlStrCmp(globalVarInfo->name, variable->name) == 0) && 
	 (variable->nameURI == NULL || (xmlStrCmp(globalVarInfo->name, variable->nameURI) == 0 ))){
	if (variable->select && variable->comp && variable->comp->inst ){
                if (xmlSetProp(variable->comp->inst, (xmlChar *) "select",
			       globalVarInfo->select ) != NULL){
		  xsltVariableComp(style, variable->comp->inst);
		  xsltParseGlobalVariable(style, variable->comp->inst);
		}else
		  info->error = 1;
	}
	info->found = 1;
	break;
      }else
	variable= variable->next;
    } 
  }

}
*/

int
xslDbgShellSetVariable(xsltTransformContextPtr styleCtxt, xmlChar * arg)
{
    int result = 0, showUsage = 0;
    xmlChar *name, *nameURI, *selectExpr, *opts[3];

    if (xmlStrLen(arg) > 1) {
        if (splitString(arg, 3, opts) == 3) {
            if (opts[1][0] == '=') {
                nameURI = NULL;
                name = xmlSplitQName2(opts[0], &nameURI);
                if (name == NULL)
                    name = xmlStrdup(opts[0]);
                selectExpr = xmlStrdup(opts[2]);
                if (name && selectExpr) {
                    xsltStackElemPtr def = NULL;

                    if (styleCtxt->varsBase) {
                        /* try finding varaible in stack */
                        xsltStackElemPtr item =
                            styleCtxt->varsTab[styleCtxt->varsBase];
                        while (item) {
                            if ((xmlStrCmp(name, item->name) == 0) &&
                                (item->nameURI == NULL
                                 || (xmlStrCmp(name, item->nameURI) ==
                                     0))) {
                                def = item;
                                break;
                            }
                            item = item->next;
                        }
                    }

                    if (def == NULL)
                        def = (xsltStackElemPtr)
                            xmlHashLookup2(styleCtxt->globalVars,
                                           name, nameURI);
                    if (def != NULL) {
                        if (def->select) {
                            /* we've found the variable so change it */
                            xmlFree(def->select);
                            def->select = selectExpr;
                            if (def->comp->comp)
                                xmlXPathFreeCompExpr(def->comp->comp);
                            def->comp->comp = xmlXPathCompile(def->select);
                            if (def->value)
                                xmlXPathFreeObject(def->value);
                            def->value = xmlXPathNewString(def->select);
                            result++;
                        } else {
                            xmlFree(selectExpr);
                            xsltGenericError(xsltGenericErrorContext,
                                             "Can't change a variable that doesn't use the selectn attribute\n");
                        }
                    } else
                        xsltGenericError(xsltGenericErrorContext,
                                         "Variable %s not found\n", name);
                    xmlFree(name);
                } else
                    xsltGenericError(xsltGenericErrorContext,
                                     "Internal error set failed\n");
            } else {
                showUsage = 1;
            }
        } else {
            showUsage = 1;
        }

        if (showUsage == 1)
            xsltGenericError(xsltGenericErrorContext,
                             "Invalid format, expected\n set <NAME> = <VALUE>\n");
    }
    return result;
}
