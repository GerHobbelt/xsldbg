
/***************************************************************************
                          help.c  -  help system for win32 platform
                             -------------------
    begin                : Tue Jan 29 2002
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

/*
 * Uses docs/xsldoc.xsl docs/xsldoc.xml and xslproc to generate text
 */

#ifdef VERSION
#undef VERSION
#endif

#include "xsldbg.h"
#include "options.h"
#include "debugXSL.h"
#include "help.h"
#include <stdlib.h>

#ifdef WITH_DEBUG_HELP

/**
 * helpTop:
 * @args : Is valid command or empty string
 *
 * Display help about the command in @args
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
helpTop(const xmlChar * args)
{
    char buff[500], helpParam[100];

    const char *docsDirPath =
        (const char *) getStringOption(OPTIONS_DOCS_PATH);
    int result = 0;

#ifdef __riscos
    docsDirPath = unixfilename(docsDirPath);
#endif

    if (xmlStrLen(args) > 0) {
        snprintf(helpParam, 100, "--param help %c'%s'%c", QUOTECHAR, args,
                 QUOTECHAR);
    } else
        xmlStrCpy(helpParam, "");
    if (docsDirPath) {
        snprintf((char *) buff, sizeof(buff), "%s %s"
                 " --param xsldbg_version %c'%s'%c "
                 " %s%cxsldoc.xsl %s%cxsldoc.xml | more",
                 XSLDBG_BIN, helpParam,
                 QUOTECHAR, VERSION, QUOTECHAR,
                 docsDirPath, PATHCHAR, docsDirPath, PATHCHAR);
        if (xslDbgShellExecute((xmlChar *) buff, 1) == 0) {
            if (docsDirPath)
                xsltGenericError(xsltGenericErrorContext,
                                 "Help failed : Maybe help files not found in %s or "
                                 "xsldbg not found in path\n",
                                 docsDirPath);
            /* FIXME: Comments not correct - the command is that invoked  */
            else
                xsltGenericError(xsltGenericErrorContext,
                                 "Unable to find xsldbg or help files\n");
        } else
            result++;

    } else {
        xsltGenericError(xsltGenericErrorContext,
                         "No path to documentation aborting help\n");
#ifdef USE_DOCS_MACRO
        xsltGenericError(xsltGenericErrorContext,
                         "Error in value of USE_DOC_MACRO look at Makefile.am\n");
#else
        xsltGenericError(xsltGenericErrorContext,
                         "Required environment variable %s is not set "
                         "to the directory of xsldbg documentation\n",
                         XSLDBG_DOCS_DIR_VARIABLE);
#endif
    }
    return result;
}

#else

/**
 * helpTop:
 * @args : Is valid command or empty string
 *
 * Display help about the command in @args
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
helpTop(const xmlChar * args ATTRIBUTE_UNUSED)
{
    int result = 0;
    xmlChar buff[500];
    char *docsDirPath = (char *) getStringOption(OPTIONS_DOCS_PATH);

    if (docsDirPath) {
        snprintf((char *) buff, sizeof(buff), "more %sxsldoc.txt",
                 docsDirPath);
        if (xslDbgShellExecute(buff, 1) == 0) {
            /* JRF: docsDirPath can't be NULL 'cos it's checked above */
            xsltGenericError(xsltGenericErrorContext,
                             "Help failed : Maybe help files not found in %s or "
                             "more not found in path\n", docsDirPath);
        } else
            result++;
    } else {
        xsltGenericError(xsltGenericErrorContext,
                         "No path to documentation aborting help\n");
#ifdef USE_DOCS_MACRO
        xsltGenericError(xsltGenericErrorContext,
                         "Error in value of USE_DOCS_MACRO look at Makefile.am\n");
#else
        xsltGenericError(xsltGenericErrorContext,
                         "Required environment variable %s not set "
                         "to the directory of xsldbg documentation\n",
                         XSLDBG_DOCS_DIR_VARIABLE);
#endif
    }
    return result;
}

#endif
