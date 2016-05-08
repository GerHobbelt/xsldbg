
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


#include <QApplication>

#include "../libxsldbg/xsldbgevent.h"
#include "../libxsldbg/qtnotifier2.h"
#include "xsldbgdebuggerbase.h"
#include "../libxsldbg/xsldbgthread.h"

XsldbgDebuggerBase *_debugger = 0L;

int qtNotifyXsldbgApp(XsldbgMessageEnum type, const void *data)
{
    int result = 0;
    if (::getThreadStatus() == XSLDBG_MSG_THREAD_NOTUSED){
      return 1;
    }

    /* clear the input ready flag as quickly as possible*/
     if ( ::getInputStatus() == XSLDBG_MSG_READ_INPUT)
     	  ::setInputReady(0);

     /* state of the thread */
     if (_debugger != 0L){
       XsldbgEvent *e = new XsldbgEvent(type, data);
       if (e != 0L) {
	 /* The application will now have this event in its event queue
	    that all that is needed from here*/
	 QApplication::postEvent(_debugger, e);
       }
     }

    if (::getThreadStatus() == XSLDBG_MSG_THREAD_STOP)
        ::xsldbgThreadCleanup();        /* thread has died so cleanup after it */

    result++; /* at the moment this function will always work */

    return result;
}



void connectNotifier(XsldbgDebuggerBase *debugger){
  _debugger = debugger;
}


