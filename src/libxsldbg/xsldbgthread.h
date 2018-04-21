
/***************************************************************************
    begin                : Sat Dec 22 2001
    copyright            : (C) 2001-2018 by Keith Isdale
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
