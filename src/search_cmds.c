
/***************************************************************************
                          search_cmds.c  - search related commands for xsldbg
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
#include "breakpointInternals.h"

/* -----------------------------------------
   
           Seach related commands
   
  ------------------------------------------- */

/**
 * xslDbgShellSearch:
 * @styleCtxt :
 * @style :
 * @arg : xpath query to use for searching dataBase,
 *        may be preceeded by "-sort " to enable sorting of
 *        data
 * 
 * Return 1 if able to run query with @arg, 
 *        0 otherwise
 */
int
xslDbgShellSearch(xsltTransformContextPtr styleCtxt,
                  xsltStylesheetPtr style, xmlChar * arg)
{
    int result = updateSearchData(styleCtxt, style, NULL, DEBUG_ANY_VAR);
    xmlChar buff[DEBUG_BUFFER_SIZE];
    const xmlChar *sortOption = (xmlChar *) "-sort ";
    int sortOptionLen = xmlStrLen(sortOption);

    trimString(arg);
    if (xmlStrLen(arg) == 0) {
        arg = (xmlChar *) "//search/*";
    }
    strncpy((char *) buff, (char *) arg, sortOptionLen);
    if (!xmlStrCmp(buff, sortOption)) {
        /* yep do sorting as well */
        if (snprintf
            (buff, DEBUG_BUFFER_SIZE,
             "--param dosort 1 --param query \"%s\"",
             &arg[sortOptionLen])) {
            result = result && xslSearchQuery(NULL, buff);
        }
    } else {
        if (snprintf
            (buff, DEBUG_BUFFER_SIZE,
             "--param dosort 0 --param query \"%s\"", arg)) {
            result = result && xslSearchQuery(NULL, buff);
        }
    }
    return result;
}
