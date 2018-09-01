
/***************************************************************************
                          breakpoint_cmds.cpp  - user commands to work with breapoints
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
#include <libxml/valid.h>       /* needed for xmlSplitQName2 */
#include <libxml/xpathInternals.h> /* needed for xmlNSLookup */
#include "xsldbgthread.h"       /* for getThreadStatus() */
#include "xsldbgmsg.h"
#include "options.h"

/* temp buffer needed occationaly */
static xmlChar buff[DEBUG_BUFFER_SIZE];

/* needed by breakpoint validation */
extern int breakPointCounter;

/* we need to have a fake URL and line number for orphaned template breakpoints */
int orphanedTemplateLineNo = 1;
const xmlChar *orphanedTemplateURL= (xmlChar*)"http://xsldbg.sourceforge.net/default.xsl";
/* ---------------------------------------------------
   Private function declarations for breakpoint_cmds.c
 ----------------------------------------------------*/

int validateSource(xmlChar ** url, long *lineNo);

int validateData(xmlChar ** url, long *lineNo);


/* -------------------------------------
    End private functions
---------------------------------------*/



/* -----------------------------------------

   BreakPoint related commands

  ------------------------------------------- */


int xslDbgShellFrameBreak(xmlChar * arg, int stepup)
{
    int result = 0;

    /* how many frames to go up/down */
    int noOfFrames;
    static const char *errorPrompt = I18N_NOOP("Failed to add breakpoint.");

    if (!filesGetStylesheet() || !filesGetMainDoc()) {
        xsldbgGenericErrorFunc(QObject::tr("Error: Debugger has no files loaded. Try reloading files.\n"));
        xsldbgGenericErrorFunc(QString("Error: %1.\n").arg(QObject::tr(errorPrompt)));
        return result;
    }

    if (!arg) {
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
#endif
        xsldbgGenericErrorFunc(QString("Error: %1\n").arg(QObject::tr(errorPrompt)));
        return result;
    }

    if (xmlStrLen(arg) > 0) {
        if (!sscanf((char *) arg, "%d", &noOfFrames)) {
            xsldbgGenericErrorFunc(QObject::tr("Error: Unable to parse %1 as a number of frames.\n").arg((char*)arg));
            noOfFrames = -1;
        }
    } else {
        noOfFrames = 0;
    }

    if (noOfFrames >0){
        if (stepup) {
            result = callStackStepup(callStackGetDepth() - noOfFrames);
        } else {
            result = callStackStepdown(callStackGetDepth() + noOfFrames);
        }
    }

    if (!result)
        xsldbgGenericErrorFunc(QString("Error: %1\n").arg(QObject::tr(errorPrompt)));
    return result;
}


