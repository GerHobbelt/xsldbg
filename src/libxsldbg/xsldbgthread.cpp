
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


#include "xsldbgthread.h"
#include "xsldbgmsg.h"

#include "xsldbg.h"
#include "breakpoint.h"
#include "options.h"

static void (*cleanupFuncPtr)(void) = 0;
static int threadStatus = XSLDBG_MSG_THREAD_NOTUSED;
static int inputStatus = XSLDBG_MSG_AWAITING_INPUT;

/* is xsldbg ready for input from the application */
static int inputReady = 0;

/* Is the application ready for a notification message */
static int appReady = 0;

static notifyMessageListPtr notifyList;

arrayListPtr msgList = NULL;

int getAppReady(void)
{
    return appReady;
}

void setAppReady(int ready)
{
    appReady = ready;
}


/* the compiler will optimize this function to inline and to keep variable private*/
int getInputStatus(void)
{
    return inputStatus;
}

void setInputStatus(XsldbgMessageEnum type)
{
    switch (type) {
        case XSLDBG_MSG_AWAITING_INPUT:        /* Waiting for user input */
        case XSLDBG_MSG_READ_INPUT:    /* Read user input */
        case XSLDBG_MSG_PROCESSING_INPUT:      /* Processing user's request */
            inputStatus = type;
            break;

        default:
            printf("Invalid input status %d\n", type);
    }
}


/* the compiler will optimize this function to inline and to keep variable private*/
int getThreadStatus(void)
{
    return threadStatus;
}

/* reset the status to @p type */
void setThreadStatus(XsldbgMessageEnum type)
{
    switch (type) {
        case XSLDBG_MSG_THREAD_NOTUSED:
        case XSLDBG_MSG_THREAD_INIT:
        case XSLDBG_MSG_THREAD_RUN:
            threadStatus = type;
            break;

        case XSLDBG_MSG_THREAD_STOP:
        case XSLDBG_MSG_THREAD_DEAD:
            xslDebugStatus = DEBUG_QUIT;
            threadStatus = type;
            break;

        default:
            printf("Invalid thread status %d\n", type);
    }
}


/* Is input ready yet */
int getInputReady(void)
{
    return inputReady;
}

/* set/clear flag that indicates if input is ready*/
void setInputReady(int value)
{
    inputReady = value;
}



int notifyListStart(XsldbgMessageEnum type)
{
    int result = 0;

    switch (type) {
        case XSLDBG_MSG_INTOPTION_CHANGE:
        case XSLDBG_MSG_STRINGOPTION_CHANGE:
            break;

        default:
            msgList = arrayListNew(10, NULL);
    }

    notifyList =
        (notifyMessageListPtr) xmlMalloc(sizeof(notifyMessageList));
    if (notifyList && msgList) {
        notifyList->type = type;
        notifyList->list = msgList;
        result = 1;
    }

    return result;
}

int notifyListQueue(const void *data)
{
    int result = 0;

    if (msgList) {
        arrayListAdd(msgList, (void *) data);
        result = 1;
    }
    return result;
}


int notifyListSend(void)
{
    int result = 0;

    if (notifyList && msgList) {
        notifyXsldbgApp(XSLDBG_MSG_LIST, notifyList);
        result = 1;
    }
    return result;
}

void xsldbgSetThreadCleanupFunc(void (*cleanupFunc)(void))
{
    cleanupFuncPtr = cleanupFunc;
}

void xsldbgThreadCleanup(void)
{
    if (cleanupFuncPtr != 0)
	(cleanupFuncPtr)();
}