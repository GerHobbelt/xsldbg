
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
#include "options.h"
#include "help.h"
#include <stdlib.h>


#ifdef WITH_DEBUG_HELP
void
helpTop(const xmlChar * args)
{
    char buff[300], helpParam[100];

    char *docsDirPath = getStringOption(OPTIONS_DOCS_PATH);


    if (strlen(args) > 0) {
        snprintf(helpParam, 100, "--param help %c'%s'%c", QUOTECHAR, args,
                 QUOTECHAR);
    } else
        strcpy(helpParam, "");
    if (docsDirPath && snprintf((char *) buff, 299, "xsldbg %s"
                                 " --param xsldbg_version %c'%s'%c "
                                 " %s%cxsldoc.xsl %s%cxsldoc.xml | more",
                                 helpParam,
                                 QUOTECHAR, VERSION, QUOTECHAR,
                                 docsDirPath, PATHCHAR,
                                 docsDirPath, PATHCHAR) <= 290) {
        if (system(buff)) {
            if (docsDirPath)
                xsltGenericError(xsltGenericErrorContext,
                        "Help failed : Maybe help files not found in %s or "
                        "xsldbg not found in path\n", docsDirPath);
            else
                xsltGenericError(xsltGenericErrorContext, 
				 "Unable to find xsldbg or help files\n");
            xsltGenericError(xsltGenericErrorContext, 
			     "Used command: %s\n", buff);
        }

    } else {
        if (!docsDirPath)
#ifdef USE_DOCS_MACRO
            xsltGenericError(xsltGenericErrorContext,
                    "Error in seting for USE_HELP_MACRO look at Makefile.am\n");
#else
            xsltGenericError(xsltGenericErrorContext,
                    "Required environment variable XSLDBG_DOCS_DIR not set "
                    "to the directory of xsltproc documentation\n");
#endif
        else
            xsltGenericError(xsltGenericErrorContext, 
			     "Help error: Argument %s too long\n", args);
    }
}

#else

void
helpTop(const xmlChar * args ATTRIBUTE_UNUSED)
{
    /* Add more help here if normal help system will not work on some systems
     */
    xsltGenericError(xsltGenericErrorContext,"Available commands are \n (bye|exit|quit) "
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
