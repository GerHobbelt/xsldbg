
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

#include "xsldbg.h"
#include "debugXSL.h"
#include "options.h"

#ifdef USE_XSLDBG_AS_THREAD
#include "xsldbgmsg.h"
#endif


/* -----------------------------------------

   libxslt parameter related commands

  ------------------------------------------- */

/** 
 * xslDbgShellAddParam:
 * @arg: A string comprised of two words separated by
 *          one or more spaces. 
 *
 * Add a libxslt parameter to be sent to libxslt later on
 *
 * Returns 1 on success,
 *         0 otherwise
*/
int
xslDbgShellAddParam(xmlChar * arg)
{
    int result = 0;
    ParameterItemPtr paramItem = NULL;
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
      paramItem =  paramItemNew(opts[0], opts[1]);
      result = arrayListAdd(getParamItemList(), paramItem);
    }
    if (!result)
        xsltGenericError(xsltGenericErrorContext, "%s", errorPrompt);
    else{
      xsltGenericError(xsltGenericErrorContext,"\n");
    }
    return result;
}


/**
 * xslDbgShellDelParam:
 * @arg: A single white space trimmed parameter number to look for
 * 
 * Delet a libxslt parameter to be sent to libxslt later on
 *
 * Returns 1 if able to delete parameter @name,
 *         0 otherwise
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
            result = arrayListDelete(getParamItemList(), paramId);
    } else {
        /* Delete all parameters */
        arrayListEmpty(getParamItemList());
        result++;
    }
    if (!result)
        xsltGenericError(xsltGenericErrorContext, "%s", errorPrompt);
    else
      xsltGenericError(xsltGenericErrorContext,"\n");

    return result;
}


/**
 * xslDbgShellShowParam:
 * @arg: Not used
 *
 * Print list of current paramters
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
xslDbgShellShowParam(xmlChar * arg ATTRIBUTE_UNUSED)
{
    int result = 0;

#ifdef USE_XSLDBG_AS_THREAD
    int paramIndex = 0;
    int itemCount = arrayListCount(getParamItemList());

   notifyXsldbgApp(XSLDBG_MSG_PARAMETER_CHANGED, NULL);

    if (itemCount > 0) {
      ParameterItemPtr paramItem = NULL;
        while (paramIndex < itemCount) {
	  paramItem = 
	    (ParameterItemPtr) arrayListGet(getParamItemList(), paramIndex++);
	  if (paramItem != NULL) 
	    notifyXsldbgApp(XSLDBG_MSG_PARAMETER_CHANGED, paramItem);
        }
    }
#endif

    if (printParamList())
      result = 1;
    else
        xsltGenericError(xsltGenericErrorContext,
                         "Error in printing parameters\n");
      xsltGenericError(xsltGenericErrorContext,"\n");

    return result;
}
