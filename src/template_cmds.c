
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

#include "xsldbg.h"
#include "debugXSL.h"
#include "files.h"

static int printCounter;        /* Dangerous name think of a better one */

/* -----------------------------------------
   Private function declarations for files.c
 -------------------------------------------*/

/**
 * xslDbgShellPrintStylesheetsHelper:
 * @payload :valid xxsltStylesheetPtr
 * @data :  not used
 * name : not used
 *
 * Print out the stylesheet name from the stylesheet given to
 *   us via walkStylesheets
 */
void
  xslDbgShellPrintStylesheetsHelper(void *payload,
                                    void *data ATTRIBUTE_UNUSED,
                                    xmlChar * name ATTRIBUTE_UNUSED);


/**
 * xslDbgShellPrintStylesheetsHelper2:
 * @payload :valid xmlNodePtr of included stylesheet
 * @data :  not used
 * name : not used
 *
 * Print out the stylesheet name from the stylesheet given to
 *   us via walkIncludes
 */
void
  xslDbgShellPrintStylesheetsHelper2(void *payload,
                                     void *data ATTRIBUTE_UNUSED,
                                     xmlChar * name ATTRIBUTE_UNUSED);

/* ------------------------------------- 
    End private functions
---------------------------------------*/



/**
 * xslDbgPrintTemplateHelper:
 * @templ : is valid
 * @verbose : either 1 or 0
 * @templateCount : is valid
 * @printCount : is valid
 * @templateName : template name to print, may be NULL
 *
 * This display the templates in the same order as they are in the 
 *   stylesheet. If verbose is 1 then print more information
 *   For each template found @templateCount is increased
 *   For each printed template @printCount is increased
 */
void
xslDbgPrintTemplateHelper(xsltTemplatePtr templ, int verbose,
                          int *templateCount, int *printCount,
                          xmlChar * templateName)
{
    const xmlChar *name, *defaultUrl = (xmlChar *) "<n/a>";
    const xmlChar *url;

    if (templ) {
        *templateCount = *templateCount + 1;
        xslDbgPrintTemplateHelper(templ->next, verbose,
                                  templateCount, printCount, templateName);
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
            if ((templateName != NULL) &&
                (xmlStrcmp(templateName, name) != 0)) {
                /*  search for template name supplied failed */
                /* empty */
            } else {
                *printCount = *printCount + 1;
                if (verbose)
                    xsltGenericError(xsltGenericErrorContext,
                                     " template :\"%s\" in file %s : line %ld\n",
                                     name, url, xmlGetLineNo(templ->elem));
                else
                    xsltGenericError(xsltGenericErrorContext, "\"%s\" ",
                                     name);
            }
        }
        templ = templ->next;
    }
}


/** 
 * xslDbgPrintTemplateNames:
 * @styleCtxt : is valid 
 * @ctxt : not used
 * @arg : if non-null then is the template name to look for
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
                         xmlChar * arg, int verbose, int allFiles)
{
    int templateCount = 0, printCount = 0;
    xsltStylesheetPtr curStyle;
    xsltTemplatePtr templ;

    trimString(arg);
    if (xmlStrLen(arg) == 0) {
        arg = NULL;
    } else {
        allFiles = 1;           /* make sure we find it if we can */
    }

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
        xslDbgPrintTemplateHelper(templ, verbose, &templateCount,
                                  &printCount, arg);
        xsltGenericError(xsltGenericErrorContext, "\n");
        if (curStyle->next)
            curStyle = curStyle->next;
        else
            curStyle = curStyle->imports;
    }
    if (templateCount == 0) {
        xsltGenericError(xsltGenericErrorContext, "No templates found\n ");
    } else {
        xsltGenericError(xsltGenericErrorContext,
                         "\t Total of %d templates found\n",
                         templateCount);
        xsltGenericError(xsltGenericErrorContext,
                         "\t Total of %d templates printed\n", printCount);
    }
}


/**
 * xslDbgShellPrintStylesheetsHelper:
 * @payload :valid xxsltStylesheetPtr
 * @data :  not used
 * name : not used
 *
 * Print out the stylesheet name from the stylesheet given to
 *   us via walkStylesheets
 */
void
xslDbgShellPrintStylesheetsHelper(void *payload,
                                  void *data ATTRIBUTE_UNUSED,
                                  xmlChar * name ATTRIBUTE_UNUSED)
{
    xsltStylesheetPtr style = (xsltStylesheetPtr) payload;

    if (style && style->doc && style->doc->URL) {
        xsltGenericError(xsltGenericErrorContext,
                         " Stylesheet %s\n", style->doc->URL);
        printCounter++;
    }
}


/**
 * xslDbgShellPrintStylesheetsHelper2:
 * @payload :valid xmlNodePtr of included stylesheet
 * @data :  not used
 * name : not used
 *
 * Print out the stylesheet name from the stylesheet given to
 *   us via walkIncludes
 */
void
xslDbgShellPrintStylesheetsHelper2(void *payload,
                                   void *data ATTRIBUTE_UNUSED,
                                   xmlChar * name ATTRIBUTE_UNUSED)
{
    xmlNodePtr node = (xmlNodePtr) payload;

    if (node && node->doc && node->doc->URL) {
        xsltGenericError(xsltGenericErrorContext,
                         " Stylesheet %s\n", node->doc->URL);
        printCounter++;
    }
}

/**
 * xslDbgPrintStyleSheets:
 * @arg : stylesheets of interests, is NULL for all stylsheets
 *
 * Print stylesheets that can be found in loaded stylsheet
 */
void
xslDbgPrintStyleSheets(xmlChar * arg)
{
    printCounter = 0;
    walkStylesheets((xmlHashScanner) xslDbgShellPrintStylesheetsHelper,
                    NULL, getStylesheet());
    walkIncludes((xmlHashScanner) xslDbgShellPrintStylesheetsHelper2,
                 NULL, getStylesheet());
    if (printCounter != 0)
        xsltGenericError(xsltGenericErrorContext,
                         "\n\tTotal of %d stylesheets found\n",
                         printCounter);
    else
        /* strange but possible */
        xsltGenericError(xsltGenericErrorContext,
                         "\n\tNo stylesheets found\n");
}
