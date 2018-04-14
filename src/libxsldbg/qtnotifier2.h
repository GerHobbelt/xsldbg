
/**
 *
 *  This file is part of the kdewebdev package
 *  Copyright (c) 2001 Keith Isdale <keithisdale@gmail.com>
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