int validateSource(xmlChar ** url, long *lineNo)
{

    int result = 0, type;
    searchInfoPtr searchInf;
    nodeSearchDataPtr searchData = NULL;

    if (!filesGetStylesheet()) {
        xsldbgGenericErrorFunc(QObject::tr("Error: Stylesheet is not valid or file is not loaded.\n"));
        return result;
    }

    if (!url) {
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
#endif
        return result;
    }

    searchInf = searchNewInfo(SEARCH_NODE);

    if (searchInf && searchInf->data) {
        type = DEBUG_BREAK_SOURCE;
        searchData = (nodeSearchDataPtr) searchInf->data;
        if (lineNo != NULL)
            searchData->lineNo = *lineNo;
        searchData->nameInput = (xmlChar *) xmlMemStrdup((char *) *url);
        guessStylesheetName(searchInf);
        /* try to verify that the line number is valid */
        if (searchInf->found) {
            /* ok it looks like we've got a valid url */
            /* searchData->url will be freed by searchFreeInfo */
            if (searchData->absoluteNameMatch)
                searchData->url = (xmlChar *)
                        xmlMemStrdup((char *) searchData->absoluteNameMatch);
            else
                searchData->url = (xmlChar *)
                        xmlMemStrdup((char *) searchData->guessedNameMatch);

            if (lineNo != NULL) {
                /* now to check the line number */
                if (searchData->node) {
                    searchInf->found = 0;
                    /* searchData->node is set to the topmost node in stylesheet */
                    walkChildNodes((xmlHashScanner) scanForNode, searchInf,
                                   searchData->node);
                    if (!searchInf->found) {
                        xsldbgGenericErrorFunc(QObject::tr("Warning: Breakpoint for file \"%1\" at line %2 does not seem to be valid.\n").arg(xsldbgUrl(*url)).arg(*lineNo));
                    }

                    *lineNo = searchData->lineNo;
                    xmlFree(*url);
                    *url = xmlStrdup(searchData->url);
                    result = 1;
                }

            } else {
                /* we've been asked just to check the file name */
                if (*url)
                    xmlFree(*url);
                if (searchData->absoluteNameMatch)
                    *url = (xmlChar *)
                        xmlMemStrdup((char *) searchData->absoluteNameMatch);
                else
                    *url = (xmlChar *)
                        xmlMemStrdup((char *) searchData->guessedNameMatch);
                result = 1;
            }
        } else{
            xsldbgGenericErrorFunc(QObject::tr("Error: Unable to find a stylesheet file whose name contains %1.\n").arg(xsldbgUrl(*url)));
            if (lineNo){
                xsldbgGenericErrorFunc(QObject::tr("Warning: Breakpoint for file \"%1\" at line %2 does not seem to be valid.\n").arg(xsldbgUrl(*url)).arg(*lineNo));
            }
        }
    }

    if (searchInf)
        searchFreeInfo(searchInf);
    else
        xsldbgGenericErrorFunc(QObject::tr("Error: Out of memory.\n"));

    return result;
}




int validateData(xmlChar ** url, long *lineNo)
{
    int result = 0;
    searchInfoPtr searchInf;
    nodeSearchDataPtr searchData = NULL;

    if (!filesGetMainDoc()) {
        if (!optionsGetIntOption(OPTIONS_GDB)){
            xsldbgGenericErrorFunc(QObject::tr("Error: Data file is invalid. Try the run command first.\n"));
        }
        return result;
    }

    if (!url) {
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
#endif
        return result;
    }

    searchInf = searchNewInfo(SEARCH_NODE);
    if (searchInf && searchInf->data && filesGetMainDoc()) {
        /* Try to verify that the line number is valid.
       First try an absolute name match */
        searchData = (nodeSearchDataPtr) searchInf->data;
        if (lineNo != NULL)
            searchData->lineNo = *lineNo;
        else
            searchData->lineNo = -1;
        searchData->url = (xmlChar *) xmlMemStrdup((char *) *url);
        walkChildNodes((xmlHashScanner) scanForNode, searchInf,
                       (xmlNodePtr) filesGetMainDoc());

        if (!searchInf->found) {
            if (lineNo){
                xsldbgGenericErrorFunc(QObject::tr("Warning: Breakpoint for file \"%1\" at line %2 does not seem to be valid.\n").arg(xsldbgUrl(*url)).arg(*lineNo));
            } else{
                xsldbgGenericErrorFunc(QObject::tr("Error: Unable to find a data file whose name contains %1.\n").arg(xsldbgUrl(*url)));
            }
            result = 1;
        } else {
            if (*url)
                xmlFree(*url);
            *url = filesGetBaseUri(searchData->node);
            result = 1;
        }
    }

    if (searchInf)
        searchFreeInfo(searchInf);
    else
        xsldbgGenericErrorFunc(QObject::tr("Error: Out of memory.\n"));

    return result;
}


