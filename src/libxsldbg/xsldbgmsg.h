/***************************************************************************
                          xsldbgmsg.h  -  description
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

 #ifndef XSLDBGMSG_H
 #define XSLDBGMSG_H

#include <libxml/tree.h> /* needed for the definition of xmlChar */

 typedef enum {
	 /* thread status */
   XSLDBG_MSG_THREAD_NOTUSED,               /* Thread are not to be used*/
   XSLDBG_MSG_THREAD_INIT,		   /* The xsldbg thread is initializing */
   XSLDBG_MSG_THREAD_RUN,		     /* The xsldbg thread is running */
   XSLDBG_MSG_THREAD_STOP,		   /* The xsldbg thread is abou to die */
   XSLDBG_MSG_THREAD_DEAD,       /* The xsldbg thread died */

	 /* input status ( once thread is running) */		
   XSLDBG_MSG_AWAITING_INPUT,		 /* Waiting for user input */
   XSLDBG_MSG_READ_INPUT,			   /* Read user input */
   XSLDBG_MSG_PROCESSING_INPUT,  /* Processing user's request*/

	 /* provide more informatiom about state of xsldbg (optional)*/	
   XSLDBG_MSG_PROCESSING_RESULT,  /* An error occured performing 
				    												requested command */
   XSLDBG_MSG_LINE_CHANGED,	     /* Changed to new line number ie a step */
   XSLDBG_MSG_FILE_CHANGED,      /* Loaded source/data file */
   XSLDBG_MSG_BREAKPOINT_CHANGED /* Added, deleted or modified a break point*/	
} XsldbgMessageEnum;


/**
 * Notify the KPart that something happened to the xsldbg thread
 *
 * @param type : A valid XsldbgMessageEnum
 *
 * @param data : The meaning of data can have a different meaning for each value of @type
 *<pre>
 *   Value of @type                       Meaning of @data
 *    --------------------------  +++ ---------------------
 * XSLDBG_MSG_THREAD_NOTUSED,               not used
 * XSLDBG_MSG_THREAD_INIT,		     					not used
 * XSLDBG_MSG_THREAD_RUN,		     						not used
 * XSLDBG_MSG_THREAD_STOP,		     					not used
 * XSLDBG_MSG_THREAD_DEAD,       	     			not used
 * XSLDBG_MSG_AWAITING_INPUT,		     				not used
 * XSLDBG_MSG_READ_INPUT,		 					A volitile value of the char* for user input
 * XSLDBG_MSG_PROCESSING_INPUT,  	    		 not used
 * XSLDBG_MSG_PROCESSING_RESULT,      A volitile value of type xsldbgErrorMsgPtr
 * XSLDBG_MSG_FILE_CHANGED,      	     			not used
 * XSLDBG_MSG_BREAKPOINT_CHANGED      A volitle xsldbgBreakPointPtr of the break point
 *	                                    changed. If NULL then one or more break points have
 *	                                    changed
 *
 * Legend :
 *	        not used  :- value may be NULL but must not be used
 *          volitile  :- value is only guaranteed to be valid for the life the call to
 *                       this function. ie make a NEW copy of value if needed for longer
 *                       than that.
 *																
 *<pre>
 * @returns 1 on sucess
 *          0 otherwise
*/

typedef struct _xsldbgErrorMsg xsldbgErrorMsg;
typedef xsldbgErrorMsg *xsldbgErrorMsgPtr;
struct _xsldbgErrorMsg {
  XsldbgMessageEnum type;
  int commandId;
  int commandState;
  xmlChar *text;
}; 

#ifdef __cplusplus
extern "C" {
#endif

 int notifyXsldbgApp(XsldbgMessageEnum type, const void *data);

 int notifyStateXsldbgApp(XsldbgMessageEnum type, int commandId, 
			  int commandState, const char *text);

 int notifyTextXsldbgApp(XsldbgMessageEnum type, const char *text);

#ifdef __cplusplus
}
#endif


#endif
