
/***************************************************************************
                          os_cmds.cpp  - user access to operating sytem
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
#include "files.h"

/* -----------------------------------------

   Operating system related commands

  ------------------------------------------- */


int xslDbgShellChangeWd(xmlChar * path)
{
    int result = 0;
    xmlChar *opts[2];

    if (xmlStrLen(path) && splitString(path, 1, opts)) {
        /* call function in files.c to do the work */
        result = changeDir(xsldbgText(opts[0]));
    } else
        xsldbgGenericErrorFunc(QObject::tr("Error: Missing arguments for the command %1.\n").arg(QString("chdir")));
    return result;
}


int xslDbgShellExecute(xmlChar * name, int verbose)
{
    int result = 0;

    /* Quick check to see if we have a command processor; embedded systems
     * may not have such a thing */
    if (system(NULL) == 0) {
        xsldbgGenericErrorFunc(QObject::tr("Error: No command processor available for shell command \"%1\".\n").arg(xsldbgText(name)));
    } else {
        int return_code;

        if (verbose)
            xsldbgGenericErrorFunc(QObject::tr("Information: Starting shell command \"%1\".\n").arg(xsldbgText(name)));

        return_code = system((char *) name);
        /* JRF: Strictly system returns an implementation defined value -
         * we are interpreting that value here, so we need
         * implementation specific code to handle each case */

#ifdef __riscos
        /* on RISC OS -2 means 'system call failed', otherwise it is the
         * return code from the sub-program */
        if (return_code != -2) {
#else
        if (return_code == 0) {
#endif
            if (verbose)
                xsldbgGenericErrorFunc(QObject::tr("Information: Finished shell command.\n"));
            result = 1;
        } else {
            if (verbose)
                xsldbgGenericErrorFunc(QObject::tr("Error: Unable to run command. System error %1.\n").arg(return_code));
        }
    }
    return result;
}
