
/***************************************************************************
              template_cmds.cpp  - user commands to work with libxslt templates
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
                    int *templateCount, int *count, xmlChar * templateName, xmlListPtr templateList)
{
      if (!templ)
        return;

      xmlChar *name, *defaultUrl = (xmlChar *) "<n/a>";
      const xmlChar *url;

      *templateCount = *templateCount + 1;
      printTemplateHelper(templ->next, verbose,
                          templateCount, count, templateName, templateList);
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
              *count = *count + 1;
              if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
                  notifyListQueue(templ);
              } else {
                  xmlListAppend(templateList, templ);
              }
          }

          xmlFree(name);

      }
}
/**
 * _compareTemplates:
 * @data1:		the pointer to first Template
 * @data2:		the pointer to second Template
 *
 * Compares the namespaces by breakpoint id.
 *
 * Returns -1 if data1 < data2, 0 if data1 == data2 or 1 if data1 > data2.
 */
static int _compareTemplates(const void *data1, const void *data2)
{
    Q_CHECK_PTR(data1);
    Q_CHECK_PTR(data2);
    xmlChar *defaultUrl = (xmlChar *) "<n/a>";
    const xmlChar *url1, *url2;
    xsltTemplatePtr templ1 = (xsltTemplatePtr)data1;
    xsltTemplatePtr templ2 = (xsltTemplatePtr)data2;
    int temp1_lineNo = xmlGetLineNo(templ1->elem);
    int temp2_lineNo = xmlGetLineNo(templ2->elem);

    if (templ1->elem && templ1->elem->doc && templ1->elem->doc->URL) {
        url1 = templ1->elem->doc->URL;
    } else {
        url1 = defaultUrl;
    }

    if (templ2->elem && templ2->elem->doc && templ2->elem->doc->URL) {
        url2 = templ2->elem->doc->URL;
    } else {
        url2 = defaultUrl;
    }

    int result = xmlStrcmp(url1, url2);
    if (result == 0) {
        result = temp1_lineNo - temp2_lineNo;
    }

    return result;
}

struct templateListInfo {
    bool verbose;
    const xmlChar *templateSearchName;
    int templatesMatching;
};

#if LIBXML_VERSION > 20904
static int _templateListPrinter(const void *data, void *user)
#else
static int _templateListPrinter(const void *data, const void *user)
#endif
{
    Q_CHECK_PTR(data);
    Q_CHECK_PTR(user);

    xsltTemplatePtr templ = (xsltTemplatePtr)data;
    templateListInfo *infoPtr = (templateListInfo*)user;
    bool verbose = infoPtr->verbose;
    const xmlChar *templateName = infoPtr->templateSearchName;
    xmlChar *name, *defaultUrl = (xmlChar *) "<n/a>";
    const xmlChar *url;

    if (!templ)
        return 0;

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
                (xmlStrstr(name, templateName) == 0)) {
            /*  search for template name supplied failed */
            /* empty */
        } else {
            xmlChar *modeTemp = NULL;
            infoPtr->templatesMatching++;
            if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
                notifyListQueue(templ);
            } else {
                modeTemp = fullQName(templ->modeURI, templ->mode);

                if (verbose)
                    xsldbgGenericErrorFunc(QObject::tr(" template: \"%1\" mode: \"%2\" in file \"%3\" at line %4\n").arg(xsldbgText(name)).arg(xsldbgText(modeTemp)).arg(xsldbgUrl(url)).arg(xmlGetLineNo(templ->elem)));
                else
                    xsldbgGenericErrorFunc(QString(" template: \"%1\"\n").arg(xsldbgText(name)));
                if (modeTemp)
                    xmlFree(modeTemp);
            }
        }

        xmlFree(name);
    }

    return 1; // keep walking list
}


int xslDbgShellPrintTemplateNames(xsltTransformContextPtr styleCtxt,
                              xmlShellCtxtPtr ctxt,
                              xmlChar * arg, int verbose)
{
    Q_UNUSED(ctxt);
    int result = 0;
    xsltStylesheetPtr curStyle;
    xsltTemplatePtr templ;
    xmlListPtr templateList = xmlListCreate(NULL, _compareTemplates);
    const xmlChar *templateSearchName = NULL;

    if (xmlStrLen(arg) == 0) {
        arg = NULL;
    } else {
        templateSearchName = arg;
    }

    if (!styleCtxt) {
        xsldbgGenericErrorFunc(QObject::tr("Error: Stylesheet is not valid.\n"));
        return result;
    }

    curStyle = styleCtxt->style;

    while (curStyle) {
        templ = curStyle->templates;
        while(templ) {
            xmlListAppend(templateList, templ);
            templ = templ->next;
        }
        if (curStyle->next)
            curStyle = curStyle->next;
        else
            curStyle = curStyle->imports;
    }

    templateListInfo templateInfo;
    templateInfo.verbose = verbose;
    templateInfo.templatesMatching = 0;
    templateInfo.templateSearchName = templateSearchName;

    if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
        notifyListStart(XSLDBG_MSG_TEMPLATE_CHANGED);
    }

    xmlListSort(templateList);
    xmlListWalk(templateList, _templateListPrinter, &templateInfo);

    if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
         notifyListSend();
    } else {
        int templateCount = xmlListSize(templateList);
        if (!templateCount) {
            xsldbgGenericErrorFunc(QObject::tr("\n\tNo XSLT templates found.\n"));
        } else {
            xsldbgGenericErrorFunc(QObject::tr("\n\tTotal of %1 XSLT templates found\n").arg(templateInfo.templatesMatching));
            if (templateSearchName) {
                xsldbgGenericErrorFunc(QObject::tr("\tTotal of %1 XSLT templates available\n").arg(templateCount));
            }
        }
    }

    xmlListDelete(templateList);

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
        if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
            notifyListQueue(payload);
        } else {
            /* display the URL of stylesheet  */
            if (style->parent && style->parent->doc) {
                xsldbgGenericErrorFunc(QObject::tr(" Stylesheet \"%1\" with parent \"%2\"\n").arg(xsldbgUrl(style->doc->URL)).arg(xsldbgUrl(style->parent->doc->URL)));
            } else {
                xsldbgGenericErrorFunc(QObject::tr(" Stylesheet \"%1\"\n").arg(xsldbgUrl(style->doc->URL)));
            }
            printCounter++;
        }
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
        if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN){
            notifyListQueue(payload);
        }else {
            /* display the URL of stylesheet  */
            if (node->parent && node->parent->doc) {
                xsldbgGenericErrorFunc(QObject::tr(" Stylesheet \"%1\" with parent \"%2\"\n").arg(xsldbgUrl(node->doc->URL)).arg(xsldbgUrl(node->parent->doc->URL)));
            } else {
                xsldbgGenericErrorFunc(QObject::tr(" Stylesheet \"%1\"\n").arg(xsldbgUrl(node->doc->URL)));
            }
            printCounter++;
        }
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