int xslDbgShellBreak(xmlChar * arg, xsltStylesheetPtr style,
                     xsltTransformContextPtr ctxt)
{
    int result = 0;
    long lineNo = -1;
    xmlChar *url = NULL;
    int orphanedBreakPoint = 0;
    breakPointPtr breakPtr;

    static const char *errorPrompt = I18N_NOOP("Failed to add breakpoint.");

    if (style == NULL) {
        style = filesGetStylesheet();
    }
    if (!style || !filesGetMainDoc()) {
        if (!optionsGetIntOption(OPTIONS_GDB)){
            xsldbgGenericErrorFunc(QObject::tr("Error: Debugger has no files loaded. Try reloading files.\n"));
            xsldbgGenericErrorFunc(QString("Error: %1\n").arg(QObject::tr(errorPrompt)));
            return result;
        }else{
            orphanedBreakPoint = 1;
        }
    }

    if (!arg) {
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
#endif
        return result;
    }

    if (arg[0] == '-') {
        xmlChar *opts[2];

        if ((xmlStrLen(arg) > 1) && (arg[1] == 'l')) {
            if (splitString(&arg[2], 2, opts) == 2) {
                if ((xmlStrlen(opts[1]) == 0) ||
                        !sscanf((char *) opts[1], "%ld", &lineNo)) {
                    xsldbgGenericErrorFunc(QObject::tr("Error: Unable to parse %1 as a line number.\n").arg((char*)opts[1]));
                    xsldbgGenericErrorFunc(QString("Error: %1\n").arg(QObject::tr(errorPrompt)));
                    return result;
                } else {
                    /* try to guess whether we are looking for source or data
                     * break point
                     */
                    trimString(opts[0]);
                    url = xmlStrdup((const xmlChar*)filesExpandName(xsldbgText(opts[0])).toUtf8().constData());
                    if (url) {
                        if (!orphanedBreakPoint){
                            if (filesIsSourceFile(url)) {
                                if (validateSource(&url, &lineNo))
                                    result =
                                            breakPointAdd(url, lineNo, NULL, NULL,
                                                          DEBUG_BREAK_SOURCE);
                            } else {
                                if (validateData(&url, &lineNo))
                                    result =
                                            breakPointAdd(url, lineNo, NULL, NULL,
                                                          DEBUG_BREAK_DATA);
                            }
                        }else{
                            if (filesIsSourceFile(url)) {
                                result =
                                        breakPointAdd(url, lineNo, NULL, NULL,
                                                      DEBUG_BREAK_SOURCE);
                            }else{
                                result =
                                        breakPointAdd(url, lineNo, NULL, NULL,
                                                      DEBUG_BREAK_DATA);
                            }
                            breakPtr = breakPointGet(url, lineNo);
                            if (breakPtr){
                                breakPtr->flags |= BREAKPOINT_ORPHANED;
                            }else{
                                xsldbgGenericErrorFunc(QObject::tr("Error: Unable to find the added breakpoint."));
                            }
                        }
                    }
                }
            } else
                xsldbgGenericErrorFunc(QObject::tr("Error: Invalid arguments to command %1.\n").arg(QString("break")));
        }
    } else {
        /* add breakpoint at specified template names */
        xmlChar *opts[2];
        xmlChar *name = NULL, *nameURI = NULL, *mode = NULL, *modeURI = NULL;
        xmlChar *templateName = NULL, *modeName = NULL;
        xmlChar *tempUrl = NULL; /* we must use a non-const xmlChar *
                   and we are not making a copy
                   of orginal value so this must not be
                   freed */
        xmlChar *defaultUrl = (xmlChar *) "<n/a>";
        int newBreakPoints = 0, validatedBreakPoints = 0;
        int allTemplates = 0;
        int ignoreTemplateNames = 0;
        int argCount;
        int found;
        xsltTemplatePtr templ;
        if (orphanedBreakPoint){
            /* Add an orphaned template breakpoint we will need to call this function later to
        activate the breakpoint */
            result =
                    breakPointAdd(orphanedTemplateURL, orphanedTemplateLineNo, arg, NULL,
                                  DEBUG_BREAK_SOURCE);
            breakPtr = breakPointGet(orphanedTemplateURL, orphanedTemplateLineNo++);
            if (breakPtr){
                breakPtr->flags |= BREAKPOINT_ORPHANED;
            }else{
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: Unable to find added breakpoint");
#endif
            }
            return result;
        }

        argCount = splitString(arg, 2, opts);
        if ((argCount == 2) && (xmlStrLen(opts[1]) == 0))
            argCount = 1;

        switch (argCount){
        case 0:
            allTemplates = 1;
            break;

        case 1:
            if (xmlStrEqual(opts[0], (xmlChar*)"*")){
                allTemplates = 1;
            }else{

                if (xmlStrEqual(opts[0], (xmlChar*)"\\*")){
                    opts[0][0] = '*';
                    opts[0][1] = '\0';
                }

                name = xmlSplitQName2(opts[0], &nameURI);
                if ((name == NULL) || (ctxt == NULL)){
                    name = xmlStrdup(opts[0]);
                }else{
                    if (nameURI){
                        /* get the real URI for this namespace */
                        const xmlChar *temp = xmlXPathNsLookup(ctxt->xpathCtxt, nameURI);
                        if (temp)
                            xmlFree(nameURI);
                        nameURI = xmlStrdup(temp);
                    }

                }
            }
            break;

        case 2:
            if (xmlStrLen(opts[0]) == 0){
                /* we don't care about the template name ie we are trying to match
            templates with a given mode */
                ignoreTemplateNames = 1;
            }else{
                name = xmlSplitQName2(opts[0], &nameURI);
                if ((name == NULL) || (ctxt == NULL))
                    name = xmlStrdup(opts[0]);
                if (nameURI){
                    /* get the real URI for this namespace */
                    const xmlChar *temp = xmlXPathNsLookup(ctxt->xpathCtxt,
                                                           nameURI);
                    if (temp)
                        xmlFree(nameURI);
                    nameURI = xmlStrdup(temp);
                }
            }
            mode = xmlSplitQName2(opts[1], &modeURI);
            if (mode == NULL)
                mode = xmlStrdup(opts[1]);
            if (modeURI && (ctxt != NULL)){
                /* get the real URI for this namespace */
                const xmlChar *temp = xmlXPathNsLookup(ctxt->xpathCtxt, modeURI);
                if (temp)
                    xmlFree(modeURI);
                modeURI = xmlStrdup(temp);
            }
            break;

        default:
            xsldbgGenericErrorFunc(QObject::tr("Error: Invalid arguments for command %1.\n").arg(QString("break")));
            return 0;
        }

        while (style) {
            templ = style->templates;
            while (templ) {
                found = 0;
                if (templ->elem && templ->elem->doc
                        && templ->elem->doc->URL) {
                    tempUrl = (xmlChar *) templ->elem->doc->URL;
                } else {
                    tempUrl = defaultUrl;
                }

                if (templ->match)
                    templateName = xmlStrdup(templ->match);
                else
                    templateName = fullQName(templ->nameURI, templ->name);

                if (allTemplates)
                    found = 1;
                else {
                    if (ignoreTemplateNames){
                        if (!mode || (xmlStrEqual(templ->mode, mode) &&
                                      (!modeURI || xmlStrEqual(templ->modeURI,
                                                               modeURI))))
                            found = 1;
                    } else if (templ->match){
                        if ((xmlStrEqual(templ->match, name) &&
                             (!modeURI || xmlStrEqual(templ->modeURI,
                                                      modeURI)) &&
                             (!mode || xmlStrEqual(templ->mode,
                                                   mode))))
                            found = 1;
                    }else{
                        if(xmlStrEqual(templ->name, name) &&
                                (!nameURI || xmlStrEqual(templ->nameURI, nameURI)))
                            found = 1;
                    }
                }
                if (found) {
                    int templateLineNo = xmlGetLineNo(templ->elem);
                    breakPointPtr searchPtr = breakPointGet(tempUrl, templateLineNo);

                    if (templ->mode)
                        modeName =
                                fullQName(templ->modeURI, templ->mode);


                    if (!searchPtr){
                        if (breakPointAdd(tempUrl, templateLineNo,
                                          templateName, modeName,
                                          DEBUG_BREAK_SOURCE)){
                            newBreakPoints++;
                        }
                    }else{

                        if ((templateLineNo != searchPtr->lineNo ) || !xmlStrEqual(tempUrl, searchPtr->url)){
                            int lastId = searchPtr->id;
                            int lastCounter = breakPointCounter;
                            /* we have a new location for breakpoint */
                            if (breakPointDelete(searchPtr)){
                                if (breakPointAdd(tempUrl, templateLineNo, templateName, modeName,DEBUG_BREAK_SOURCE)){
                                    searchPtr = breakPointGet(tempUrl, templateLineNo);
                                    if (searchPtr){
                                        searchPtr->id = lastId;
                                        result = 1;
                                        breakPointCounter = lastCounter;
                                        xsldbgGenericErrorFunc(QObject::tr("Information: Breakpoint validation has caused breakpoint %1 to be re-created.\n").arg(searchPtr->id));
                                        validatedBreakPoints++;
                                    }
                                }
                            }
                        }else{
                            if (xsldbgValidateBreakpoints != BREAKPOINTS_BEING_VALIDATED){
                                xsldbgGenericErrorFunc(QObject::tr("Warning: Breakpoint exits for file \"%1\" at line %2.\n").arg(xsldbgUrl(tempUrl)).arg(templateLineNo));
                            }
                            validatedBreakPoints++;
                        }
                    }
                }
                if (templateName){
                    xmlFree(templateName);
                    templateName = NULL;
                }
                if (modeName){
                    xmlFree(modeName);
                    modeName = NULL;
                }
                templ = templ->next;
            }
            if (style->next)
                style = style->next;
            else
                style = style->imports;
        }

        if ((newBreakPoints == 0) && (validatedBreakPoints == 0)) {
            xsldbgGenericErrorFunc(QObject::tr("Error: No templates found or unable to add breakpoint.\n"));
            url = NULL;         /* flag that we've printed partial error message about the problem url */
        } else {
            result = 1;
            if (newBreakPoints){
                xsldbgGenericErrorFunc(QObject::tr("Information: Added %1 new breakpoints.").arg(newBreakPoints) + QString("\n"));
            }
        }

        if (name)
            xmlFree(name);
        if (nameURI)
            xmlFree(nameURI);
        if (mode)
            xmlFree(mode);
        if (modeURI)
            xmlFree(modeURI);
        if (defaultUrl && !xmlStrEqual((xmlChar*)"<n/a>", defaultUrl))
            xmlFree(defaultUrl);
        if (tempUrl)
            url = xmlStrdup(tempUrl);
    }  /* end add template breakpoints */

    if (!result) {
        if (url)
            xsldbgGenericErrorFunc(QObject::tr("Error: Failed to add breakpoint for file \"%1\" at line %2.\n").arg(xsldbgUrl(url)).arg(lineNo));
        else
            xsldbgGenericErrorFunc(QObject::tr("Error: Failed to add breakpoint.\n"));
    }

    if (url)
        xmlFree(url);
    return result;
}


