
/***************************************************************************
                          param_cmds.c  - libxslt parameter commands for xsldbg
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

/* -----------------------------------------

   libxslt parameter related commands

  ------------------------------------------- */

/* 
 * xslDbgShellAddParam:
 * @arg : a string comprised of two words separated by
 *          one or more spaces.
 *
 * Add a parameter to be sent to libxslt later on
 * Return 1 on success,
 *        0 otherwise
*/
int
xslDbgShellAddParam(xmlChar * arg)
{
    int result = 0;
    static const xmlChar *errorPrompt =
        (xmlChar *) "Failed to add parameter\n";
    xmlChar *opts[2];

    if (!arg) {
        xsltGenericError(xsltGenericErrorContext,
                         "%sNull argument provided to xslDbgShellAddParam\n",
                         errorPrompt);
        return result;
    }
    if ((xmlStrLen(arg) > 1) && splitString(arg, 2, opts) == 2) {
        result = xslArrayListAdd(getParamItemList(),
                                 paramItemNew(opts[0], opts[1]));
    }
    if (!result)
        xsltGenericError(xsltGenericErrorContext, "%s", errorPrompt);
    return result;
}


/**
 * xslDbgShellDelParam:
 * @arg : single white space trimmed parameter name to look for
 * 
 * Return 1 if able to delete parameter @name,
 *        0 otherwise
 */
int
xslDbgShellDelParam(xmlChar * arg)
{
    int result = 0;
    static xmlChar *errorPrompt = (xmlChar *) "Failed to add parameter\n";
    long paramId;
    xmlChar *opts[2];

    if (!arg) {
        xsltGenericError(xsltGenericErrorContext,
                         "%sNull argument provided to xslDbgShellAddParam\n",
                         errorPrompt);
        return result;
    }
    if (xmlStrLen(arg) > 0) {
        if ((splitString(arg, 1, opts) == 1) &&
            (!sscanf((char *) opts[0], "%ld", &paramId))) {
            xsltGenericError(xsltGenericErrorContext,
                             "%s\tUnable to read line number \n",
                             errorPrompt);
            return result;
        } else
            result = xslArrayListDelete(getParamItemList(), paramId);
    } else {
        /* Delete all parameters */
        xslArrayListEmpty(getParamItemList());
        result++;
    }
    if (!result)
        xsltGenericError(xsltGenericErrorContext, "%s", errorPrompt);

    return result;
}

/**
 * xslDbgShellShowParam:
 * @arg : not used
 *
 * Print list of current paramters
 * Return 1 on success,
 *        0 otherwise
 */
int
xslDbgShellShowParam(xmlChar * arg ATTRIBUTE_UNUSED)
{
    int result = 0;

    if (printParamList())
        result++;
    else
        xsltGenericError(xsltGenericErrorContext,
                         "Error in printing parameters\n");
    return result;
}
