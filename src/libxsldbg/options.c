
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
#include "xsldbgthread.h"
#include "options.h"
#include "arraylist.h"
#include "xsldbgmsg.h"


/* keep track of our integer/boolean options */
static int intOptions[OPTIONS_VERBOSE - OPTIONS_XINCLUDE + 1];

/* make use that use of options are safe by only copying
   critical values from intVolitleOptions just before stylesheet is started
 */
int intVolitileOptions[OPTIONS_VERBOSE - OPTIONS_XINCLUDE + 1];

/* keep track of our string options */
static xmlChar *stringOptions[OPTIONS_DATA_FILE_NAME -
                              OPTIONS_OUTPUT_FILE_NAME + 1];

/* keep track of our parameters */
static ArrayListPtr parameterList;

/** 
 * optionsInit:
 *
 * Intialize the options module
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
optionsInit(void)
{
    int optionId;
    const xmlChar *docsPath = NULL;

/* for non win32 environments see the macro in xsldebugger/Makefile.am
   Win32 tupe systems see  macro in libxslt/xsltwin32config.h
   For definition of USE_DOCS_MACRO see options.h */

#ifdef USE_DOCS_MACRO
    docsPath = (xmlChar *) DOCS_PATH;
#else
    docsPath = (xmlChar *) getenv(XSLDBG_DOCS_DIR_VARIABLE);
    if (!docsPath) {
        xsltGenericError(xsltGenericErrorContext,
                         "Warning no value for documentation specified in environment variable %s. "
                         "No help nor search results will display\n",
                         XSLDBG_DOCS_DIR_VARIABLE);
    }
#endif


    for (optionId = 0;
         optionId <= OPTIONS_VERBOSE - OPTIONS_XINCLUDE; optionId++) {
        intOptions[optionId] = 0;
        intVolitileOptions[optionId] = 0;
    }

    for (optionId = 0;
         optionId <= OPTIONS_DATA_FILE_NAME - OPTIONS_OUTPUT_FILE_NAME;
         optionId++) {
        stringOptions[optionId] = NULL;
    }

    /* init our parameter list */
    parameterList = arrayListNew(10, (freeItemFunc) paramItemFree);

    /* setup the docs path */
    setStringOption(OPTIONS_DOCS_PATH, docsPath);
    
    setIntOption(OPTIONS_TRACE, TRACE_OFF);
    setIntOption(OPTIONS_WALK_SPEED, WALKSPEED_STOP);

    /* set output default as standard output. Must be changed if not using
     * xsldbg's command line. Or the tty command is used */
    setStringOption(OPTIONS_OUTPUT_FILE_NAME, NULL);
    return (parameterList != NULL);
}


/**
 * optionsFree:
 *
 * Free memory used by options data structures
 */
void
optionsFree(void)
{
    int string_option;

    for (string_option = OPTIONS_OUTPUT_FILE_NAME;
         string_option <= OPTIONS_DATA_FILE_NAME; string_option++) {
        setStringOption(string_option, NULL);
    }

    /* Free up memory used by parameters */
    arrayListFree(parameterList);
    parameterList = NULL;
}


/**
 * enableOption:
 * @optionType: A valid boolean option
 * @value: 1 to enable, 0 otherwise
 *
 * Set the state of a boolean xsldbg option to @value
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
enableOption(OptionTypeEnum optionType, int value)
{
    int type = optionType, result = 1;

    switch (type) {
        case OPTIONS_XINCLUDE:
        case OPTIONS_DOCBOOK:
        case OPTIONS_TIMING:
        case OPTIONS_PROFILING:
        case OPTIONS_NOVALID:
        case OPTIONS_NOOUT:
        case OPTIONS_HTML:
        case OPTIONS_DEBUG:
        case OPTIONS_SHELL:
        case OPTIONS_GDB:
        case OPTIONS_REPEAT:
        case OPTIONS_CATALOGS:
    case OPTIONS_PREFER_HTML:
        case OPTIONS_VERBOSE:
            /* make sure that use of options are safe by only copying
             * critical values from intVolitleOptions just before 
             * stylesheet is started
             */
            intVolitileOptions[type - OPTIONS_XINCLUDE] = value;
            result++;
            break;

        case OPTIONS_TRACE:
        case OPTIONS_WALK_SPEED:
            intVolitileOptions[type - OPTIONS_XINCLUDE] = value;
            intOptions[type - OPTIONS_XINCLUDE] = value;
            result++;
            break;

        default:
            xsltGenericError(xsltGenericErrorContext,
                             "Not a valid boolean xsldbg option %d\n",
                             type);
            result = 0;
    }
    return result;
}


/**
 * isOptionEnabled:
 * @optionType: A valid boolean option to query
 *
 * Return the state of a boolean option
 *
 * Returns The state of a boolean xsldbg option. 
 *         ie 1 for enabled , 0 for disabled
 */
int
isOptionEnabled(OptionTypeEnum optionType)
{
    int type = optionType, result = 0;

    switch (type) {
        case OPTIONS_XINCLUDE:
        case OPTIONS_DOCBOOK:
        case OPTIONS_TIMING:
        case OPTIONS_PROFILING:
        case OPTIONS_NOVALID:
        case OPTIONS_NOOUT:
        case OPTIONS_HTML:
        case OPTIONS_DEBUG:
        case OPTIONS_SHELL:
        case OPTIONS_GDB:
        case OPTIONS_REPEAT:
        case OPTIONS_TRACE:
        case OPTIONS_VERBOSE:
        case OPTIONS_CATALOGS:
    case OPTIONS_PREFER_HTML:
        case OPTIONS_WALK_SPEED:
            result = intOptions[type - OPTIONS_XINCLUDE];
            break;

        default:
            xsltGenericError(xsltGenericErrorContext,
                             "Not a valid boolean xsldbg option %d\n",
                             type);
    }
    return result;
}