int xslDbgShellDelete(xmlChar * arg)
{
    int result = 0, breakPointId;
    long lineNo;
    breakPointPtr breakPtr = NULL;
    static const char *errorPrompt = I18N_NOOP("Failed to delete breakpoint.");

    if (!arg) {
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
#endif
        xsldbgGenericErrorFunc(QString("Error: %1\n").arg(QObject::tr(errorPrompt)));
        return result;
    }

    if (arg[0] == '-') {
        xmlChar *opts[2], *url = NULL;

        if ((xmlStrLen(arg) > 1) && (arg[1] == 'l')) {
            if (splitString(&arg[2], 2, opts) == 2) {
                if ((xmlStrlen(opts[1]) == 0) ||
                        !sscanf((char *) opts[1], "%ld", &lineNo)) {
                    xsldbgGenericErrorFunc(QObject::tr("Error: Unable to parse %1 as a line number.\n").arg((char*)opts[1]));
                } else {
                    trimString(opts[0]);
                    url = xmlStrdup((const xmlChar*)filesExpandName(xsldbgText(opts[0])).toUtf8().constData());
                    if (url) {
                        if (filesIsSourceFile(url)) {
                            if (validateSource(&url, &lineNo))
                                breakPtr = breakPointGet(url, lineNo);
                        } else if (validateData(&url, &lineNo))
                            breakPtr = breakPointGet(url, lineNo);
                        if (!breakPtr || !breakPointDelete(breakPtr)){
                            xsldbgGenericErrorFunc(QObject::tr("Error: Breakpoint does not exist for file \"%1\" at line %2.\n").arg(xsldbgUrl(url)).arg(lineNo));
                        }else{
                            result = 1;
                        }
                        xmlFree(url);
                    }
                }
            } else{
                xsldbgGenericErrorFunc(QObject::tr("Error: Invalid arguments for command %1.\n").arg(QString("delete")));
            }
        }
    } else if (xmlStrEqual((xmlChar*)"*", arg)) {
        result = 1;
        /*remove all from breakpoints */
        breakPointEmpty();

    } else if (sscanf((char *) arg, "%d", &breakPointId)) {
        breakPtr = findBreakPointById(breakPointId);
        if (breakPtr) {
            result = breakPointDelete(breakPtr);
            if (!result) {
                xsldbgGenericErrorFunc(QObject::tr("Error: Unable to delete breakpoint %1.\n").arg(breakPointId));
            }
        } else {
            xsldbgGenericErrorFunc(QObject::tr("Error: Breakpoint %1 does not exist.\n").arg(breakPointId));
        }
    } else {
        breakPtr = findBreakPointByName(arg);
        if (breakPtr) {
            result = breakPointDelete(breakPtr);
            if (!result) {
                xsldbgGenericErrorFunc(QObject::tr("Error: Unable to delete breakpoint at template %1.\n").arg(xsldbgText(arg)));
            }
        } else{
            xsldbgGenericErrorFunc(QObject::tr("Error: Breakpoint at template \"%1\" does not exist.\n").arg(xsldbgText(arg)));
        }
    }
    if (!result)
        xsldbgGenericErrorFunc(QString("Error: %1\n").arg(QObject::tr(errorPrompt)));
    return result;
}


