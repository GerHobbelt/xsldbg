/***************************************************************************
                          options.c  -  provide the implementation for option
                                           related functions
                             -------------------
    begin                : Sat Nov 10 2001
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

#include "xsldbg.h"
#include "options.h"
#include "arraylist.h"

/* Enable the use of xinclude during file parsing*/
int xinclude = 0;

/* Enable the use of docbook sgml file parsing */
int docbook = 0;

/* Enable the use of timing during file parsing/execution */
int timing = 0;

/* Enable the use of profiling during file execution */
int profile = 0;

/* Disables validation when parsing files */
int novalid = 0;

/* Disables output to stdout */
int noout = 0;

/* Enable the use of html parsing */
int html = 0;

/* Enable the use of debuging */
int debug = 0;

/* Set the number of time to repeat */
int repeat = 0;

/* Enable the use of debugger shell */
int shell = 0 ;


/* keep track of our string options */
xmlChar *stringOptions[ OPTIONS_DATA_FILE_NAME - OPTIONS_OUTPUT_FILE_NAME + 1];

/* keep track of our parameters */
ArrayListPtr parameterList;

/** 
 * optionsInit:
 *
 * Allocate memory needed by options data structures
 * Returns 1 on success,
 *         0 otherwise
 */
int optionsInit(){
  int index;
  for (index = 0; 
       index <= OPTIONS_DATA_FILE_NAME - OPTIONS_OUTPUT_FILE_NAME; 
       index++){
    stringOptions[index] = NULL;
  }

  setStringOption(OPTIONS_ROOT_TEMPLATE_NAME, "/");
  /* init our parameter list*/
  parameterList = xslArrayListNew(10, (freeItemFunc)paramItemFree); 
}


/**
 * optionsFree:
 *
 * Free memory used by options data structures
 */
void optionsFree(){
  int string_option;
  for (string_option = OPTIONS_OUTPUT_FILE_NAME; 
       string_option <= OPTIONS_DATA_FILE_NAME; 
       string_option++){
    setStringOption(string_option, NULL);
  }  

  /* Free up memory used by parameters*/
  xslArrayListFree(parameterList);
}

/**
 * enableOption :
 * @type : option type to change
 * @value : 1 to enable, 0 otherwise
 *
 * Set the state of a xsldbg option to @value
 * Returns 1 on success,
 *         0 otherwise
 */

int enableOption(enum Option_type option_type, int value){
  int type = option_type, result = 1;
  switch(type){
  case  OPTIONS_XINCLUDE:
    xinclude = value;
    break;

  case OPTIONS_DOCBOOK:  
    docbook = value;
    break;

  case OPTIONS_TIMING:  
    timing = value;
    break;

  case OPTIONS_PROFILING:
    profile = value;
    break;

  case OPTIONS_NOVALID:
    novalid = value;
    break;
    
  case OPTIONS_NOOUT:
    noout = value;
    break;

  case OPTIONS_HTML:
    html = value;
    break;

  case OPTIONS_DEBUG:
    debug = value;
    break;

  case OPTIONS_SHELL:
    shell = value;
    break;

  case OPTIONS_REPEAT:
    repeat = value;
    break;

  default:
      xsltGenericError(xsltGenericErrorContext,
		       "Not a valid boolean xsldbg option %d\n", type);
      result = 0;
  }
  return result;
}

/**
 * isEnabled :
 * @type : option type to query
 *
 * retrieve the state of a xsldbg option
 */
int isOptionEnabled(enum Option_type option_type){
  int type = option_type, result = 0;
  switch(type){
  case  OPTIONS_XINCLUDE:
    result = xinclude;
    break;

  case OPTIONS_DOCBOOK:  
    result = docbook;
    break;

  case OPTIONS_TIMING:  
    result = timing;
    break;

  case OPTIONS_PROFILING:
    result = profile;
    break;

  case OPTIONS_NOVALID:
    return novalid;
    break;
    
  case OPTIONS_NOOUT:
    return noout;
    break;

  case OPTIONS_HTML:
    return html;
    break;

  case OPTIONS_DEBUG:
    return debug;
    break;

  case OPTIONS_SHELL:
    result = shell;    
    break;

  case OPTIONS_REPEAT:
    result = repeat;
    break;
    
  default:
      xsltGenericError(xsltGenericErrorContext,
		       "Not a valid boolean xsldbg option %d\n", type);
  }
  return result;
}


/**
 * setIntOption :
 * @type : option type to change
 * @value : valid to adopt
 *
 * Set the value of a xsldbg option to @value
 * Returns 1 on success,
 *         0 otherwise
 */
int setIntOption(enum Option_type option_type, int value){
  return enableOption(option_type, value);
}


/**
 * getIntOption :
 * @type : option type to query
 *
 * retrieve the state of a xsldbg option
 */
int getIntOption(enum Option_type option_type) {
  return isOptionEnabled(option_type);
}

/**
 * setStringOption:
 * @type : option type to change
 * @ value : value to copy
 *
 * Set value for a string xsldbg option to @value. If value is NULL
 * then the memory for option @type is freed
 * Returns 1 on success,
 *         0 otherwise
 */
int setStringOption(enum Option_type option_type, const char *value){
  int result = 0;
  if ((option_type >= OPTIONS_OUTPUT_FILE_NAME) &&
      (option_type <= OPTIONS_DATA_FILE_NAME)){
    int index = option_type - OPTIONS_OUTPUT_FILE_NAME;
    if (stringOptions[index])
      xmlFree(stringOptions[index]);
    if (value)
      stringOptions[index]= xmlMemStrdup(value);
    else
      stringOptions[index] = NULL;
    result++;
  }else
      xsltGenericError(xsltGenericErrorContext,
		       "Not a valid string xsldbg option %d\n", option_type);
  return result;
}

/**
 * getStringOption:
 * @type : option type to change
 *
 * Get value for a string xsldbg option of @type
 * Returns current option value which may be NULL
 */
const char *getStringOption(enum Option_type option_type){
  xmlChar *result = NULL;
  if ((option_type >= OPTIONS_OUTPUT_FILE_NAME) &&
      (option_type <= OPTIONS_DATA_FILE_NAME)){
    result = stringOptions[option_type - OPTIONS_OUTPUT_FILE_NAME];
  }else
      xsltGenericError(xsltGenericErrorContext,
		       "Not a valid string xsldbg option %d\n", option_type);
  return result;
}


/**
 * newParamItem:
 * @name : valid 
 * @value : value
 *
 * Create a new Parameter Item
 * Returns non-null if sucessful
 *         0 otherwise
 */
ParameterItemPtr paramItemNew(const xmlChar *name, const xmlChar *value){
  ParameterItemPtr result = NULL;
  if (name && value){
    result = (ParameterItem*)xmlMalloc(sizeof(ParameterItem));
    if (result){
      result->name = xmlMemStrdup(name);
      result->value = xmlMemStrdup(value);
    }
  }
  return result;
}



/**
 * freeParamItem:
 * @item : valid
 *
 * free memory used by parameter item
 */
void paramItemFree(ParameterItemPtr item){
  if (item){
    if (item->name)
      xmlFree(item->name);
    if (item->value)
      xmlFree(item->value);
  }
}

/**
 * getParamItemList:
 *
 * Return the list of parameters to provide to libxslt when doing 
 *   stylesheet transformation
 */
ArrayListPtr getParamItemList(){
  return parameterList;
}
