
/***************************************************************************
                          breakpoint_cmds.c  - breakpoint commands for xsldbg
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
#include "utils.h"
#include "xsldbgthread.h"       /* for getThreadStatus() */

#include "xsldbgmsg.h"

/* temp buffer needed occationaly */
static xmlChar buff[DEBUG_BUFFER_SIZE];

/* ---------------------------------------------------
   Private function declarations for breakpoint_cmds.c
 ----------------------------------------------------*/

/**
 * validateSource:
 * @url : is valid name of a xsl source file
 * @lineNo : lineNo >= 0
 *
 * Returns 1 if a breakpoint could be set at specified file url and line number
 *         0 otherwise
 */
int validateSource(xmlChar ** url, long *lineNo);

/**
 * validateData:
 * @url : is valid name of a xml data file
 * @lineNo : lineNo >= 0
 *
 * Returns 1 if a breakpoint could be set at specified file url and line number
 *         0 otherwise
 */
int validateData(xmlChar ** url, long *lineNo);


/* ------------------------------------- 
    End private functions
---------------------------------------*/



/* -----------------------------------------

   BreakPoint related commands

  ------------------------------------------- */


/**
 * xslDbgShellFrameBreak:
 * @arg: Is valid number of frames to change location by
 * @stepup: If != 1 then we step up, otherwise step down
 *
 * Set a "frame" break point either up or down from here
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
xslDbgShellFrameBreak(xmlChar * arg, int stepup)
{
    int result = 0;

    /* how many frames to go up/down */
    int noOfFrames;
    static const xmlChar *errorPrompt =
        (xmlChar *) "Failed to add break point\n";

    if (!filesGetStylesheet() || !filesGetMainDoc()) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error : Debugger has no files loaded, try reloading files\n%s",
                         errorPrompt);
        return result;
    }

    if (!arg) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
        return result;
    }

    if (xmlStrLen(arg) > 0) {
        if (!sscanf((char *) arg, "%d", &noOfFrames)) {
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Unable to read number of frames\n");
            return result;
        }
    } else {
        noOfFrames = 0;
    }

    if (stepup) {
        result = callStackStepup(callStackGetDepth() - noOfFrames);
    } else {
        result = callStackStepdown(callStackGetDepth() + noOfFrames);
    }

    if (!result)
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Failed to set frame break point\n");
    return result;
}


/**
 * validateSource:
 * @url : is valid name of a xsl source file
 * @lineNo : lineNo >= 0
 *
 * Returns 1 if a breakpoint could be set at specified file url and line number
 *         0 otherwise
 */
int
validateSource(xmlChar ** url, long *lineNo)
{

    int result = 0, type;
    searchInfoPtr searchInf;
    nodeSearchDataPtr searchData = NULL;

    if (!filesGetStylesheet()) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Stylesheet not valid files not loaded yet?\n");
        return result;
    }

    if (!url || !lineNo) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
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
                        xsltGenericError(xsltGenericErrorContext,
                                         "Warning: Breakpoint at file %s : line %ld doesn't "
                                         "seem to be valid.\n",
                                         *url, *lineNo);
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
                *url = xmlStrdup(searchData->url);
                result = 1;
            }
        } else
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Unable to find a stylesheet file whose name contains %s\n",
                             *url);
    }

    if (searchInf)
        searchFreeInfo(searchInf);
    else
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Unable to create searchInfo out of memory?\n");

    return result;
}




/**
 * validateData:
 * @url : is valid name of a xml data file
 * @lineNo : lineNo >= 0
 *
 * Returns 1 if a breakpoint could be set at specified file url and line number
 *         0 otherwise
 */