void xslDbgShellEnableBreakPoint(void *payload, void *data,
                                 xmlChar * name)
{
    Q_UNUSED(name);
    if (payload && data) {
        breakPointEnable((breakPointPtr) payload, *(int *) data);
    }
}



int xslDbgShellEnable(xmlChar * arg, int enableType)
{
    int result = 0, breakPointId;
    long lineNo;
    breakPointPtr breakPtr = NULL;
    static const char *errorPrompt = I18N_NOOP("Failed to enable/disable breakpoint.");

    if (!filesGetStylesheet() || !filesGetMainDoc()) {
        xsldbgGenericErrorFunc(QObject::tr("Error: Debugger has no files loaded. Try reloading files.\n"));
        xsldbgGenericErrorFunc(QString("Error: %1\n").arg(QObject::tr(errorPrompt)));
        return result;
    }

    if (!arg) {
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
#endif
        xsldbgGenericErrorFunc(QString("Error: %1\n").arg(QObject::tr(errorPrompt)));
        return result;
    }

    if (arg[0] == '-') {
        xmlChar *opts[2], *url = NULL;

        if ((xmlStrLen(arg) > 1) && (arg[1] == 'l')) {
            if (splitString(&arg[2], 2, opts) == 2) {
                if ((xmlStrlen(opts[1]) == 0) ||
                        !sscanf((char *) opts[1], "%ld", &lineNo)) {
                    xsldbgGenericErrorFunc(QObject::tr("Error: Unable to parse %1 as a line number.\n").arg((char*)opts[1]));
                } else {
                    trimString(opts[0]);
                    url = xmlStrdup((const xmlChar *)filesExpandName(xsldbgText(opts[0])).toUtf8().constData());
                    if (url) {
                        if (strstr((char *) url, ".xsl")) {
                            if (validateSource(&url, NULL))
                                breakPtr = breakPointGet(url, lineNo);
                        } else if (validateData(&url, NULL))
                            breakPtr = breakPointGet(url, lineNo);
                        if (breakPtr){
                            result = breakPointEnable(breakPtr, enableType);
                        }else{
                            xsldbgGenericErrorFunc(QObject::tr("Error: Breakpoint does not exist for file \"%1\" at line %2.\n").arg(xsldbgUrl(url)).arg(lineNo));
                        }
                        xmlFree(url);
                    }
                }
            } else
                xsldbgGenericErrorFunc(QObject::tr("Error: Invalid arguments for command %1.\n").arg(QString("enable")));
        }
    } else if (xmlStrEqual((xmlChar*)"*", arg)) {
        result = 1;
        /*enable/disable all from breakpoints */
        walkBreakPoints((xmlHashScanner) xslDbgShellEnableBreakPoint,
                        &enableType);

    } else if (sscanf((char *) arg, "%d", &breakPointId)) {
        breakPtr = findBreakPointById(breakPointId);
        if (breakPtr) {
            result = breakPointEnable(breakPtr, enableType);
            if (!result) {
                xsldbgGenericErrorFunc(QObject::tr("Error: Unable to enable/disable breakpoint %1.\n").arg( breakPointId));
            }
        } else {
            xsldbgGenericErrorFunc(QObject::tr("Error: Breakpoint %1 does not exist.\n").arg(breakPointId));
        }
    } else {
        breakPtr = findBreakPointByName(arg);
        if (breakPtr) {
            result = breakPointEnable(breakPtr, enableType);
        } else
            xsldbgGenericErrorFunc(QObject::tr("Error: Breakpoint at template \"%1\" does not exist.\n").arg(xsldbgText(arg)));
    }

    if (!result)
        xsldbgGenericErrorFunc(QString("Error: %1\n").arg(QObject::tr(errorPrompt)));
    return result;
}


