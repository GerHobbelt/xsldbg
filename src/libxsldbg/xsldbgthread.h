
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


#ifndef XSLDBGTHREAD_H
#define  XSLDBGTHREAD_H

#if defined WIN32
#include <libxsldbg/xsldbgwin32config.h>
#endif


#include <libxml/xmlmemory.h>
#include <libxml/tree.h>

#include "xsldbgmsg.h"


int getAppReady(void);

void setAppReady(int ready);

int getInputStatus(void);

void setInputStatus(XsldbgMessageEnum type);

int getThreadStatus(void);

void setThreadStatus(XsldbgMessageEnum type);

void *xsldbgThreadMain(void *data);

int xsldbgMain(int argc, char **argv);

int xsldbgThreadInit(void);

void xsldbgThreadFree(void);

/* thread has died so cleanup after it */
void xsldbgThreadCleanup(void);
void xsldbgSetThreadCleanupFunc(void (*cleanupFunc)(void));

const char *getFakeInput(void);

int fakeInput(const char *text);

/* Is input ready yet */
int getInputReady(void);

/* set/clear flag that indicates if input is ready */
void setInputReady(int value);

xmlChar *xslDbgShellReadline(xmlChar * prompt);



/* This is implemented by xsldbg.c */

/**
 * xsldbgFree:
 *
 * Free memory used by xsldbg
 */
void xsldbgFree(void);


#endif
