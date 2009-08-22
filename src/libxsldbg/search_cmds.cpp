
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
#include "options.h"
#include "search.h"

/* -----------------------------------------

           Seach related commands

  ------------------------------------------- */


int xslDbgShellSearch(xsltTransformContextPtr styleCtxt,
                  xsltStylesheetPtr style, xmlChar * arg)
{
    int result = 0;
    xmlChar buff[DEBUG_BUFFER_SIZE];
    const xmlChar *sortOption = (xmlChar *) "-sort ";
    int sortOptionLen = xmlStrLen(sortOption);

    if (optionsGetStringOption(OPTIONS_DOCS_PATH) == NULL) {
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
    trimString(arg);
    if (xmlStrLen(arg) == 0) {
        arg = (xmlChar *) "//search/*";
    }
    strncpy((char *) buff, (char *) arg, sortOptionLen);
    if (xmlStrEqual(buff, sortOption)) {
        /* yep do sorting as well */
        if (snprintf
            ((char *) buff, DEBUG_BUFFER_SIZE,
             "--param dosort 1 --param query \"%s\"",
             &arg[sortOptionLen])) {
            result = result && searchQuery(NULL, NULL, buff);
        }
    } else {
        if (snprintf
            ((char *) buff, DEBUG_BUFFER_SIZE,
             "--param dosort 0 --param query \"%s\"", arg)) {
            result = result && searchQuery(NULL, NULL, buff);
        }
    }
    return result;
}