int
validateData(xmlChar ** url, long *lineNo)
{
    int result = 0;
    searchInfoPtr searchInf;
    nodeSearchDataPtr searchData = NULL;
    char *lastSlash;

    if (!filesGetMainDoc()) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Document not valid files not loaded yet?\n");
        return result;
    }

    if (!url || !lineNo) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
        return result;
    }

    searchInf = searchNewInfo(SEARCH_NODE);
    lastSlash = xmlStrrChr(filesGetMainDoc()->URL, PATHCHAR);

    if (lastSlash) {
        lastSlash++;
        xmlStrnCpy(buff, filesGetMainDoc()->URL,
                   lastSlash - (char *) filesGetMainDoc()->URL);
        buff[lastSlash - (char *) filesGetMainDoc()->URL] = '\0';
        xmlStrCat(buff, *url);
    } else
        xmlStrCpy(buff, "");


    if (searchInf && searchInf->data && filesGetMainDoc()) {
        /* try to verify that the line number is valid */
        searchData = (nodeSearchDataPtr) searchInf->data;
        if (lineNo != NULL)
            searchData->lineNo = *lineNo;
        else
            searchData->lineNo = -1;
        searchData->url = (xmlChar *) xmlMemStrdup((char *) *url);
        walkChildNodes((xmlHashScanner) scanForNode, searchInf,
                       (xmlNodePtr) filesGetMainDoc());

        /* try to guess file name by adding the prefix of main document */
        if (!searchInf->found) {
            if (xmlStrLen(buff) > 0) {
                if (searchData->url)
                    xmlFree(searchData->url);
                searchData->url = (xmlChar *) xmlMemStrdup((char *) buff);
                walkChildNodes((xmlHashScanner) scanForNode, searchInf,
                               (xmlNodePtr) filesGetMainDoc());
            }
        }

        if (!searchInf->found) {
            xsltGenericError(xsltGenericErrorContext,
                             "Warning: Breakpoint at file %s : line %ld doesn't "
                             "seem to be valid.\n", *url, *lineNo);
            result = 1;
        } else {
            if (*url)
                xmlFree(*url);
            *url = xmlStrdup(searchData->url);
            result = 1;
        }
    }

    if (searchInf)
        searchFreeInfo(searchInf);
    else
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Unable to create searchInfo out of memory?\n");

    return result;
}


/**
 * xslDbgShellBreak:
 * @arg: Is valid and in UTF-8
 * @style: Is valid
 * @ctxt: Is valid
 * 
 * Add break point specified by arg
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
xslDbgShellBreak(xmlChar * arg, xsltStylesheetPtr style,
                 xsltTransformContextPtr ctxt)
{
    int result = 0;
    long lineNo = -1;
    xmlChar *url = NULL;
    static const xmlChar *errorPrompt =
        (xmlChar *) "Failed to add break point\n";

    if (style == NULL) {
        style = filesGetStylesheet();
    }
    if (!style || !filesGetMainDoc()) {
        xsltGenericError(xsltGenericErrorContext,
                         "Errror : Debugger has no files loaded, try reloading files\n%s",
                         errorPrompt);
        return result;
    }

    if (!arg) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
        return result;
    }

    if (arg[0] == '-') {
        xmlChar *opts[2];

        if ((xmlStrLen(arg) > 1) && (arg[1] == 'l')) {
            if (splitString(&arg[2], 2, opts) == 2) {
                if (!sscanf((char *) opts[1], "%ld", &lineNo)) {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Error: Unable to read line number %s\n",
                                     errorPrompt);
                    return result;
                } else {
                    /* try to guess whether we are looking for source or data 
                     * break point
                     */
                    trimString(opts[0]);
                    url = xmlStrdup(opts[0]);
                    if (url) {
                        if (filesIsSourceFile(url)) {
                            if (validateSource(&url, &lineNo))
                                result =
                                    breakPointAdd(url, lineNo, NULL,
                                                  DEBUG_BREAK_SOURCE);
                        } else {
                            if (validateData(&url, &lineNo))
                                result =
                                    breakPointAdd(url, lineNo, NULL,
                                                  DEBUG_BREAK_DATA);
                        }
                    }
                }
            } else
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: break command arguments not in format \"-l <FILE_NAME> <LINE_NUMBER>\"\n");
        }
    } else if (xmlStrCmp(arg, "*") != 0) {
        /* Add breakpoint at supplied template name */
        xmlNodePtr templNode = findTemplateNode(style, arg);

        if (templNode && templNode->doc) {
            if (!breakPointAdd
                (templNode->doc->URL, xmlGetLineNo(templNode), arg,
                 DEBUG_BREAK_SOURCE))
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: Breakpoint to template '%s' in file %s :"
                                 "line %d exists \n", arg,
                                 templNode->doc->URL,
                                 xmlGetLineNo(templNode));
            else
                result = 1;
        } else
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Unable to find template '%s'\n", arg);
    } else {
        /* add breakpoint at all template names */
        const xmlChar *name;
        xmlChar *defaultUrl = (xmlChar *) "<n/a>";
        int newBreakPoints = 0;
        xsltTemplatePtr templ;

        while (style) {
            templ = style->templates;
            while (templ) {
                if (templ->elem && templ->elem->doc
                    && templ->elem->doc->URL) {
                    url = (xmlChar *) templ->elem->doc->URL;
                } else {
                    url = defaultUrl;
                }
                if (templ->match)
                    name = templ->match;
                else
                    name = templ->name;

                if (name) {
                    if (!breakPointAdd(url, xmlGetLineNo(templ->elem),
                                       name, DEBUG_BREAK_SOURCE)) {
                        xsltGenericError(xsltGenericErrorContext,
                                         "Error: Can't add breakPoint to file %s : line %d\n",
                                         url, xmlGetLineNo(templ->elem));
                        xsltGenericError(xsltGenericErrorContext,
                                         "Error: Breakpoint to template '%s' in file %s :"
                                         " line %d exists \n", name,
                                         templ->elem->doc->URL,
                                         xmlGetLineNo(templ->elem));
                    } else
                        newBreakPoints++;
                }
                templ = templ->next;
            }
            if (style->next)
                style = style->next;
            else
                style = style->imports;
        }
        if (newBreakPoints == 0) {
            xsltGenericError(xsltGenericErrorContext,
                             "Error: No templates found or unable to add any breakPoints\n ");
            url = NULL;         /* flag that we've printed partial error message about the problem url */
        } else {
            result = 1;
            xsltGenericError(xsltGenericErrorContext,
                             "Information: Added %d new breakPoints\n",
                             newBreakPoints);
        }

        if (defaultUrl)
            xmlFree(defaultUrl);
    }

    if (!result) {
        if (url)
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Failed to add breakpoint "
                             "at file %s: line %ld\n", url, lineNo);
        else
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Failed to add breakpoint(s)\n");
    }

    if (url)
        xmlFree(url);
    return result;
}


