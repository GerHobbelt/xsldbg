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

#include <libxsldbg/xsldbgmsg.h>
#include <libxml/xmlmemory.h>
#include <libxml/tree.h>

#ifdef __cplusplus
extern "C" {
#endif


  int getInputStatus(void);

  void setInputStatus(XsldbgMessageEnum type);

  int getThreadStatus(void);

  void setThreadStatus(XsldbgMessageEnum type);

  int xsldbgThreadInit(void);

  void * xsldbgThreadMain(void *data);

  int xsldbgMain(int argc, char **argv);


  void xsldbgThreadFree(void);

  /* thread has died so cleanup after it */
  void xsldbgThreadCleanup(void);

  const char *getFakeInput(void);

  int fakeInput(const char *text);

  /* Is input ready yet */
  int getInputReady(void);

  /* set/clear flag that indicates if input is ready*/ 
  void setInputReady(int value);

  xmlChar *xslDbgShellReadline(xmlChar * prompt);


  /* This is implemented by xsldbg.c */
/**
 * xsldbgFree:
 *
 * Free memory used by xsldbg
 */
void xsldbgFree(void);


  /**
   * xsldbgLineNo:
   *
   * What line number are we at 
   *
   * Returns The current line number of xsldbg, may be -1
   **/
  int xsldbgLineNo(void);


  /**
   * xsldbgUrl:
   * 
   * What URL did we stop at
   *
   * Returns A NEW copy of URL stopped at. Caller must free memory for URL,   
   *         may be NULL
   */
  xmlChar *xsldbgUrl(void);

#ifdef __cplusplus
}
#endif

#endif
