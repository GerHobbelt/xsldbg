
/***************************************************************************
    begin                : Sat Dec 22 2001
    copyright            : (C) 2004-2018 by Keith Isdale
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


#ifndef QTNOTIFIER_H
#define QTNOTIFIER_H

#if defined WIN32
#include <libxsldbg/xsldbgwin32config.h>
#endif

#include "xsldbgnotifier.h"


class  XsldbgDebuggerBase;

void connectNotifier(XsldbgDebuggerBase *debugger);
int qtNotifyXsldbgApp(XsldbgMessageEnum type, const void *data);
int qtNotifyStateXsldbgApp(XsldbgMessageEnum type, int commandId, XsldbgCommandStateEnum commandState, const char *text);
int qtNotifyTextXsldbgApp(XsldbgMessageEnum type, const char *text);

#endif