/**
 * xslDbgShellDelete:
 * @arg: Is valid and in UTF-8
 * 
 * Delete break point specified by arg
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
xslDbgShellDelete(xmlChar * arg)
{
    int result = 0, breakPointId;
    long lineNo;
    breakPointPtr breakPtr = NULL;
    static const xmlChar *errorPrompt =
        (xmlChar *) "Failed to delete break point\n";

    if (!filesGetStylesheet() || !filesGetMainDoc()) {
        xsltGenericError(xsltGenericErrorContext,
                         "Errror : Debugger has no files loaded, try reloading files\n%s",
                         errorPrompt);
        return result;
    }

    if (!arg) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
        return result;
    }

    if (arg[0] == '-') {
        xmlChar *opts[2], *url = NULL;

        if ((xmlStrLen(arg) > 1) && (arg[1] == 'l')) {
            if (splitString(&arg[2], 2, opts) == 2) {
                if (!sscanf((char *) opts[1], "%ld", &lineNo)) {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Error: Unable to read line number. %s\n",
                                     errorPrompt);
                } else {
                    url = xmlStrdup(opts[0]);
                    if (url) {
                        if (filesIsSourceFile(url)) {
                            if (validateSource(&url, &lineNo))
                                breakPtr = breakPointGet(url, lineNo);
                        } else if (validateData(&url, &lineNo))
                            breakPtr = breakPointGet(url, lineNo);
                        if (!breakPtr || !breakPointDelete(breakPtr))
                            xsltGenericError(xsltGenericErrorContext,
                                             "Error: Breakpoint to '%s' doesn't exist. %s\n",
                                             errorPrompt, arg);
                        else
                            result = 1;
                        xmlFree(url);
                    }
                }
            } else
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: delete command arguments not in format \"-l <FILE_NAME> <LINE_NUMBER>\" %s\n",
                                 errorPrompt);
        }
    } else if (!xmlStrCmp("*", arg)) {
        result = 1;
        /*remove all from breakpoints */
        breakPointEmpty();

    } else if (sscanf((char *) arg, "%d", &breakPointId)) {
        breakPtr = findBreakPointById(breakPointId);
        if (breakPtr) {
            result = breakPointDelete(breakPtr);
            if (!result) {
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: Unable to delete breakpoint %d . %s\n",
                                 breakPointId, errorPrompt);
            }
        } else {
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Breakpoint %d doesn't exist. %s\n",
                             breakPointId, errorPrompt);
        }
    } else {
        breakPtr = findBreakPointByName(arg);
        if (breakPtr) {
            result = breakPointDelete(breakPtr);
            if (!result) {
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: Delete breakpoint to template %s failed. %s\n",
                                 arg, errorPrompt);
            }
        } else
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Breakpoint at template '%s' doesn't exist. %s\n",
                             arg, errorPrompt);
    }
    return result;
}


