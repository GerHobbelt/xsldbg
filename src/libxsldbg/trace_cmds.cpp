
/**
 *
 *  This file is part of the kdewebdev package
 *  Copyright (c) 2001 Keith Isdale <keith@kdewebdev.org>
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


#include "xsldbg.h"
#include "debugXSL.h"
#include "options.h"


/* -----------------------------------------

   Tracing related commands

  ------------------------------------------- */


int xslDbgShellTrace(xmlChar * arg)
{
    Q_UNUSED(arg);
    xslDebugStatus = DEBUG_RUN_RESTART;
    optionsSetIntOption(OPTIONS_TRACE, TRACE_ON);
    return 1;
}


int xslDbgShellWalk(xmlChar * arg)
{
    int result = 0;

    long speed = WALKSPEED_NORMAL;

    if (xmlStrLen(arg)
        && (!sscanf((char *) arg, "%ld", &speed) || ((speed < 0) || (speed > 9)))) {
        xsldbgGenericErrorFunc(QObject::tr("Error: Invalid arguments to command %1.\n").arg(QString("walk")));
        xsldbgGenericErrorFunc(QObject::tr("Warning: Assuming normal speed.\n"));
        speed = WALKSPEED_NORMAL;
    }
    result = 1;
    optionsSetIntOption(OPTIONS_WALK_SPEED, speed);
    xslDebugStatus = DEBUG_WALK;

    return result;
}
