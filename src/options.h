
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

#include "arraylist.h"
#include <libxslt/xslt.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------------------  
        Misc parsing related options
-------------------------------------------*/

    typedef enum {
        OPTIONS_XINCLUDE = 500, /* pick a unique starting point */
        OPTIONS_DOCBOOK,
        OPTIONS_TIMING,
        OPTIONS_PROFILING,
        OPTIONS_NOVALID,
        OPTIONS_NOOUT,
        OPTIONS_HTML,
        OPTIONS_DEBUG,
        OPTIONS_SHELL,
        OPTIONS_GDB,
        OPTIONS_REPEAT,
        OPTIONS_TRACE,          /* trace the execution */
        OPTIONS_WALK_SPEED,     /* how fast do we walk through code */
        OPTIONS_VERBOSE,        /* do we print out messages/debuging info */

        /* string options */
        OPTIONS_OUTPUT_FILE_NAME = 520,
        OPTIONS_SOURCE_FILE_NAME,
        OPTIONS_DOCS_PATH,
        OPTIONS_DATA_FILE_NAME
    } OptionTypeEnum;



/* define what tracing is used */
    typedef enum {
        TRACE_OFF = 600,
        TRACE_ON,
        TRACE_RUNNING,
        TRACE_FINISHED          /* not needed but just in case */
    } TraceModeEnum;

    /* what speeds can we walk through a stylesheet */
    typedef enum {
        WALKSPEED_0,            /* must start at zero !! */
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
    } WalkSpeedEnum;

/* how many microseconds is each speed increase worth*/
#define WALKDELAY 250000


/* for non win32 environments see the macro in xsldebugger/Makefile.am
   Win32 tupe systems see  macro in libxslt/xsltwin32config.h
*/
#ifndef __riscos                /* JRF: Under RISC OS we'll use the sysvar */
#define USE_DOCS_MACRO
#endif

/* used to keep track of libxslt paramters 
 see Parameter related option near end of file
*/
    typedef struct _ParameterItem ParameterItem;
    typedef ParameterItem *ParameterItemPtr;
    struct _ParameterItem {
        xmlChar *name;
        xmlChar *value;
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
 * @type : valid integer option
 * @value : 1 to enable, 0 otherwise
 *
 * Set the state of a xsldbg option to @value
 * Returns 1 on success,
 *         0 otherwise
 */
    int enableOption(OptionTypeEnum optionType, int value);


/**
 * isOptionEnabled :
 * @type : valid integer option to query
 *
 * Returns the state of a xsldbg option. ie 1 for enabled , 0 for disabled
 */
    int isOptionEnabled(OptionTypeEnum optionType);


/**
 * setIntOption :
 * @type : valid integer option
 * @value : valid to adopt
 *
 * Set the value of a xsldbg option to @value
 * Returns 1 on success,
 *         0 otherwise
 */
    int setIntOption(OptionTypeEnum optionType, int value);


/**
 * getIntOption :
 * @type : valid integer option
 *
 * Returns the state of a xsldbg option
 */
    int getIntOption(OptionTypeEnum optionType);


/**
 * setStringOption:
 * @type : valid string option
 * @ value : value to copy
 *
 * Set value for a string xsldbg option to @value. If value is NULL
 * Then the memory for option @type is freed
 * Returns 1 on success,
 *         0 otherwise
 */
    int setStringOption(OptionTypeEnum optionType, const xmlChar * value);


/**
 * getStringOption:
 * @type : valid string option 
 *
 * Get value for a string xsldbg option of @type
 * Returns current option value which may be NULL
 */
    const xmlChar *getStringOption(OptionTypeEnum optionType);



/* ---------------------------------------------
          Parameter related options 
-------------------------------------------------*/

/**
 * getParamItemList:
 *
 * Returns the list of parameters to provide to libxslt when doing 
 *           stylesheet transformation if successful
 *        NULL otherwise
 */
    ArrayListPtr getParamItemList(void);


/**
 * paramItemNew:
 * @name : is valid 
 * @value : is valid 
 *
 * Create a new libxslt parameter item
 * Returns non-null if sucessful
 *         NULL otherwise
 */
    ParameterItemPtr paramItemNew(const xmlChar * name,
                                  const xmlChar * value);


/**
 * paramItemFree:
 * @item : is valid
 *
 * Free memory used by libxslt parameter item @item
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

#ifdef __cplusplus
}
#endif
#endif
