
/***************************************************************************
                          help.c  -  description
                             -------------------
    begin                : Sun Sep 16 2001
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

#include "config.h"
#include "xsldbg.h"
#include "help.h"
#include <stdlib.h>


#ifdef WITH_DEBUG_HELP
void
helpTop(const xmlChar * args)
{
    char buff[300], helpParam[100];

    /* Handle the differnces in path and quote character between
     * win32 and Unix/Linux systems */
#ifdef WIN32
    char quoteChar = ' ';
    char pathChar = '\\';
#else
    char quoteChar = '\"';
    char pathChar = '/';
#endif

/* try to make finding help files user friendly by using HELP_DOC_PATH macro*/

/* for non win32 environments see the macro in xsldebugger/Makefile.am
   Win32 tupe systems see  macro in libxslt/xsltwin32config.h
*/
#define USE_HELP_MACRO

#ifndef USE_HELP_MACRO
    char *debugDirPath = getenv("XSL_DEBUG_DIR");
#else
    char *debugDirPath = HELP_DOC_PATH;
#endif

    if (strlen(args) > 0) {
        snprintf(helpParam, 100, "--param help %c'%s'%c", quoteChar, args,
                 quoteChar);
    } else
        strcpy(helpParam, "");
    if (debugDirPath && snprintf((char *) buff, 299, "xsldbg %s"
                                 " --param xsldbg_version %c'%s'%c "
                                 " %s%cxsldoc.xsl %s%cxsldoc.xml | more",
                                 helpParam,
                                 quoteChar, VERSION, quoteChar,
                                 debugDirPath, pathChar,
                                 debugDirPath, pathChar) <= 290) {
        if (system(buff)) {
            if (debugDirPath)
                fprintf(stderr,
                        "Help failed : Maybe help files not found in %s or "
                        "xsldbg not found in path\n", debugDirPath);
            else
                fprintf(stderr, "Unable to find xsldbg or help files\n");
            fprintf(stderr, "Used command: %s\n", buff);
        }

    } else {
        if (!debugDirPath)
#ifdef USE_HELP_MACRO
            fprintf(stderr,
                    "Error in seting for USE_HELP_MACRO look at Makefile.am\n");
#else
            fprintf(stderr,
                    "Required environment variable XSL_DEBUG_DIR not set "
                    "to the directory of xsltproc documentation\n");
#endif
        else
            fprintf(stderr, "Help error: Argument %s too long\n", args);
    }
    /*      fprintf(stderr, "Used command: %s\n",buff); */
}

#else

void
helpTop(const xmlChar * args ATTRIBUTE_UNUSED)
{
    /* Add more help here if normal help system will not work on some systems
     */
    printf("Available commands are \n (bye|exit|quit) "
           ", step, stepout, cont, run, break, showbreak, clear "
           ", validate, load, save, write, free, cd, pwd, du, ls, dir,"
           " where, globals, locals, "
           " cat source data \n"
           "For all bellow commands NAME is a valid template name\n"
           "To find template quickly use : cd -tNAME \n"
           "To create a template break point use  :break (*|NAME) "
           "supply a valid template name "
           " or \"*\" to match any template \n"
           "To display list of template breaks use : showbreak \n"
           "To clear a template break use : clear NAME \n"
           "To step to next xsl instruction use : step \n");
}

#endif
