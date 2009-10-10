
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


#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <libxml/xmlerror.h>

#include "../libxsldbg/breakpoint.h"
#include "../libxsldbg/xsldbgmsg.h"
#include "../libxsldbg/xsldbgthread.h"
#include "../libxsldbg/qtnotifier2.h"
#include "../libxsldbg/xsldbg.h"
#include <QThread>
#ifdef Q_OS_WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#ifdef HAVE_READLINE
#include <readline/readline.h>
#ifdef HAVE_HISTORY
#include <readline/history.h>
#endif
#endif

#define DEBUG_BUFFER_SIZE 500

static char inputBuffer[DEBUG_BUFFER_SIZE];
static char outputBuffer[DEBUG_BUFFER_SIZE];

FILE *stdoutIO = NULL;


/* -----------------------------------------------
   private functions
 ---------------------------------------------------*/


extern "C" { 
xmlChar * qtXslDbgShellReadline(xmlChar * prompt);

};

static void xsldbgThreadCleanupQt(void);

class XsldbgThread : public QThread
{
public:
    void run();
};

void XsldbgThread::run()
{
    if (getThreadStatus() != XSLDBG_MSG_THREAD_INIT){
       fprintf(stderr, "xsldbg thread is not ready to be started. Or one is already running.\n");
    }

    xsldbgSetThreadCleanupFunc(xsldbgThreadCleanupQt);
    setThreadStatus(XSLDBG_MSG_THREAD_RUN);
    setInputStatus(XSLDBG_MSG_AWAITING_INPUT);

	/* call the "main of xsldbg" found in debugXSL.c */
    xsldbgMain(0,0);

    setInputStatus(XSLDBG_MSG_PROCESSING_INPUT);
    notifyXsldbgApp(XSLDBG_MSG_THREAD_DEAD, NULL);
    setThreadStatus(XSLDBG_MSG_THREAD_DEAD);
}

static XsldbgThread *xsldbgThreadRunner = 0;

/* -----------------------------------------------
   end functions
 ---------------------------------------------------*/

/* setup all application wide items */
int
xsldbgThreadInit(void)
{
    int result = 0;
    xsltSetGenericErrorFunc(0, xsldbgGenericErrorFunc);
    setThreadStatus(XSLDBG_MSG_THREAD_INIT);
    xsldbgSetAppFunc(qtNotifyXsldbgApp);
    xsldbgSetAppStateFunc(qtNotifyStateXsldbgApp); 
    xsldbgSetTextFunc(qtNotifyTextXsldbgApp);
    xsldbgSetReadlineFunc(qtXslDbgShellReadline);
    

    /* create the thread and start it */
    xsldbgThreadRunner = new XsldbgThread();
    xsldbgThreadRunner->start();

    int counter;
    for (counter = 0; counter < 11; counter++){
        if (getThreadStatus() != XSLDBG_MSG_THREAD_INIT)
          break;
		/*guess that it will take at most 2.5 seconds to startup */
#ifdef Q_OS_WIN32
		Sleep(250);
#else
		sleep(250);
#endif
    }
    /* xsldbg should have started by now if it can */
    if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN){
        result++;
    }else
         qWarning("Thread did not start\n");

    return result;
}


/* tell the thread to stop and free that memory !*/
void
xsldbgThreadFree(void)
{
    if (getThreadStatus() != XSLDBG_MSG_THREAD_DEAD)
    {
      setThreadStatus(XSLDBG_MSG_THREAD_STOP);
      /*guess that it will take at most 2.5 seconds to stop */
#ifdef Q_OS_WIN32
		Sleep(250);
#else
		sleep(250);
#endif
    }

	if (getThreadStatus() != XSLDBG_MSG_THREAD_DEAD)
        qWarning("xsldbg's thread did not stop properly killing it anyhow\n");
    delete xsldbgThreadRunner;
    xsldbgThreadRunner = 0;
}

const char *getFakeInput()
{
	return inputBuffer;
}


/* put text into standard input just like we had typed it */
int
fakeInput(const char *text)
{
    int result = 0;

    if (!text || (getInputReady() == 1) || (getThreadStatus() != XSLDBG_MSG_THREAD_RUN))
        return result;

    //    fprintf(stderr, "\nFaking input of \"%s\"\n", text);
    strncpy(inputBuffer, text, sizeof(inputBuffer));
    setInputReady(1);
    result++;
    return result;
}


/* use this function instead of the one that was in debugXSL.c */
/**
 * qtXslDbgShellReadline:
 * @prompt:  the prompt value
 *
 * Read a string
 *
 * Returns a copy of the text inputed or NULL if EOF in stdin found.
 *    The caller is expected to free the returned string.
 */
