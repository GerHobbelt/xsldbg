
/***************************************************************************
                          template_cmds.c  - template commands for xsldbg
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

#include "config.h"
#include "xsldbg.h"
#include "debugXSL.h"

/* This is a semi private function defined in debugXSL.c */
/**
 * getTemplate:
 * 
 * Returns the last template node found, if any
 */
xsltTemplatePtr getTemplate();


/* invert the order of printin template names so that it prints in the 
same order that they are in file*/
int
xslDbgPrintTemplateHelper(xsltTemplatePtr templ, int verbose,
                          int templateCount)
{
    const xmlChar *name, *defaultUrl = (xmlChar *) "<n/a>";
    const xmlChar *url;

    if (templ) {
        templateCount =
            xslDbgPrintTemplateHelper(templ->next, verbose,
                                      templateCount + 1);
        if (templ->elem && templ->elem->doc && templ->elem->doc->URL) {
            url = templ->elem->doc->URL;
        } else {
            url = defaultUrl;
        }
        if (templ->match)
            name = templ->match;
        else
            name = templ->name;

        if (name) {
            if (verbose)
                xsltGenericError(xsltGenericErrorContext,
                                 " template :\"%s\" in file %s : line %ld\n",
                                 name, url, xmlGetLineNo(templ->elem));
            else
                xsltGenericError(xsltGenericErrorContext, "\"%s\" ", name);
        }
        templ = templ->next;
    }
    return templateCount;
}


/** 
 * xslDbgPrintTemplateNames:
 * @styleCtxt : is valid 
 * @ctxt : not used
 * @arg : not used
 * @verbose : if 1 then print extra messages about templates found,
 *            otherwise print normal messages only 
 * @allFiles : if 1 then look for all templates in stylsheets found in of @styleCtxt
 *             otherwise look in the stylesheet found by xslDebugBreak function
 *
 * print out the list of template names found that match critieria   
 */
void
xslDbgPrintTemplateNames(xsltTransformContextPtr styleCtxt,
                         xmlShellCtxtPtr ctxt ATTRIBUTE_UNUSED,
                         xmlChar * arg ATTRIBUTE_UNUSED, int verbose,
                         int allFiles)
{
    int templateCount = 0;
    xsltStylesheetPtr curStyle;
    xsltTemplatePtr templ;

    if (!styleCtxt) {
        xmlGenericError(xmlGenericErrorContext,
                        "Null styleCtxt supplied to xslDbgPrintTemplateNames\n");
        return;
    }

    if (allFiles)
        curStyle = styleCtxt->style;
    else {
        /* try to find files in the current stylesheet */
        /* root copy is set to the stylesheet found by xslDebugBreak */
        if (getTemplate())
            curStyle = getTemplate()->style;
        else
            curStyle = NULL;
    }

    while (curStyle) {
        templ = curStyle->templates;
        /* print them out in the order their in the file */
        templateCount =
            xslDbgPrintTemplateHelper(templ, verbose, templateCount);
        xsltGenericError(xsltGenericErrorContext, "\n");
        if (curStyle->next)
            curStyle = curStyle->next;
        else
            curStyle = curStyle->imports;

    }
    if (templateCount == 0) {
        xsltGenericError(xsltGenericErrorContext, "No templates found\n ");
    } else
        xsltGenericError(xsltGenericErrorContext,
                         "\t Total of %d templates found\n",
                         templateCount);
}
