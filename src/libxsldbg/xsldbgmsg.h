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
   XSLDBG_MSG_THREAD_NOTUSED,    /* 0:  Thread are not to be used*/
   XSLDBG_MSG_THREAD_INIT,		   /* 1: The xsldbg thread is initializing */
   XSLDBG_MSG_THREAD_RUN,		     /* 2: The xsldbg thread is running */
   XSLDBG_MSG_THREAD_STOP,		   /* 3: The xsldbg thread is abou to die */
   XSLDBG_MSG_THREAD_DEAD,       /* 4: The xsldbg thread died */

	 /* input status ( once thread is running) */		
   XSLDBG_MSG_AWAITING_INPUT,		 /* 5: Waiting for user input */
   XSLDBG_MSG_READ_INPUT,			   /* 6: Read user input */
   XSLDBG_MSG_PROCESSING_INPUT,  /* 7: Processing user's request*/

	 /* provide more informatiom about state of xsldbg (optional)*/	
   XSLDBG_MSG_PROCESSING_RESULT,  /* 8: An error occured performing
				    												requested command */
   XSLDBG_MSG_LINE_CHANGED,	     /* 9: Changed to new line number ie a step */
   XSLDBG_MSG_FILE_CHANGED,      /* 10: Loaded source/data file */
   XSLDBG_MSG_BREAKPOINT_CHANGED, /* 11: Response to a showbreak command */	
   XSLDBG_MSG_PARAMETER_CHANGED   /* 12: Response to showparam command */
} XsldbgMessageEnum;


typedef enum {
	 XSLDBG_COMMAND_FAILED,		/* generic error */
	 XSLDBG_COMMAND_WARNING,        
	 XSLDBG_COMMAND_INFO,
	 XSLDBG_COMMAND_NOTUSED
} XsldbgCommandStateEnum;



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
 * XSLDBG_MSG_PROCESSING_RESULT,      A volatile value of type xsldbgErrorMsgPtr
 * XSLDBG_MSG_FILE_CHANGED,      	     			not used
 * XSLDBG_MSG_BREAKPOINT_CHANGED      A volatile xslBreakPointPtr of the break point
 *	                                    changed. If NULL then one or more break points have
 *	                                    changed
 *  XSLDBG_MSG_PARAMETER_CHANGED     A volatile ParameterItemPtr of libxslt pameter that changed.
 *                                    If NULL then one or more break points have changed
 *
 * Legend :
 *	        not used  :- value may be NULL but must not be used
 *          volatile  :- value is only guaranteed to be valid for the life the call to
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
  XsldbgCommandStateEnum commandState;
  xmlChar *text;
  xmlChar *messagefileName; /* used when send large chunks of data  */
}; 

#ifdef __cplusplus
extern "C" {
#endif

 int notifyXsldbgApp(XsldbgMessageEnum type, const void *data);

 int notifyStateXsldbgApp(XsldbgMessageEnum type, int commandId, 
			  XsldbgCommandStateEnum commandState, const char *text);

 int notifyTextXsldbgApp(XsldbgMessageEnum type, const char *text);

#ifdef __cplusplus
}
#endif


#endif
