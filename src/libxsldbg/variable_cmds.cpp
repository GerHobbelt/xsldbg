
/***************************************************************************
              variable_cmds.cpp  - user commands to work with libxslt variables
                             -------------------
    begin                : Mon May 2 2016
    copyright            : (C) 2016 by Keith Isdale
    email                : keithisdale@gmail.com
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
#include "search.h"


int xslDbgShellSetVariable(xsltTransformContextPtr styleCtxt, xmlChar * arg)
{
    int result = 0, showUsage = 0;
    xmlChar *name, *nameURI, *selectExpr, *opts[3];

    if (!styleCtxt) {
	
        xsldbgGenericErrorFunc(QObject::tr("Error: Stylesheet is not valid.\n"));
        return result;
    }

    if (!arg) {
#ifdef WITH_XSLDBG_DEBUG_PROCESS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
#endif
        return result;
    }

    if (xmlStrLen(arg) > 1) {
        if (splitString(arg, 2, opts) == 2) {
            nameURI = NULL;
	    /* ignore any "$" prefix as user probably didn't mean that 
	       "$" is part of variable name*/
	    if (*opts[0] =='$'){
	      opts[0] = opts[0] + 1;
	    }
            name = xmlSplitQName2(opts[0], &nameURI);
            if (name == NULL)
                name = xmlStrdup(opts[0]);
            selectExpr = xmlStrdup(opts[1]);
            if (name && selectExpr) {
                xsltStackElemPtr def = NULL;

                if (styleCtxt->varsNr && styleCtxt->varsTab) {
                    /* try finding varaible in stack */
		    for (int i = styleCtxt->varsNr; i > styleCtxt->varsBase; i--) {
			    xsltStackElemPtr item = styleCtxt->varsTab[i-1];
			    while (item) {
				    if ((xmlStrCmp(name, item->name) == 0) &&
						    (item->nameURI == NULL
						     || (xmlStrCmp(name, item->nameURI) == 0))) {
					    def = item;
					    break;
				    }
				    item = item->next;
			    }
		    }
                }

                if (def == NULL)
                    def = (xsltStackElemPtr)
                        xmlHashLookup2(styleCtxt->globalVars,
                                       name, nameURI);
                if (def != NULL) {
                    if (def->select) {
			def->select = xmlDictLookup(styleCtxt->dict, selectExpr, -1);
			def->tree = NULL;  /* maybe a memory leak, but play it safe */
			def->computed = 1;
                        if (def->comp->comp)
                            xmlXPathFreeCompExpr(def->comp->comp);
                        def->comp->comp = xmlXPathCompile(def->select);
                        if (def->value)
                            xmlXPathFreeObject(def->value);
                        def->value =
                            xmlXPathEval(def->select,
                                         styleCtxt->xpathCtxt);
                        result = 1;
                    } else {
                        xmlFree(selectExpr);
                        xsldbgGenericErrorFunc(QObject::tr("Error: Cannot change a variable that does not use the select attribute.\n"));
                    }
                } else
                    xsldbgGenericErrorFunc(QObject::tr("Error: Variable %1 was not found.\n").arg(xsldbgText(name)));
                xmlFree(name);
            } else
                xsldbgGenericErrorFunc(QObject::tr("Error: Out of memory.\n"));
        } else {
            showUsage = 1;
        }

        if (showUsage == 1)
            xsldbgGenericErrorFunc(QObject::tr("Error: Invalid arguments to command %1.\n").arg(QString("set")));
    }
    return result;
}
