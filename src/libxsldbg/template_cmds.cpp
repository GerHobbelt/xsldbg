
/**
 *
 *  This file is part of the kdewebdev package
 *  Copyright (c) 2001 Keith Isdale <keith@kdewebdev.org>
 *
 *  This library is free software; you can redistribute it and/or 
 *  modify it under the terms of the GNU General Public License as 
 *  published by the Free Software Foundation; either version 2 of 
 *  the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 **/


#include "xsldbg.h"
#include "debugXSL.h"
#include "files.h"
#include "utils.h"
#include "xsldbgmsg.h"
#include "xsldbgthread.h"       /* for getThreadStatus */

static int printCounter;        /* Dangerous name think of a better one */

/* -----------------------------------------
   Private function declarations for files.c
 -------------------------------------------*/

/**
 * Print out the stylesheet name from the stylesheet given to
 *   us via walkStylesheets
 *
 * @param payload :valid xxsltStylesheetPtr
 * @param data :  not used
 * @parm name : not used
 *
 */
void xslDbgShellPrintStylesheetsHelper(void *payload, void *data, xmlChar * name);


/**
 *
 * Print out the stylesheet name from the stylesheet given to
 *   us via walkIncludes
 *
 * @param payload :valid xmlNodePtr of included stylesheet
 * @param data :  not used
 * @parm name : not used
 */
void   xslDbgShellPrintStylesheetsHelper2(void *payload, void *data, xmlChar * name);


/**
 * This display the templates in the same order as they are in the 
 *   stylesheet. If verbose is 1 then print more information
 *   For each template found @templateCount is increased
 *   For each printed template @printCount is increased
 *
 * @param templ: Is valid
 * @param verbose: Either 1 or 0
 * @param templateCount: Is valid
 * @param count: Is valid
 * @param templateName: template name to print and in UTF-8, may be NULL
 *
 */
void printTemplateHelper(xsltTemplatePtr templ, int verbose,
                         int *templateCount, int *count,
                         xmlChar * templateName);

/* ------------------------------------- 
    End private functions
---------------------------------------*/



void printTemplateHelper(xsltTemplatePtr templ, int verbose,
                    int *templateCount, int *count, xmlChar * templateName)
{
  xmlChar *name, *defaultUrl = (xmlChar *) "<n/a>";
    const xmlChar *url;

      if (!templ) 
        return;

      *templateCount = *templateCount + 1;
      printTemplateHelper(templ->next, verbose,
			  templateCount, count, templateName);
      if (templ->elem && templ->elem->doc && templ->elem->doc->URL) {
	url = templ->elem->doc->URL;
      } else {
	url = defaultUrl;
      }

      if (templ->match)
	name = xmlStrdup(templ->match);
      else
	name = fullQName(templ->nameURI, templ->name);

      if (name) {
	if (templateName &&
	    (xmlStrcmp(templateName, name) != 0)) {
	  /*  search for template name supplied failed */
	  /* empty */
	} else {
	  xmlChar *modeTemp = NULL;
	  *count = *count + 1;
	  if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
	    notifyListQueue(templ);
	  } else {
	    modeTemp = fullQName(templ->modeURI, templ->mode);
	    if (verbose)
	      xsldbgGenericErrorFunc(QObject::tr(" template: \"%1\" mode: \"%2\" in file \"%3\" at line %4\n").arg(xsldbgText(name)).arg(xsldbgText(modeTemp)).arg(xsldbgUrl(url)).arg(xmlGetLineNo(templ->elem)));
	    else
	       xsldbgGenericErrorFunc(QString("\"%s\" ").arg(xsldbgText(name)));
	    if (modeTemp)
	      xmlFree(modeTemp);
	  }
	}
	       
	xmlFree(name);
	  
      }
}


