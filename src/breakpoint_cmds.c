
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

/* temp buffer needed occationaly */
static xmlChar buff[DEBUG_BUFFER_SIZE];

/* ---------------------------------------------------
   Private function declarations for breakpoint_cmds.c
 ----------------------------------------------------*/

/**
 * addSourceBreakPoint:
 * @url : is valid name of stylesheet source
 * @lineNo : lineNo >= 0
 *
 * Returns 1 if able to add specified source break point
 *         0 otherwise
 */
int addSourceBreakPoint(xmlChar * url, long lineNo);

/**
 * addDataBreakPoint:
 * @url : is valid name of a xml data file
 * @lineNo : lineNo >= 0
 *
 * Returns 1 if able to add specified xml data break point
 *         0 otherwise
 */
int addDataBreakPoint(xmlChar * url, long lineNo);


/* ------------------------------------- 
    End private functions
---------------------------------------*/



/* -----------------------------------------

   BreakPoint related commands

  ------------------------------------------- */


/**
 * xslDbgShellFrameBreak:
 * @arg: Is valid 
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

    // how many frames to go up/down
    int noOfFrames;

    if (!arg) {
        return result;
    }

    if (xmlStrLen(arg) > 0) {
        if (!sscanf((char *) arg, "%d", &noOfFrames)) {
            xsltGenericError(xsltGenericErrorContext,
                             "Error : Unable to read number of frames\n");
            return result;
        }
    } else {
        noOfFrames = 0;
    }

    if (stepup) {
        result = stepupToDepth(callDepth() - noOfFrames);
    } else {
        result = stepdownToDepth(callDepth() + noOfFrames);
    }

    if (!result)
        xsltGenericError(xsltGenericErrorContext,
                         "Error : Failed to set frame break point\n");
    return result;
}


/**
 * addSourceBreakPoint:
 * @url : is valid name of stylesheet source
 * @lineNo : lineNo >= 0
 *
 * Returns 1 if able to add specified source break point
 *         0 otherwise
 */
int
addSourceBreakPoint(xmlChar * url, long lineNo)
{
    int result = 0;
    int type;

    searchInfoPtr searchInf = searchNewInfo(SEARCH_NODE);
    nodeSearchDataPtr searchData = NULL;

    if (searchInf && searchInf->data) {
        type = DEBUG_BREAK_SOURCE;
        searchData = (nodeSearchDataPtr) searchInf->data;
        searchData->lineNo = lineNo;
        searchData->nameInput = (xmlChar *) xmlMemStrdup((char *) url);
        guessStylesheetName(searchInf);
        /* try to verify that the line number is valid */
        if (searchInf->found) {
            /* ok it looks like we've got a valid url */
            if (searchData->node) {
                searchInf->found = 0;
                /* searchData->url will be freed by searchFreeInfo */
                if (searchData->absoluteNameMatch)
                    searchData->url = (xmlChar *)
                        xmlMemStrdup((char *) searchData->
                                     absoluteNameMatch);
                else
                    searchData->url = (xmlChar *)
                        xmlMemStrdup((char *) searchData->
                                     guessedNameMatch);
                url = searchData->url;  /* we don't need another copy of url */
                /* searchData->node is set to the topmost node in stylesheet */
                walkChildNodes((xmlHashScanner) scanForNode, searchInf,
                               searchData->node);
                if (!searchInf->found) {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Warning : Breakpoint at file %s : line %ld doesn't "
                                     "seem to be valid. But set it anyhow\n",
                                     url, lineNo);
                }
            }
            if (addBreakPoint(url, lineNo, NULL, type))
                result++;

        } else {
            xsltGenericError(xsltGenericErrorContext,
                             "Error : Unable to find a stylesheet file whose name contains %s\n",
                             url);
        }
    } else {
        xsltGenericError(xsltGenericErrorContext,
                         "Error : Unable to create searchInfo, out of memory?\n");
    }

    if (searchInf)
        searchFreeInfo(searchInf);

    return result;
}



/**
 * addDataBreakPoint:
 * @url : is valid name of a xml data file
 * @lineNo : lineNo >= 0
 *
 * Returns 1 if able to add specified xml data break point
 *         0 otherwise
 */
