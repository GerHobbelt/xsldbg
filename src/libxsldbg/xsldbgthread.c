/***************************************************************************
                          xsldbgthread.c  -  basic thread support
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


#include "config.h"
#include <libxslt/xsltutils.h>

#include <libxsldbg/xsldbgmsg.h>
#include <libxsldbg/xsldbgthread.h>

static int threadStatus = XSLDBG_MSG_THREAD_NOTUSED;
static int inputStatus = XSLDBG_MSG_AWAITING_INPUT;

static int inputReady = 0;



/* the compiler will optimize this function to inline and to keep variable private*/
int
getInputStatus(void)
{
    return inputStatus;
}

void setInputStatus(XsldbgMessageEnum type)
{
  switch(type)
    {
  	 case XSLDBG_MSG_AWAITING_INPUT:		 /* Waiting for user input */
   	 case XSLDBG_MSG_READ_INPUT:			   /* Read user input */
	 	 case XSLDBG_MSG_PROCESSING_INPUT:  /* Processing user's request*/
			inputStatus = type;
		break;

    default:
      printf("Invalid input status %d\n", type);
    }
}


/* the compiler will optimize this function to inline and to keep variable private*/
int
getThreadStatus(void)
{
    return threadStatus;
}

/* reset the status to @p type */
void setThreadStatus(XsldbgMessageEnum type)
{
  switch(type)
    {
    case XSLDBG_MSG_THREAD_NOTUSED:
    case XSLDBG_MSG_THREAD_INIT:
    case XSLDBG_MSG_THREAD_RUN:
    case XSLDBG_MSG_THREAD_STOP:
    case XSLDBG_MSG_THREAD_DEAD:
	threadStatus = type;
	break;

    default:
      printf("Invalid thread status %d\n",type);
  }
}


/* Is input ready yet */
int getInputReady()
{
  return inputReady;
}

/* set/clear flag that indicates if input is ready*/ 
void setInputReady(int value)
{
  inputReady = value;
}



