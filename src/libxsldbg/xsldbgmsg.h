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
   XSLDBG_MSG_THREAD_INIT,	  /* 1: The xsldbg thread is initializing */
   XSLDBG_MSG_THREAD_RUN,	 /* 2: The xsldbg thread is running */
   XSLDBG_MSG_THREAD_STOP,	 /* 3: The xsldbg thread is abou to die */
   XSLDBG_MSG_THREAD_DEAD,       /* 4: The xsldbg thread died */

	 /* input status ( once thread is running) */		
   XSLDBG_MSG_AWAITING_INPUT,	 /* 5: Waiting for user input */
   XSLDBG_MSG_READ_INPUT,	 /* 6: Read user input */
   XSLDBG_MSG_PROCESSING_INPUT,  /* 7: Processing user's request*/

	 /* provide more informatiom about state of xsldbg (optional)*/	
   XSLDBG_MSG_PROCESSING_RESULT,  /* 8: An error occured performing
				    												requested command */
   XSLDBG_MSG_LINE_CHANGED,	     /* 9: Changed to new line number ie a step */
   XSLDBG_MSG_FILE_CHANGED,      /* 10: Loaded source/data file */
   XSLDBG_MSG_BREAKPOINT_CHANGED, /* 11: Response to a showbreak command */	
   XSLDBG_MSG_PARAMETER_CHANGED,   /* 12: Response to showparam command */
   XSLDBG_MSG_TEXTOUT,              /* 13 : Free form text from xsldg */
   XSLDBG_MSG_FILEOUT,              /* 14 : Free form text in file from xsldg */
   XSLDBG_MSG_LOCALVAR_CHANGED,   /* 15 : Local variable */
   XSLDBG_MSG_GLOBALVAR_CHANGED,   /* 16 : Global variable */  
   XSLDBG_MSG_TEMPLATE_CHANGED,    /* 17 : template details*/
   XSLDBG_MSG_SOURCE_CHANGED,       /* 18 : a normal stylesheet */ 
   XSLDBG_MSG_INCLUDED_SOURCE_CHANGED,  /* 19: xmlNodeptr of a included stylesheet */
   XSLDBG_MSG_CALLSTACK_CHANGED         /* 20: a item on th call stack */
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
 * XSLDBG_MSG_THREAD_INIT,		    not used
 * XSLDBG_MSG_THREAD_RUN,		    not used
 * XSLDBG_MSG_THREAD_STOP,		    not used
 * XSLDBG_MSG_THREAD_DEAD,    	     	    not used
 * XSLDBG_MSG_AWAITING_INPUT, 		    not used
 * XSLDBG_MSG_READ_INPUT,		A volitile value of the char* for user input
 * XSLDBG_MSG_PROCESSING_INPUT,  	    not used
 * XSLDBG_MSG_PROCESSING_RESULT,      A volatile value of type xsldbgErrorMsgPtr
 * XSLDBG_MSG_LINE_CHANGED            Is non-NULL if reached breakpoint otherwise 
 *                                         just change in line number of displayed source/data
 * XSLDBG_MSG_FILE_CHANGED,      		not used
 * XSLDBG_MSG_BREAKPOINT_CHANGED      A volatile xslBreakPointPtr of the break point
 *	                                    changed. If NULL then one or more break points have
 *	                                    changed
 * XSLDBG_MSG_PARAMETER_CHANGED     A volatile ParameterItemPtr of libxslt pameter that changed.
 *                                   If NULL then one or more break points have changed
 * XSLDBG_MSG_TEXTOUT               A char * to buffer for text output 
 * XSLDBG_MSG_FILEOUT               A FILE * for text to output
 * XSLDBG_MSG_LOCALVAR_CHANGED,     A local variable of type xsltStackElemPtr
 * XSLDBG_MSG_GLOBALVAR_CHANGED,    A global variable of type xsltStackElemPtr
 * XSLDBG_MSG_TEMPLATE_CHANGED,     A template of type xsltTemplatePtr
 * XSLDBG_MSG_SOURCE_CHANGED,           A xsltStylesheetPtr of a normal stylesheet
 * XSLDBG_MSG_INCLUDED_SOURCE_CHANGED  A xmlNodePtr of a included stylsheet 
 * XSLDBG_MSG_CALLSTACK_CHANGED        A xslCallPointPtr of a call stack item
 *
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