int
addDataBreakPoint(xmlChar * url, long lineNo)
{
    int result = 0;
    int type;
    searchInfoPtr searchInf = searchNewInfo(SEARCH_NODE);

    nodeSearchDataPtr searchData = NULL;

    type = DEBUG_BREAK_DATA;

    if (searchInf && searchInf->data && getMainDoc()) {
        /* try to verify that the line number is valid */
        searchData = (nodeSearchDataPtr) searchInf->data;
        searchData->lineNo = lineNo;
        searchData->url = (xmlChar *) xmlMemStrdup((char *) url);
        walkChildNodes((xmlHashScanner) scanForNode, searchInf,
                       (xmlNodePtr) getMainDoc());

        /* try to guess file name by adding the prefix of main document */
        if (!searchInf->found) {
            char *lastSlash = xmlStrrChr(getMainDoc()->URL, PATHCHAR);

            if (lastSlash) {
                lastSlash++;
                xmlStrnCpy(buff, getMainDoc()->URL,
                           lastSlash - (char *) getMainDoc()->URL);
                buff[lastSlash - (char *) getMainDoc()->URL] = '\0';
                xmlStrCat(buff, url);
                if (searchData->url)
                    xmlFree(searchData->url);
                searchData->url = (xmlChar *) xmlMemStrdup((char *) buff);
                walkChildNodes((xmlHashScanner) scanForNode, searchInf,
                               (xmlNodePtr) getMainDoc());
                if (searchInf->found)
                    url = searchData->url;
            }
        }

        if (!searchInf->found)
            xsltGenericError(xsltGenericErrorContext,
                             "Warning ; Breakpoint at file %s : line %ld doesn't "
                             "seem to be valid. But set it anyhow\n",
                             url, lineNo);

        if (addBreakPoint(url, lineNo, NULL, type))
            result++;
    } else {
        xsltGenericError(xsltGenericErrorContext,
                         "Error : Unable to create searchInfo out of memory?\n");
    }

    if (searchInf)
        searchFreeInfo(searchInf);

    return result;
}


/**
 * xslDbgShellBreak:
 * @arg: Is valid
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

    if (style == NULL) {
        style = getStylesheet();
    }
    if (!arg || !style) {
        xsltGenericError(xsltGenericErrorContext,
                         "Errror : Debuger has no files loaded, try reloading files\n");
        return result;
    }
    if (arg[0] == '-') {
        xmlChar *opts[2];

        if ((xmlStrLen(arg) > 1) && (arg[1] == 'l')) {
            if (splitString(&arg[2], 2, opts) == 2) {
                if (!sscanf((char *) opts[1], "%ld", &lineNo)) {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Error : Unable to read line number \n");
                    return result;
                } else {
                    /* try to guess whether we are looking for source or data 
                     * break point
                     */
                    trimString(opts[0]);
                    if (strstr((char *) opts[0], ".xsl"))
                        result = addSourceBreakPoint(opts[0], lineNo);
                    else
                        result = addDataBreakPoint(opts[0], lineNo);
                }
            } else
                xsltGenericError(xsltGenericErrorContext,
                                 "Error : Missing arguments to break command\n");
        }
    } else if (xmlStrCmp(arg, "*") != 0) {
        xmlNodePtr templNode = findTemplateNode(style, arg);

        if (templNode && templNode->doc) {
            if (!addBreakPoint
                (templNode->doc->URL, xmlGetLineNo(templNode), arg,
                 DEBUG_BREAK_SOURCE))
                xsltGenericError(xsltGenericErrorContext,
                                 "Error : Break point to template '%s' in file %s :"
                                 "line %d exists \n", arg,
                                 templNode->doc->URL,
                                 xmlGetLineNo(templNode));
            else
                result++;
        } else
            xsltGenericError(xsltGenericErrorContext,
                             "Error : Unable to find template '%s' \n",
                             arg);
    } else {
        /* add all template names */
        const xmlChar *name;
        xmlChar *defaultUrl = (xmlChar *) "<n/a>";
        int newBreakPoints = 0;
        xsltTemplatePtr templ;

        while (style) {
            templ = style->templates;
            while (templ) {
                if (templ->elem && templ->elem->doc
                    && templ->elem->doc->URL) {
                    url = templ->elem->doc->URL;
                } else {
                    url = defaultUrl;
                }
                if (templ->match)
                    name = templ->match;
                else
                    name = templ->name;

                if (name) {
                    if (!addBreakPoint(url, xmlGetLineNo(templ->elem),
                                          name, DEBUG_BREAK_SOURCE)) {
                        xsltGenericError(xsltGenericErrorContext,
                                         "Error : Can't add breakPoint to file %s : line %d\n",
                                         url, xmlGetLineNo(templ->elem));
                        xsltGenericError(xsltGenericErrorContext,
                                         "Error : BreakPoint to template '%s' in file %s :"
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
                             "Error : No templates found or unable to add any breakPoints\n ");
            url = NULL;         /* flag that we've printed partial error message about the problem url */
        } else {
            result++;
            xsltGenericError(xsltGenericErrorContext,
                             "Added %d new breakPoints\n", newBreakPoints);
        }

        if (defaultUrl)
            xmlFree(defaultUrl);
    }

    if (!result) {
        if (url)
            xsltGenericError(xsltGenericErrorContext,
                             "Failed to add breakPoint "
                             "at file %s: line %ld\n", url, lineNo);
        else
            xsltGenericError(xsltGenericErrorContext,
                             "Failed to some breakPoints\n");
    }
    return result;
}


/**
 * xslDbgShellDelete:
 * @arg: Is valid
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
    xslBreakPointPtr breakPoint;
    static const xmlChar *errorPrompt =
        (xmlChar *) "Failed to delete break point\n";
    if (!arg)
        return result;

    if (arg[0] == '-') {
        xmlChar *opts[2];

        if ((xmlStrLen(arg) > 1) && (arg[1] == 'l')) {
            if (splitString(&arg[2], 2, opts) == 2) {
                if (!sscanf((char *) opts[1], "%ld", &lineNo)) {
                    xsltGenericError(xsltGenericErrorContext,
                                     "\n%s\tUnable to read line number \n",
                                     errorPrompt);
                } else {
                    breakPoint = getBreakPoint(opts[0], lineNo);
                    if (!breakPoint || !deleteBreakPoint(breakPoint))
                        xsltGenericError(xsltGenericErrorContext,
                                         "\n%s\tBreak point to '%s' doesn't exist\n",
                                         errorPrompt, arg);
                    else
                        result++;
                }
            } else
                xsltGenericError(xsltGenericErrorContext,
                                 "\n%s\tMissing arguments to delete command\n",
                                 errorPrompt);
        }
    } else if (!xmlStrCmp("*", arg)) {
        result = 1;
        /*remove all from breakpoints */
        emptyBreakPoint();

    } else if (sscanf((char *) arg, "%d", &breakPointId)) {
        breakPoint = findBreakPointById(breakPointId);
        if (breakPoint) {
            result = deleteBreakPoint(breakPoint);
            if (!result) {
                xsltGenericError(xsltGenericErrorContext,
                                 "\nUnable to delete breakpoint %d\n",
                                 breakPointId);
            }
        } else {
            xsltGenericError(xsltGenericErrorContext,
                             "\nBreakpoint %d doesn't exist\n",
                             breakPointId);
        }
    } else {
        breakPoint = findBreakPointByName(arg);
        if (breakPoint) {
            result = deleteBreakPoint(breakPoint);
            if (!result) {
                xsltGenericError(xsltGenericErrorContext,
                                 "\nDelete breakpoint to template %s failed\n",
                                 arg);
            }
        } else
            xsltGenericError(xsltGenericErrorContext,
                             "%s\tBreakpoint to template '%s' doesn't exist\n",
                             errorPrompt, arg);
    }
    return result;
}


