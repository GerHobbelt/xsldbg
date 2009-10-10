
/***************************************************************************
                          main.c  - main fiule for xsldbg
                             -------------------
    begin                : Sat Dec 22 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : keith@kdewebdev.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <libxsldbg/xsldbg.h>
#include <libxsldbg/xsldbgmsg.h>
#include <libxsldbg/xsldbgio.h>
#include <libxslt/xsltutils.h>
#include <QCoreApplication>


int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    app.setApplicationName(QObject::tr("xsldbg"));
	app.setOrganizationDomain("xsldbg");
    app.setApplicationVersion(QLatin1String(XSLDBG_VERSION));
    return xsldbgMain(argc, argv);
}



