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

#include "config.h"
#include "xsldbg.h"
#include "debugXSL.h"

/* how may items have been printed */
int printCount;


/* -----------------------------------------

   BreakPoint related commands

  ------------------------------------------- */
xmlChar buff[DEBUG_BUFFER_SIZE];


/**
 * xslFrameBreak:
 *@arg : non-null 
 *@stepup : if != 1 then we step up, otherwise step down
 *
 * Set a "frame" breakpoint either up or down from here
 * Return 1 on success,
 *        0 otherwise
 */
int
xslDbgShellFrameBreak(xmlChar * arg, int stepup)
{
    int result = 0;

    // how many frames to go up/down
    int noOfFrames;
    static const xmlChar *errorPrompt =
        (xmlChar *) "Failed to set frame break point\n";
    if (!arg) {
        return result;
    }

    if (xmlStrLen(arg) > 0) {
        if (!sscanf((char *) arg, "%d", &noOfFrames)) {
            xsltGenericError(xsltGenericErrorContext,
                             "%s\tUnable to read number of frames \n",
                             errorPrompt);
            return result;
        }
    } else {
        noOfFrames = 0;
    }

    if (stepup) {
        result = xslStepupToDepth(xslCallDepth() - noOfFrames);
    } else {
        result = xslStepdownToDepth(xslCallDepth() + noOfFrames);
    }
    return result;
}


/**
 * xslDbgShellBreak:
 * @arg : non-null
 * 
 * Add break point specified by arg
 * Return 1 on success,
 *        0 otherwise
 */
int
xslDbgShellBreak(xmlChar * arg, xsltStylesheetPtr style)
{
    int result = 0;
    long lineNo;
    static const xmlChar *errorPrompt =
        (xmlChar *) "Failed to add break point\n";
    if (!arg || !style) {
        xsltGenericError(xsltGenericErrorContext,
                         "Debuger has no files loaded, try reloading files\n");
        return result;
    }
    if (arg[0] == '-') {
        xmlChar *opts[2];

        if ((xmlStrLen(arg) > 1) && (arg[1] == 'l')) {
            if (splitString(&arg[2], 2, opts) == 2) {
                if (!sscanf((char *) opts[1], "%ld", &lineNo)) {
                    xsltGenericError(xsltGenericErrorContext,
                                     "%s\tUnable to read line number \n",
                                     errorPrompt);
                    return result;
                } else {
                    int type;

                    /* try to guess whether we are looking for source or data 
                     * break point
                     */
                    if (strstr(opts[0], ".xsl")){
                        type = DEBUG_BREAK_SOURCE;
			opts[0] = guessStyleSheetName(opts[0]);
			if (!xslAddBreakPoint(opts[0], lineNo, NULL, type))
			  xsltGenericError(xsltGenericErrorContext, "%s",
					   errorPrompt);
			else{
			  xmlFree(opts[0]);
			  result++;
			}
		    }else{
                        type = DEBUG_BREAK_DATA;
			if (!xslAddBreakPoint(opts[0], lineNo, NULL, type))
			  xsltGenericError(xsltGenericErrorContext, "%s",
				       errorPrompt);
			else
			  result++;
		    }
                }
            } else
                xsltGenericError(xsltGenericErrorContext,
                                 "%s\tMissing arguments to break command\n",
                                 errorPrompt);
        }
    } else if (strcmp(arg, "*") != 0) {
        xmlNodePtr templNode = xslFindTemplateNode(style, arg);

        if (templNode && templNode->doc) {
            if (!xslAddBreakPoint
                (templNode->doc->URL, xmlGetLineNo(templNode), arg,
                 DEBUG_BREAK_SOURCE))
                xsltGenericError(xsltGenericErrorContext,
                                 "%s\tBreak point to template '%s' in file %s :"
                                 "line %d exists \n", errorPrompt, arg,
                                 templNode->doc->URL,
                                 xmlGetLineNo(templNode));
            else
                result++;
        } else
            xsltGenericError(xsltGenericErrorContext,
                             "%s\tUnable to find template '%s' \n",
                             errorPrompt, arg);
    } else {
        /* add all template names */
        const xmlChar *name, *defaultUrl = (xmlChar *) "<n/a>";
        const xmlChar *url;
        int templateCount = 0;
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
                    if (!xslAddBreakPoint(url, xmlGetLineNo(templ->elem),
                                          name, DEBUG_BREAK_SOURCE)) {
                        xsltGenericError(xsltGenericErrorContext,
                                         "Can't add breakpoint to file %s : line %d\n",
                                         url, xmlGetLineNo(templ->elem));
                        xsltGenericError(xsltGenericErrorContext,
                                         "%s\tBreak point to template '%s' in file %s :"
                                         " line %d exists \n", errorPrompt,
                                         name, templ->elem->doc->URL,
                                         xmlGetLineNo(templ->elem));
                    } else
                        templateCount++;
                }
                templ = templ->next;
            }
            if (style->next)
                style = style->next;
            else
                style = style->imports;
        }
        if (templateCount == 0) {
            xsltGenericError(xsltGenericErrorContext,
                             "No templates found or unable to add any break points\n ");
        }
    }

    return result;
}


/**
 * xslDbgShellDelete:
 * @arg : non-null
 * 
 * Delete break point specified by arg
 * Return 1 on success,
 *        0 otherwise
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
    } else if (!strcmp("*", arg)) {
        result = 1;
        /*remove all from breakpoints */
        xslEmptyBreakPoint();

    } else if (sscanf((char *) arg, "%d", &breakPointId)) {
        breakPoint = xslFindBreakPointById(breakPointId);
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
        breakPoint = xslFindBreakPointByName(arg);
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
 * @payload : valid xslBreakPointPtr
 * @data : enable type, a pointer to an integer 
 *         for a value of 
 *                 1 enable breakpoint
 *                 0 disable breakpoint
 *                 -1 toggle enabling of breakpoint 
 *
 * Enable/disable break points via use of scan of breakPoints
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
 * @arg : non-null
 * 
 * Enable/disable break point specified by arg
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
                        result =
                            enableBreakPoint(breakPoint, enableType);
                    else
                        xsltGenericError(xsltGenericErrorContext, "\n%s",
                                         errorPrompt);
                }
            } else
                xsltGenericError(xsltGenericErrorContext,
                                 "\n%s\tMissing arguments to enable command\n",
                                 errorPrompt);
        }
    } else if (!strcmp("*", arg)) {
        result = 1;
        /*enable/disable all from breakpoints */
        walkBreakPoints((xmlHashScanner) xslDbgEnableBreakPoint,
                           &enableType);

    } else if (sscanf((char *) arg, "%d", &breakPointId)) {
        breakPoint = xslFindBreakPointById(breakPointId);
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
        breakPoint = xslFindBreakPointByName(arg);
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
 * @payload : the breakpoint to print
 * @data : not used
 * @name : not used
 *
 * Print data given by scan of breakPoints 
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