xmlChar *
qtXslDbgShellReadline(xmlChar * prompt)
{

  const char *inputReadBuff;

  static char last_read[DEBUG_BUFFER_SIZE] = { '\0' };

  if (getThreadStatus() != XSLDBG_MSG_THREAD_RUN)
    {
#ifdef HAVE_READLINE
      xmlChar *line_read;

      /* Get a line from the user. */
      line_read = (xmlChar *) readline((char *) prompt);

      /* If the line has any text in it, save it on the history. */
      if (line_read && *line_read) {
        add_history((char *) line_read);
        strncpy((char*)last_read, (char*)line_read, DEBUG_BUFFER_SIZE - 1);
      } else {
        /* if only <Enter>is pressed then try last saved command line */
        line_read = (xmlChar *) xmlMemStrdup(last_read);
      }
      return (line_read);
#else
      char line_read[DEBUG_BUFFER_SIZE];

      if (prompt != NULL)
        xsltGenericError(xsltGenericErrorContext, "%s", prompt);
      if (!fgets(line_read, DEBUG_BUFFER_SIZE - 1, stdin))
        return (NULL);
      line_read[DEBUG_BUFFER_SIZE - 1] = 0;
      /* if only <Enter>is pressed then try last saved command line */
      if ((strlen(line_read) == 0) || (line_read[0] == '\n')) {
        strcpy(line_read, last_read);
      } else {
        strcpy(last_read, line_read);
      }
      return (xmlChar *) xmlMemStrdup(line_read);
#endif

    }
  else{

    setInputStatus(XSLDBG_MSG_AWAITING_INPUT);
    notifyXsldbgApp(XSLDBG_MSG_AWAITING_INPUT, NULL);

    while (getInputReady() == 0){
#ifdef Q_OS_WIN32
		Sleep(10);
#else
		sleep(10);
#endif
      /* have we been told to die */
      if (getThreadStatus() ==  XSLDBG_MSG_THREAD_STOP){
	xslDebugStatus = DEBUG_QUIT;
	return NULL;
      }
    }

    setInputStatus(XSLDBG_MSG_READ_INPUT);
    inputReadBuff =  getFakeInput();
    if(inputReadBuff){
      notifyXsldbgApp(XSLDBG_MSG_READ_INPUT, inputReadBuff);
      return (xmlChar*)xmlMemStrdup(inputReadBuff);
    }else{
      return NULL;
    }
  }
}


xsldbgErrorMsg msg;
xsldbgErrorMsgPtr  msgPtr = &msg;
xmlChar *msgText = NULL;

int qtNotifyStateXsldbgApp(XsldbgMessageEnum type, int commandId,
			  XsldbgCommandStateEnum commandState, const char *text)
{
  int result = 0;
  msg.type = type;
  msg.commandId = commandId;
  msg.commandState = commandState;
  if (text != NULL)
    {
    msg.text = (xmlChar*)xmlMemStrdup(text);
    if (msg.text == NULL)
      return result; /* out of memory */
    }
   else
     msg.text = NULL;

  notifyXsldbgApp(XSLDBG_MSG_PROCESSING_RESULT, msgPtr);
  if (msg.text != NULL)
      {
	xmlFree(msg.text);
	msg.text = NULL;
      }

  result = 1; 
  return result;
}


int qtNotifyTextXsldbgApp(XsldbgMessageEnum type, const char *text)
{
  return qtNotifyStateXsldbgApp(type, -1, XSLDBG_COMMAND_NOTUSED, text);
}

char mainBuffer[DEBUG_BUFFER_SIZE];


/* this is where the thread get to do all its work */
void *
xsldbgThreadMain(void *)
{
    fprintf(stderr, "xsldbgThreadMain() not used anymore update all of kxsldbg's libraries");
    return NULL;
}



/* thread has died so cleanup after it not called directly but via
 notifyXsldbgApp*/
void
xsldbgThreadCleanupQt(void)
{
    if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN)
      {
	xsldbgThreadFree();
      }
    /* it is safe to modify threadStatus as the thread is now dead */
    setThreadStatus(XSLDBG_MSG_THREAD_DEAD);
}



void *
xsldbgThreadStdoutReader(void *data)
{
  if (!stdoutIO)
    return data;

  while (getThreadStatus() == XSLDBG_MSG_THREAD_RUN){
    if (fgets(outputBuffer, sizeof(outputBuffer -1), stdoutIO)){
#ifdef Q_OS_WIN32
	  Sleep(250);
#else
	  sleep(250);
#endif
      strcat(outputBuffer, "\n");
      notifyTextXsldbgApp(XSLDBG_MSG_TEXTOUT, outputBuffer);
    }else{
      qWarning("Unable to read from stdout from xsldbg\n");
      break;
    }
  }
  return data;
}