/**
 * xslDbgShellEnableBreakPoint:
 * @payload: A valid breakPointPtr
 * @data: Enable type, a pointer to an integer 
 *         for a value of 
 *                 1 enable break point
 *                 0 disable break point
 *                 -1 toggle enabling of break point 
 * @name: Not used
 *
 * Enable/disable break points via use of scan of break points
*/
void
xslDbgShellEnableBreakPoint(void *payload, void *data,
                            xmlChar * name ATTRIBUTE_UNUSED)
{
    if (payload && data) {
        breakPointEnable((breakPointPtr) payload, *(int *) data);
    }
}


/**
 * xslDbgShellEnable:
 * @arg : is valid and in UTF-8
 * @enableType : enable break point if 1, disable if 0, toggle if -1
 *
 * Enable/disable break point specified by arg using enable 
 *      type of @enableType
 * Returns 1 if successful,
 *         0 otherwise
 */
int
xslDbgShellEnable(xmlChar * arg, int enableType)
{
    int result = 0, breakPointId;
    long lineNo;
    breakPointPtr breakPtr = NULL;
    static const xmlChar *errorPrompt =
        (xmlChar *) "Failed to enable/disable break point\n";

    if (!filesGetStylesheet() || !filesGetMainDoc()) {
        xsltGenericError(xsltGenericErrorContext,
                         "Errror : Debugger has no files loaded, try reloading files\n%s",
                         errorPrompt);
        return result;
    }

    if (!arg) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
        return result;
    }

    if (arg[0] == '-') {
        xmlChar *opts[2], *url = NULL;

        if ((xmlStrLen(arg) > 1) && (arg[1] == 'l')) {
            if (splitString(&arg[2], 2, opts) == 2) {
                if (!sscanf((char *) opts[1], "%ld", &lineNo)) {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Error: Unable to read line number. %s\n",
                                     errorPrompt);
                } else {
                    url = xmlStrdup(opts[0]);
                    if (url) {
                        if (strstr((char *) url, ".xsl")) {
                            if (validateSource(&url, NULL))
                                breakPtr = breakPointGet(url, lineNo);
                        } else if (validateData(&url, NULL))
                            breakPtr = breakPointGet(url, lineNo);
                        if (breakPtr)
                            result =
                                breakPointEnable(breakPtr, enableType);
                        else
                            xsltGenericError(xsltGenericErrorContext,
                                             "Error: %s", errorPrompt);
                        xmlFree(url);
                    }
                }
            } else
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: enable/disable command arguments not in format \"-l <FILE_NAME> <LINE_NUMBER>\" %s\n",
                                 errorPrompt);
        }
    } else if (!xmlStrCmp("*", arg)) {
        result = 1;
        /*enable/disable all from breakpoints */
        walkBreakPoints((xmlHashScanner) xslDbgShellEnableBreakPoint,
                        &enableType);

    } else if (sscanf((char *) arg, "%d", &breakPointId)) {
        breakPtr = findBreakPointById(breakPointId);
        if (breakPtr) {
            result = breakPointEnable(breakPtr, enableType);
            if (!result) {
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: Unable to enable/disable breakpoint %d\n",
                                 breakPointId);
            }
        } else {
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Breakpoint %d doesn't exist\n",
                             breakPointId);
        }
    } else {
        breakPtr = findBreakPointByName(arg);
        if (breakPtr) {
            result = breakPointEnable(breakPtr, enableType);
        } else
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Breakpoint at template '%s' does not exist. %s\n",
                             arg, errorPrompt);
    }
    return result;
}


/**
 * xslDbgShellPrintBreakPoint:
 * @payload: A valid breakPointPtr
 * @data: Not used
 * @name: Not used
 *
 * Print data given by scan of break points 
*/
void
xslDbgShellPrintBreakPoint(void *payload, void *data ATTRIBUTE_UNUSED,
                           xmlChar * name ATTRIBUTE_UNUSED)
{

    if (payload) {
        if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
            notifyListQueue(payload);
        } else {
            printCount++;
            xsltGenericError(xsltGenericErrorContext, " ");
            breakPointPrint(NULL, (breakPointPtr) payload);
            xsltGenericError(xsltGenericErrorContext, "\n");
        }
    }
}
