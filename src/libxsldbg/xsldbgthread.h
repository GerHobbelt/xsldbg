
/***************************************************************************
                          xsldbgthread.h  -  description
                             -------------------
    begin                : Thu Dec 20 2001
    copyright            : (C) 2001 by keith
    email                : keith@linux
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
#include "libqtnotifier_so.h"
#endif


#ifndef BUILD_DOCS
#include <libxsldbg/xsldbgmsg.h>
#include <libxml/xmlmemory.h>
#include <libxml/tree.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


    int getAppReady(void);

    void setAppReady(int ready);

    int getInputStatus(void);

    void setInputStatus(XsldbgMessageEnum type);

    int getThreadStatus(void);

    void setThreadStatus(XsldbgMessageEnum type);

    void *xsldbgThreadMain(void *data);

    int xsldbgMain(int argc, char **argv);

    XSLDBG_SO_API int xsldbgThreadInit(void);

    XSLDBG_SO_API void xsldbgThreadFree(void);

    /* thread has died so cleanup after it */
    XSLDBG_SO_API void xsldbgThreadCleanup(void);

    XSLDBG_SO_API const char *getFakeInput(void);

    XSLDBG_SO_API int fakeInput(const char *text);

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


#ifdef __cplusplus
}
#endif
#endif
