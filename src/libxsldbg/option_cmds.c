/***************************************************************************
                          option_cmds.c  -  implementation for option
                                                 related commands

                             -------------------
    begin                : Fri Feb 1 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "utils.h"
#include "options.h"
#include "xsldbgmsg.h"
#include "xsldbgthread.h"
#include "debugXSL.h"


/**
 * xslDbgShellSetOption:
 * @arg : Is valid, and in the format   <NAME> <VALUE>
 * 
 * Set the value of an option 
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int xslDbgShellSetOption(xmlChar *arg){
  int result = 0;
  if (xmlStrLen(arg) > 0) {
    xmlChar *opts[2];
    long optValue;
    long optID;

    if (splitString(arg, 2, opts) == 2) {
      optID = optionsGetOptionID(opts[0]);
      if (optID >= OPTIONS_XINCLUDE) {
	if (optID <= OPTIONS_VERBOSE) {
	  /* handle setting integer option */
	  if (!sscanf
	      ((char *) opts[1], "%ld", &optValue)) {
	    xsltGenericError
	      (xsltGenericErrorContext,
	       "Error : Unable to parse integer value for option \n");
	  } else {
	    result = optionsSetIntOption(optID, optValue);
	  }
	} else {
	  /* handle setting a string option */
	  result = optionsSetStringOption(optID, opts[1]);

	}
      } else {
	xsltGenericError(xsltGenericErrorContext,
			 "Error: Unknown option name %s\n",
			 opts[0]);
      }
    } else {
      xsltGenericError(xsltGenericErrorContext,
		       "Error: Expected two arguments to setoption command\n");
    }
  } else {
    xsltGenericError(xsltGenericErrorContext,
		     "Error: Expected two arguments to setoption command\n");
  }

  return result;
}



/**
 * xslDbgShellOptions:
 *
 * Prints out values for user options
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int xslDbgShellOptions(void)
{
  int result = 1;
  int optionIndex;
  const xmlChar* optionName, *optionValue;
  /* Print out the integer options and thier values */
  if (getThreadStatus() != XSLDBG_MSG_THREAD_RUN){
    for (optionIndex = OPTIONS_XINCLUDE;
	 optionIndex <= OPTIONS_VERBOSE; optionIndex++) {
      /* skip any non-user options */
      optionName = optionsGetOptionName(optionIndex);
      if (optionName && (optionName[0] != '*')) {
	xsltGenericError(xsltGenericErrorContext,
			 "Option %s = %d\n",
			 optionName, optionsGetIntOption(optionIndex));
			    
      }
    }
    /* Print out the string options and thier values */
    for (optionIndex = OPTIONS_OUTPUT_FILE_NAME;
	 optionIndex <= OPTIONS_DATA_FILE_NAME;
	 optionIndex++) {
      optionName = optionsGetOptionName(optionIndex);
      if (optionName && (optionName[0] != '*') ){
	optionValue =  optionsGetStringOption(optionIndex);
	if (optionValue) {
	  xsltGenericError(xsltGenericErrorContext,
			 "Option %s = \"%s\"\n",
			   optionName, optionValue);
	} else {
	  xsltGenericError(xsltGenericErrorContext,
			   "Option %s = \"\"\n", optionName);

	}
      }

    }
    xsltGenericError(xsltGenericErrorContext, "\n");
  }else{
    /* we are now notifying the application of the value of options */
    parameterItemPtr paramItem;
    notifyListStart(XSLDBG_MSG_INTOPTION_CHANGE);
    /* send the integer options and their values*/
    for (optionIndex = OPTIONS_XINCLUDE;
	 optionIndex <= OPTIONS_VERBOSE; optionIndex++) {
      /* skip any non-user options */
      optionName = optionsGetOptionName(optionIndex);
      if (optionName && (optionName[0] != '*')) {
	paramItem = optionsParamItemNew(optionName, 0L);
	if (!paramItem){
	  notifyListSend(); /* send what ever we've got so far*/
	  return 0; /* out of memory */
	}
	paramItem->intValue =  optionsGetIntOption(optionIndex);
	notifyListQueue(paramItem); /* this will be free later*/
      }
    }

    notifyListSend();
    notifyListStart(XSLDBG_MSG_STRINGOPTION_CHANGE);
    /* Send the string options and thier values */
    for (optionIndex = OPTIONS_OUTPUT_FILE_NAME;
	 optionIndex <= OPTIONS_DATA_FILE_NAME;
	 optionIndex++) {
      optionName = optionsGetOptionName(optionIndex);
      if (optionName && (optionName[0] != '*') ){
	paramItem = 
	  optionsParamItemNew( optionName, 
			       optionsGetStringOption(optionIndex));
	if (!paramItem){
	  notifyListSend(); /* send what ever we've got so far*/
	  return 0; /* out of memory*/
	}else
	  notifyListQueue(paramItem); /* this will be freed later*/  
      }
    }
    notifyListSend();
  }  

  return result;
}
