
/***************************************************************************
                          help_unix.cpp  - unix specific code provide user help text
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
#include <QCoreApplication>
#include <QDebug>
#include <QFile>

int helpTop(const xmlChar * args)
{

    //Extra phrases to support translation of help display
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
    QString xsldbg_bin(QLatin1String(XSLDBG_BIN));

    char buff[500], helpParam[100];

    QByteArray docsDirPath(QFile::encodeName(optionsGetStringOption(OPTIONS_DOCS_PATH)));
    int result = 0;
    // Do not require xsldbg to be in %PATH%
    xsldbg_bin = QCoreApplication::applicationDirPath() + PATHCHAR + xsldbg_bin;

    if (xmlStrLen(args) > 0) {
        snprintf(helpParam, 100, "--param help:%c'%s'%c", QUOTECHAR, args,
                 QUOTECHAR);
    } else
        xmlStrCpy(helpParam, "");
    if (!docsDirPath.isEmpty() && !filesTempFileName(0).isEmpty()) {
        snprintf((char *) buff, sizeof(buff), "%s %s"
                 " --param xsldbg_version:%c'%s'%c "
                 " --param xsldbgVerTxt:%c'%s'%c "
                 " --param helpDocVerTxt:%c'%s'%c "
                 " --param helpErrorTxt:%c'%s'%c "
                 " --output %c%s%c "
                 " --noautoloadconfig "
                 " --cd %c%s%c "
		 "xsldoc.xsl xsldoc.xml",
                 xsldbg_bin.toLocal8Bit().constData(), helpParam,
                 QUOTECHAR, XSLDBG_VERSION , QUOTECHAR,
                 QUOTECHAR, xsldbgVerTxt.toLocal8Bit().constData(), QUOTECHAR,
                 QUOTECHAR, helpDocVerTxt.toLocal8Bit().constData(), QUOTECHAR,
                 QUOTECHAR, helpErrorTxt.toLocal8Bit().constData(), QUOTECHAR,
                 QUOTECHAR, filesTempFileName(0).constData(), QUOTECHAR,
		 QUOTECHAR, docsDirPath.constData(),QUOTECHAR);
        if (xslDbgShellExecute((xmlChar *) buff, optionsGetIntOption(OPTIONS_VERBOSE)) == 0) {
            if (!docsDirPath.isEmpty())
                xsldbgGenericErrorFunc(QObject::tr("Error: Unable to display help. Help files not found in"
                                            " %1 or xsldbg not found in path.\n").arg(docsDirPath.constData())); 
            else
                xsldbgGenericErrorFunc(QObject::tr("Error: Unable to find xsldbg or help files.\n"));
        } else {
            if (filesMoreFile((xmlChar*)filesTempFileName(0).constData(), NULL) == 1) {
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