int xslDbgShellPrintTemplateNames(xsltTransformContextPtr styleCtxt,
                              xmlShellCtxtPtr ctxt,
                              xmlChar * arg, int verbose, int allFiles)
{
    Q_UNUSED(ctxt);
    int templateCount = 0, printedTemplateCount = 0;
    int result = 0;
    xsltStylesheetPtr curStyle;
    xsltTemplatePtr templ;

    if (xmlStrLen(arg) == 0) {
        arg = NULL;
    } else {
        allFiles = 1;           /* make sure we find it if we can */
    }

    if (!styleCtxt) {
        xsldbgGenericErrorFunc(QObject::tr("Error: Stylesheet is not valid.\n"));
        return result;
    }

    if (allFiles)
        curStyle = styleCtxt->style;
    else {
        /* try to find files in the current stylesheet */
        /* root copy is set to the stylesheet found by debugXSLBreak */
        if (debugXSLGetTemplate())
            curStyle = debugXSLGetTemplate()->style;
        else
            curStyle = NULL;
    }

    if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
        notifyListStart(XSLDBG_MSG_TEMPLATE_CHANGED);
        while (curStyle) {
            templ = curStyle->templates;
            /* print them out in the order their in the file */
            printTemplateHelper(templ, verbose, &templateCount,
                                &printedTemplateCount, arg);
            if (curStyle->next)
                curStyle = curStyle->next;
            else
                curStyle = curStyle->imports;
        }
        notifyListSend();
    } else {
        xsltGenericError(xsltGenericErrorContext, "\n");
        while (curStyle) {
            templ = curStyle->templates;
            /* print them out in the order their in the file */
            printTemplateHelper(templ, verbose, &templateCount,
                                &printedTemplateCount, arg);
            xsltGenericError(xsltGenericErrorContext, "\n");
            if (curStyle->next)
                curStyle = curStyle->next;
            else
                curStyle = curStyle->imports;
        }
        if (templateCount == 0) {
            xsldbgGenericErrorFunc(QObject::tr("\tNo XSLT templates found.\n"));
        } else {
	    xsldbgGenericErrorFunc(QObject::tr("\tTotal of %1 XSLT templates found").arg(templateCount) + QString("\n"));
	    xsldbgGenericErrorFunc(QObject::tr("\tTotal of %1 XSLT templates printed").arg(printedTemplateCount) + QString("\n"));
        }
    }

    result = 1;
    return result;
}


void xslDbgShellPrintStylesheetsHelper(void *payload,
                                  void *data,
                                  xmlChar * name)
{
    Q_UNUSED(data);
    Q_UNUSED(name);
    xsltStylesheetPtr style = (xsltStylesheetPtr) payload;

    if (style && style->doc && style->doc->URL) {
        if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN)
            notifyListQueue(payload);
        else
	    /* display the URL of stylesheet  */
	    xsldbgGenericErrorFunc(QObject::tr(" Stylesheet %1\n").arg(xsldbgUrl(style->doc->URL)));
        printCounter++;
    }
}


void xslDbgShellPrintStylesheetsHelper2(void *payload,
                                   void *data,
                                   xmlChar * name)
{
    Q_UNUSED(data);
    Q_UNUSED(name);
    xmlNodePtr node = (xmlNodePtr) payload;

    if (node && node->doc && node->doc->URL) {
        if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN)
            notifyListQueue(payload);
        else
	    /* display the URL of stylesheet  */
	     xsldbgGenericErrorFunc(QObject::tr(" Stylesheet %1\n").arg(xsldbgUrl(node->doc->URL)));
        printCounter++;
    }
}


int xslDbgShellPrintStyleSheets(xmlChar * arg)
{
    Q_UNUSED(arg);
    printCounter = 0;
    if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
        notifyListStart(XSLDBG_MSG_SOURCE_CHANGED);
        walkStylesheets((xmlHashScanner) xslDbgShellPrintStylesheetsHelper,
                        NULL, filesGetStylesheet());
        notifyListSend();
        notifyListStart(XSLDBG_MSG_INCLUDED_SOURCE_CHANGED);
        walkIncludes((xmlHashScanner) xslDbgShellPrintStylesheetsHelper2,
                     NULL, filesGetStylesheet());
        notifyListSend();
    } else {
        walkStylesheets((xmlHashScanner) xslDbgShellPrintStylesheetsHelper,
                        NULL, filesGetStylesheet());
        walkIncludes((xmlHashScanner) xslDbgShellPrintStylesheetsHelper2,
                     NULL, filesGetStylesheet());
        if (printCounter != 0)
	    xsldbgGenericErrorFunc(QObject::tr("\tTotal of %1 XSLT stylesheets found.").arg(printCounter) + QString("\n"));
        else
            /* strange but possible */
            xsldbgGenericErrorFunc(QObject::tr("\tNo XSLT stylesheets found.\n"));
    }
    return 1;                   /* always succeed */
}