/**
 * setIntOption:
 * @optionType: Is a valid integer option
 * @value: Value to adopt
 *
 * Set the value of an integer xsldbg option to @value
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
setIntOption(OptionTypeEnum optionType, int value)
{
    return enableOption(optionType, value);
}


/**
 * getIntOption:
 * @optionType: A valid integer option
 *
 * Return the state of an integer option
 *
 * Returns The state of a integer xsldbg option
 */
int
getIntOption(OptionTypeEnum optionType)
{
    return isOptionEnabled(optionType);
}


/**
 * setStringOption:
 * @optionType: A valid string option
 * @value: The Value to copy
 *
 * Set value for a string xsldbg option to @value. 
 * Any memory used currently by option @optionType will be freed
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
setStringOption(OptionTypeEnum optionType, const xmlChar * value)
{
    int result = 0;

    if ((optionType >= OPTIONS_OUTPUT_FILE_NAME) &&
        (optionType <= OPTIONS_DATA_FILE_NAME)) {
        int optionId = optionType - OPTIONS_OUTPUT_FILE_NAME;

        if (stringOptions[optionId])
            xmlFree(stringOptions[optionId]);
        if (value)
            stringOptions[optionId] =
                (xmlChar *) xmlMemStrdup((char *) value);
        else
            stringOptions[optionId] = NULL;
        result++;
    } else
        xsltGenericError(xsltGenericErrorContext,
                         "Not a valid string xsldbg option %d\n",
                         optionType);
    return result;
}


/**
 * getStringOption:
 * @optionType: A valid string option 
 *
 * Get value for a string xsldbg option of @optionType

 * Returns current option value which may be NULL
 */
const xmlChar *
getStringOption(OptionTypeEnum optionType)
{
    xmlChar *result = NULL;

    if ((optionType >= OPTIONS_OUTPUT_FILE_NAME) &&
        (optionType <= OPTIONS_DATA_FILE_NAME)) {
        result = stringOptions[optionType - OPTIONS_OUTPUT_FILE_NAME];
    } else
        xsltGenericError(xsltGenericErrorContext,
                         "Not a valid string xsldbg option %d\n",
                         optionType);
    return result;
}


  /**
   * copyVolitleOptions:
   *
   * Copy volitile options to the working area for xsldbg
   */
void
copyVolitleOptions(void)
{
    int optionId;

    for (optionId = 0;
         optionId < OPTIONS_VERBOSE - OPTIONS_XINCLUDE; optionId++) {
        intOptions[optionId] = intVolitileOptions[optionId];
    }
}

/**
 * paramItemNew:
 * @name: Is valid 
 * @value: Is valid 
 *
 * Create a new libxslt parameter item
 * Returns non-null if sucessful
 *         NULL otherwise
 */
ParameterItemPtr
paramItemNew(const xmlChar * name, const xmlChar * value)
{
    ParameterItemPtr result = NULL;

    if (name && value) {
        result = (ParameterItem *) xmlMalloc(sizeof(ParameterItem));
        if (result) {
            result->name = (xmlChar *) xmlMemStrdup((char *) name);
            result->value = (xmlChar *) xmlMemStrdup((char *) value);
        }
    }
    return result;
}


/**
 * paramItemFree:
 * @item: Is valid
 *
 * Free memory used by libxslt parameter item @item
 */
void
paramItemFree(ParameterItemPtr item)
{
    if (item) {
        if (item->name)
            xmlFree(item->name);
        if (item->value)
            xmlFree(item->value);
    }
}


/**
 * getParamItemList:
 *
 * Return the list of libxlt parameters
 *
 * Returns The list of parameters to provide to libxslt when doing 
 *           stylesheet transformation if successful
 *        NULL otherwise
 */
ArrayListPtr
getParamItemList(void)
{
    return parameterList;
}


/**
 * printParam:
 * @paramId: 0 =< paramID < arrayListCount(getParamList())
 * 
 * Print parameter information
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
printParam(int paramId)
{
    int result = 0;
    ParameterItemPtr paramItem =
        (ParameterItemPtr) arrayListGet(getParamItemList(), paramId);
    if (paramItem && paramItem->name && paramItem->value) {
        xsltGenericError(xsltGenericErrorContext,
                         " Parameter %d %s=\"%s\"\n", paramId,
                         paramItem->name, paramItem->value);
        result++;
    }
    return result;
}


/**
 * printParamList:
 *
 * Prints all items in parameter list
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
printParamList(void)
{
    int result = 1;
    int paramIndex = 0;
    int itemCount = arrayListCount(getParamItemList());

    if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
        if (itemCount > 0) {
            while (result && (paramIndex < itemCount)) {
                result = printParam(paramIndex++);
            }
        }
    } else {
        if (itemCount > 0) {
            xsltGenericError(xsltGenericErrorContext, "\n");
            while (result && (paramIndex < itemCount)) {
                result = printParam(paramIndex++);
            }
        } else
            xsltGenericError(xsltGenericErrorContext,
                             "\nNo parameters present\n");
    }
    return result;
}
