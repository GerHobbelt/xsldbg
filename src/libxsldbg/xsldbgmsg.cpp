
/**
 *
 *  This file is part of the kdewebdev package
 *  Copyright (c) 2004 Keith Isdale <keithisdale@gmail.com>
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


#include "xsldbgmsg.h"
#include "xsldbgio.h"
#include "options.h"

#include "xsldbg.h"

#include <libxslt/xsltutils.h>

#ifdef HAVE_READLINE
#include <readline/readline.h>
#ifdef HAVE_HISTORY
#include <readline/history.h>
#endif
#endif

static int (*notifyXsldbgAppFuncPtr) (XsldbgMessageEnum type, const void *data) = 0;

static int (*notifyStateXsldbgAppFuncPtr)(XsldbgMessageEnum type, int commandId,
                             XsldbgCommandStateEnum commandState,
                             const char *text) = 0;

static int (*notifyTextXsldbgAppFuncPtr)(XsldbgMessageEnum type, const char *text) = 0;
static xmlChar * (*xslDbgShellReadlineFuncPtr)(xmlChar * prompt) = 0;


void xsldbgSetAppFunc(int (*notifyXsldbgAppFunc) (XsldbgMessageEnum type, const void *data))
{
    notifyXsldbgAppFuncPtr = notifyXsldbgAppFunc;
}

void xsldbgSetAppStateFunc(int (*notifyStateXsldbgAppFunc)(XsldbgMessageEnum type, int commandId,
                             XsldbgCommandStateEnum commandState,
                             const char *text))
{
    notifyStateXsldbgAppFuncPtr = notifyStateXsldbgAppFunc;
}


void xsldbgSetTextFunc(int (*notifyTextXsldbgAppFunc)(XsldbgMessageEnum type, const char *text))
{
    notifyTextXsldbgAppFuncPtr = notifyTextXsldbgAppFunc;
}

void xsldbgSetReadlineFunc(xmlChar * (*xslDbgShellReadlineFunc)(xmlChar * prompt))
{
    xslDbgShellReadlineFuncPtr = xslDbgShellReadlineFunc;
}

int notifyXsldbgApp(XsldbgMessageEnum type, const void *data)
{
    if (!notifyXsldbgAppFuncPtr)
	return 1;
    else
	return (notifyXsldbgAppFuncPtr)(type, data);
}

int notifyStateXsldbgApp(XsldbgMessageEnum type, int commandId,
			   XsldbgCommandStateEnum commandState, const char *text)
{
    if (!notifyStateXsldbgAppFuncPtr)
	return 1;
    else
	return (notifyStateXsldbgApp)(type, commandId, commandState, text);
}

int notifyTextXsldbgApp(XsldbgMessageEnum type, const char *text)
{
    if (!notifyTextXsldbgAppFuncPtr)
	return 1;
    else
	return (notifyTextXsldbgAppFuncPtr)(type, text);
}


/* use this function instead of the one that was in debugXSL.c */
xmlChar * xslDbgShellReadline(xmlChar * prompt)
{

    if (!xslDbgShellReadlineFuncPtr)
	return 0;
    else
	return (xslDbgShellReadlineFuncPtr)(prompt);
}