void xslDbgShellPrintBreakPoint(void *payload, void *data,
                                xmlChar * name)
{
    Q_UNUSED(data);
    Q_UNUSED(name);

    if (payload) {
        if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
            notifyListQueue(payload);
        } else {
            printCount++;
            xsldbgGenericErrorFunc(" ");
            breakPointPrint((breakPointPtr) payload);
            xsldbgGenericErrorFunc("\n");
        }
    }
}


/* Validiate a breakpoint at a given URL and line number
    breakPtr and copy must be valid
*/
static int validateBreakPoint(breakPointPtr breakPtr, breakPointPtr copy)
{

    int result = 0;
    if (!breakPtr || !copy){
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Warning: NULL arguments passed to validateBreakPoint\n");
#endif
        return result;
    }

    if (filesIsSourceFile(breakPtr->url)) {
        result = validateSource(&copy->url, &copy->lineNo);
    } else {
        result = validateData(&copy->url, &copy->lineNo);
    }
    if (result)
        breakPtr->flags &= BREAKPOINT_ALLFLAGS ^ BREAKPOINT_ORPHANED;
    else
        breakPtr->flags |= BREAKPOINT_ORPHANED;

    if ( breakPtr->flags & BREAKPOINT_ORPHANED){
        xsldbgGenericErrorFunc(QString("Warning: Breakpoint %1 is orphaned. Result: %2. Old flags: %3. New flags: %4.\n").arg(breakPtr->id).arg(result).arg(copy->flags).arg(breakPtr->flags));
    }

    if (!(breakPtr->flags & BREAKPOINT_ORPHANED) && ((copy->lineNo != breakPtr->lineNo ) ||
                                                     (xmlStrlen(copy->url) != xmlStrlen(breakPtr->url)) || xmlStrCmp(copy->url, breakPtr->url))){
        /* we have a new location for breakpoint */
        int lastCounter = breakPointCounter;
        copy->templateName = xmlStrdup(breakPtr->templateName);
        copy->modeName = xmlStrdup(breakPtr->modeName);
        if (breakPointDelete(breakPtr) && !breakPointGet(copy->url, copy->lineNo)){
            if (breakPointAdd(copy->url, copy->lineNo, NULL, NULL, copy->type)){
                breakPtr = breakPointGet(copy->url, copy->lineNo);
                if (breakPtr){
                    breakPtr->id = copy->id;
                    breakPtr->flags = copy->flags;
                    breakPointCounter = lastCounter; /* compensate for breakPointAdd which always
                            increments the breakPoint counter */
                    result = 1;
                    xsldbgGenericErrorFunc(QObject::tr("Information: Breakpoint validation has caused breakpoint %1 to be re-created.\n").arg(breakPtr->id));
                }
            }
            if (!result){
                xsldbgGenericErrorFunc(QObject::tr("Warning: Validation of breakpoint %1 failed.\n").arg(copy->id));
            }
        }
    }

    return result;
}


