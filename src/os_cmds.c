
/***************************************************************************
                          os_cmds.c  - operating system commands for xsldbg
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

/* -----------------------------------------

   Operating system related commands

  ------------------------------------------- */


/**
 * xslDbgShellChangeWd:
 * @path: The path to change to
 *
 * Change the current working directory of the operating system
 *
 * Returns 1 if able to change xsldbg's working directory to @path
 *         0 otherwise
 */
int
xslDbgShellChangeWd(const xmlChar * path)
{
    int result = 0;

    if (xmlStrLen(path)) {
        /* call function in files.c to do the work */
        result = changeDir(path);
    } else
        xsltGenericError(xsltGenericErrorContext,
                         "Missing path name after chdir command\n");
    return result;
}


/**
 * xslDbgShellExecute:
 * @name: The name of command string to be executed by operating system shell
 * @verbose: If 1 then print extra debugging messages,
 *            normal messages otherwise
 * 
 * Execute an operating system command
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
xslDbgShellExecute(xmlChar * name, int verbose)
{
    int result = 0;

    if (verbose)
        xsltGenericError(xsltGenericErrorContext,
                         "Starting shell command \"%s\"\n", name);
    if (!system((char *) name)) {
        if (verbose)
            xsltGenericError(xsltGenericErrorContext,
                             "\nFinished shell command\n");
        result++;
    } else {
        if (verbose)
            xsltGenericError(xsltGenericErrorContext,
                             "\nUnable to run command\n");
    }
    return result;
}