/**
 * xslDbgEnableBreakPoint:
 * @payload: A valid xslBreakPointPtr
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
xslDbgEnableBreakPoint(void *payload, void *data,
                       xmlChar * name ATTRIBUTE_UNUSED)
{
    if (payload && data) {
        enableBreakPoint((xslBreakPointPtr) payload, *(int *) data);
    }
}


/**
 * xslDbgShellEnable:
 * @arg : is valid
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
    xslBreakPointPtr breakPoint;
    static const xmlChar *errorPrompt =
        (xmlChar *) "Failed to enable/disable break point\n";
    if (!arg)
        return result;

    if (arg[0] == '-') {
        xmlChar *opts[2];

        if ((xmlStrLen(arg) > 1) && (arg[1] == 'l')) {
            if (splitString(&arg[2], 2, opts) == 2) {
                if (!sscanf((char *) opts[1], "%ld", &lineNo)) {
                    xsltGenericError(xsltGenericErrorContext,
                                     "\n%s\tUnable to read line number \n",
                                     errorPrompt);
                } else {
                    breakPoint = getBreakPoint(opts[0], lineNo);
                    if (breakPoint)
                        result = enableBreakPoint(breakPoint, enableType);
                    else
                        xsltGenericError(xsltGenericErrorContext, "\n%s",
                                         errorPrompt);
                }
            } else
                xsltGenericError(xsltGenericErrorContext,
                                 "\n%s\tMissing arguments to enable command\n",
                                 errorPrompt);
        }
    } else if (!xmlStrCmp("*", arg)) {
        result = 1;
        /*enable/disable all from breakpoints */
        walkBreakPoints((xmlHashScanner) xslDbgEnableBreakPoint,
                        &enableType);

    } else if (sscanf((char *) arg, "%d", &breakPointId)) {
        breakPoint = findBreakPointById(breakPointId);
        if (breakPoint) {
            result = enableBreakPoint(breakPoint, enableType);
            if (!result) {
                xsltGenericError(xsltGenericErrorContext,
                                 "\nUnable to enable breakpoint %d\n",
                                 breakPointId);
            }
        } else {
            xsltGenericError(xsltGenericErrorContext,
                             "\nBreakpoint %d doesn't exist\n",
                             breakPointId);
        }
    } else {
        breakPoint = findBreakPointByName(arg);
        if (breakPoint) {
            result = enableBreakPoint(breakPoint, enableType);
        } else
            xsltGenericError(xsltGenericErrorContext,
                             "\n%s\tBreakpoint to template '%s' doesn't exist\n",
                             errorPrompt, arg);
    }
    return result;
}


/**
 * xslDbgPrintBreakPoint:
 * @payload: A valid xslBreakPointPtr
 * @data: Not used
 * @name: Not used
 *
 * Print data given by scan of break points 
*/
void
xslDbgPrintBreakPoint(void *payload, void *data ATTRIBUTE_UNUSED,
                      xmlChar * name ATTRIBUTE_UNUSED)
{
    if (payload) {
        printCount++;
        xsltGenericError(xsltGenericErrorContext, " ");
        printBreakPoint(stderr, (xslBreakPointPtr) payload);
        xsltGenericError(xsltGenericErrorContext, "\n");
    }
}
