
/***************************************************************************
                search_cmds.cpp  - user commmands to search for content
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
#include "options.h"
#include "search.h"
#include <QDebug>

/* -----------------------------------------

           Seach related commands

  ------------------------------------------- */


int xslDbgShellSearch(xsltTransformContextPtr styleCtxt,
                  xsltStylesheetPtr style, xmlChar * arg)
{
    int result = 0;
    xmlChar buff[DEBUG_BUFFER_SIZE];
    const xmlChar *sortOption = (xmlChar *) "-sort";
    int sortOptionLen = xmlStrLen(sortOption);

    if (optionsGetStringOption(OPTIONS_DOCS_PATH).isEmpty()) {
        xsldbgGenericErrorFunc(i18n("Error: No path to documentation; aborting searching.\n"));
#ifdef USE_DOCS_MACRO
	xsldbgGenericErrorFunc(i18n("Error: Error in value of USE_DOCS_MACRO; look at Makefile.am.\n"));
#else
        xsldbgGenericErrorFunc(i18n("Error: Required environment variable %1 not set to the directory of xsldbg documentation.\n").arg((const char*)XSLDBG_DOCS_DIR_VARIABLE));
#endif
        return result;          /* failed */
    }

    if (!styleCtxt || !style) {
        xsldbgGenericErrorFunc(i18n("Error: Stylesheet not valid, files not loaded yet?\n"));
        return result;
    }

    result = updateSearchData(styleCtxt, style, NULL, DEBUG_ANY_VAR);
    if (result) {
        return result;
    }

    trimString(arg);
    if (xmlStrLen(arg) == 0) {
        arg = (xmlChar *) "//search/*";
    }
    strncpy((char *) buff, (char *) arg, sortOptionLen);
    if (xmlStrEqual(buff, sortOption)) {
        /* yep do sorting as well */
        int argCharIndex = sortOptionLen;
        if (xmlStrLen(&arg[argCharIndex]) == 0) {
            // handle case where no query value is provided
            arg = (xmlChar *) "//search/*";
            argCharIndex = 0;
        }

        if (snprintf
            ((char *) buff, DEBUG_BUFFER_SIZE,
             "--noshell --noautoloadconfig --param dosort 1 --param query \"%s\"",
             &arg[argCharIndex])) {
            result = searchQuery(NULL, NULL, buff);
        }
    } else {
        if (snprintf
            ((char *) buff, DEBUG_BUFFER_SIZE,
             "--noshell --noautoloadconfig --param dosort 0 --param query \"%s\"", arg)) {
            result = searchQuery(NULL, NULL, buff);
        }
    }
    return result;
}
