
/**
 *
 *  This file is part of the kdewebdev package
 *  Copyright (c) 2002 Keith Isdale <keith@kdewebdev.org>
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


/*
 * Uses docs/xsldoc.xsl docs/xsldoc.xml and xslproc to generate text
 */

#include "xsldbg.h"
//##TODO
//#include "version.h"
#include "options.h"
#include "utils.h"
#include "debugXSL.h"
#include "help.h"
#include "files.h"
#include <stdlib.h>

int helpTop(const xmlChar * args)
{

    //Extra phrases to support translation of help display see kdewebdev/doc/xsldbg/xsldbghelp.xml and kdewebdev/kxsldbg/xsldbghelp.xsl
    static const char* xsldbghelp_translations[] =
    {
	I18N_NOOP("xsldbg version"),
	I18N_NOOP("Help document version"),
	I18N_NOOP("Help not found for command")
    };
    Q_UNUSED(xsldbghelp_translations)

    QString xsldbgVerTxt(QObject::tr("xsldbg version"));
    QString helpDocVerTxt(QObject::tr("Help document version"));
    QString helpErrorTxt(QObject::tr("Help not found for command"));
    

    char buff[500], helpParam[100];

    QByteArray docsDirPath(optionsGetStringOption(OPTIONS_DOCS_PATH).toUtf8().constData());
    int result = 0;

    if (xmlStrLen(args) > 0) {
        snprintf(helpParam, 100, "--param help:%c'%s'%c", QUOTECHAR, args,
                 QUOTECHAR);
    } else
        xmlStrCpy(helpParam, "");
    if (!docsDirPath.isEmpty() && filesTempFileName(0)) {
        snprintf((char *) buff, sizeof(buff), "%s %s"
                 " --param xsldbg_version:%c'%s'%c "
                 " --param xsldbgVerTxt:%c'%s'%c "
                 " --param helpDocVerTxt:%c'%s'%c "
                 " --param helpErrorTxt:%c'%s'%c "
                 " --output %s "
                 " --noautoloadconfig "
                 " --cd %s "
                 "xsldbghelp.xsl xsldbghelp.xml",
                 XSLDBG_BIN, helpParam,
                 QUOTECHAR, KXSLDBG_VERSION , QUOTECHAR,
                 QUOTECHAR, xsldbgVerTxt.toUtf8().constData(), QUOTECHAR,
                 QUOTECHAR, helpDocVerTxt.toUtf8().constData(), QUOTECHAR,
                 QUOTECHAR, helpErrorTxt.toUtf8().constData(), QUOTECHAR,
                 filesTempFileName(0),
		 docsDirPath.constData());
        if (xslDbgShellExecute((xmlChar *) buff, optionsGetIntOption(OPTIONS_VERBOSE)) == 0) {
            if (!docsDirPath.isEmpty())
                xsldbgGenericErrorFunc(QObject::tr("Error: Unable to display help. Help files not found in"
                                            " %1 or xsldbg not found in path.\n").arg(docsDirPath.constData())); 
            else
                xsldbgGenericErrorFunc(QObject::tr("Error: Unable to find xsldbg or help files.\n"));
        } else {
            if (filesMoreFile((xmlChar*)filesTempFileName(0), NULL) == 1) {
                result = 1;
            } else {
                xsldbgGenericErrorFunc(QObject::tr("Error: Unable to print help file.\n"));
            }
        }

    } else {
        xsldbgGenericErrorFunc(QObject::tr("Error: No path to documentation; aborting help.\n"));
#ifdef WITH_XSLDBG_DEBUG_PROCESS
#ifdef USE_DOCS_MACRO
        xsltGenericError(xsltGenericErrorContext,"MACRO has been defined look at Makefile.am\n");
#else
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Environment variable %s is not set to the directory of xsldbg documentation.\n",
                         XSLDBG_DOCS_DIR_VARIABLE);
#endif
#endif
    }
    return result;
}

