
/***************************************************************************
                          options.h  -  define option related functions
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

#ifndef OPTIONS_H
#define OPTIONS_H

#include <breakpoint/arraylist.h>
#include <libxslt/xslt.h>

/* ---------------------------------------  
        Misc parsing related options
-------------------------------------------*/

enum Option_type {
    OPTIONS_XINCLUDE = 200,
    OPTIONS_DOCBOOK,
    OPTIONS_TIMING,
    OPTIONS_PROFILING,
    OPTIONS_NOVALID,
    OPTIONS_NOOUT,
    OPTIONS_HTML,
    OPTIONS_DEBUG,
    OPTIONS_SHELL,
    OPTIONS_REPEAT,
    OPTIONS_TRACE,              /* trace the execution */
    OPTIONS_WALK_SPEED,         /* how fast do we walk through code */
    OPTIONS_VERBOSE,            /* do we print out messages/debuging info */

    /* string options */
    OPTIONS_OUTPUT_FILE_NAME = 220,
    OPTIONS_SOURCE_FILE_NAME,
    OPTIONS_DATA_FILE_NAME
};

/* define what tracing is used */
enum {
    TRACE_OFF,
    TRACE_ON,
    TRACE_RUNNING,
    TRACE_FINISHED              /* not need but just in case */
};

enum {
    WALKSPEED_0,
    WALKSPEED_STOP = WALKSPEED_0,
    WALKSPEED_1,
    WALKSPEED_FAST = WALKSPEED_1,
    WALKSPEED_2,
    WALKSPEED_3,
    WALKSPEED_4,
    WALKSPEED_5,
    WALKSPEED_NORMAL = WALKSPEED_5,
    WALKSPEED_6,
    WALKSPEED_7,
    WALKSPEED_8,
    WALKSPEED_9,
    WALKSPEED_SLOW = WALKSPEED_9
};

/* how many microseconds is each speed increase worth*/
#define WALKDELAY 250000

/* used to keep track of libxslt paramters 
 see Parameter related option near end of file
*/
typedef struct _ParameterItem ParameterItem;
typedef ParameterItem *ParameterItemPtr;
struct _ParameterItem {
    xmlChar *name, *value;
};


/** 
 * optionsInit:
 *
 * Allocate memory needed by options data structures
 * Returns 1 on success,
 *         0 otherwise
 */
int optionsInit(void);


/**
 * optionsFree:
 *
 * Free memory used by options data structures
 */
void optionsFree(void);

/**
 * enableOption :
 * @type : option type to change
 * @value : 1 to enable, 0 otherwise
 *
 * Set the state of a xsldbg option to @value
 * Returns 1 on success,
 *         0 otherwise
 */
int enableOption(enum Option_type option_type, int value);


/**
 * isEnabled :
 * @type : option type to query
 *
 * retrieve the state of a xsldbg option
 */
int isOptionEnabled(enum Option_type option_type);


/**
 * setIntOption :
 * @type : option type to change
 * @value : valid to adopt
 *
 * Set the value of a xsldbg option to @value
 * Returns 1 on success,
 *         0 otherwise
 */
int setIntOption(enum Option_type option_type, int value);



/**
 * getIntOption :
 * @type : option type to query
 *
 * retrieve the state of a xsldbg option
 */
int getIntOption(enum Option_type option_type);



/**
 * setStringOption:
 * @type : option type to change
 * @ value : value to copy
 *
 * Set value for a string xsldbg option to @value. If value is NULL
 * Then the memory for option @type is freed
 * Returns 1 on success,
 *         0 otherwise
 */
int setStringOption(enum Option_type option_type, const xmlChar * value);


/**
 * getStringOption:
 * @type : option type to change
 *
 * Get value for a string xsldbg option of @type
 * Returns current option value which may be NULL
 */
const xmlChar *getStringOption(enum Option_type option_type);



/* ---------------------------------------------
          Parameter related options 
-------------------------------------------------*/

/**
 * getParamItemList:
 *
 * Return the list of parameters to provide to libxslt when doing 
 *   stylesheet transformation
 */
ArrayListPtr getParamItemList(void);



/**
 * newParamItem:
 * @name : valid 
 * @value : value
 *
 * Create a new libxslt parameter item
 * Returns non-null if sucessful
 *         0 otherwise
 */
ParameterItemPtr paramItemNew(const xmlChar * name, const xmlChar * value);


/**
 * freeParamItem:
 * @item : valid
 *
 * free memory used by libxslt parameter item
 */
void paramItemFree(ParameterItemPtr item);

/**
 * printParam:
 * @paramId: 0 =< paramID < arrayListCount(getParamList())
 * 
 * Print parameter information
 * Returns 1 on success,
 *         0 otherwise
 */
int printParam(int paramId);

/**
 * printParamList:
 *
 * Prints all items in parameter list
 * Returns 1 on success,
 *         0 otherwise
 */
int printParamList(void);

#endif