static int validateTemplateBreakPoint(breakPointPtr breakPtr, breakPointPtr copy, xsltTransformContextPtr ctxt)
{
    int result = 0;
    if (!breakPtr || !copy || !ctxt){
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Warning: NULL arguments passed to validateTemplateBreakPoint\n");
#endif
        return result;
    }

    copy->templateName = xmlStrdup(breakPtr->templateName);
    if ((xmlStrlen(copy->templateName) == 0) || xmlStrEqual(copy->templateName, (xmlChar*)"*")){
        if (xmlStrEqual(breakPtr->url, orphanedTemplateURL))
            breakPointDelete(breakPtr);
        if ( xslDbgShellBreak(copy->templateName, NULL, ctxt)){
            result = 1;
            xsldbgGenericErrorFunc(QObject::tr("Information: Breakpoint validation has caused one or more breakpoints to be re-created.\n"));
        }
    }else{
        if (xmlStrEqual(breakPtr->url, orphanedTemplateURL))
            breakPointDelete(breakPtr);
        if (xslDbgShellBreak(copy->templateName, NULL, ctxt)){
            result = 1;
        }
    }
    xmlFree(copy->templateName);
    if (!result){
        xsldbgGenericErrorFunc(QObject::tr("Warning: Validation of breakpoint %1 failed.\n").arg(copy->id));
    }
    return result;
}

/**
 * xslDbgShellValidateBreakPoint:
 * @payload: A valid breakPointPtr
 * @data: Not used
 * @name: Not used
 *
 * Print an warning if a breakpoint is invalid

 */
void xslDbgShellValidateBreakPoint(void *payload, void *data,
                                   xmlChar * name)
{
    Q_UNUSED(name);
    int result = 0;
    if (payload){
        breakPointPtr breakPtr = (breakPointPtr) payload;

        breakPoint copy; /* create a copy of the breakpoint */
        copy.lineNo = breakPtr->lineNo;
        copy.url = xmlStrdup(breakPtr->url);
        copy.flags = breakPtr->flags;
        copy.type = breakPtr->type;
        copy.id = breakPtr->id;
        if (copy.url){
            if (breakPtr->templateName){
                /* template name is used to contain the rules to add template breakpoint */
                result = validateTemplateBreakPoint(breakPtr, &copy, (xsltTransformContextPtr)data);
            }else{
                result = validateBreakPoint(breakPtr, &copy);
            }
        }else{
            xsldbgGenericErrorFunc(QObject::tr("Error: Out of memory.\n"));
        }

        xmlFree(copy.url);
    }
}
