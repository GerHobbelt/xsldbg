
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

/**
 * Provide a mechanism to change option. The Options structure is not in use,
 *   it has been added so that kdoc puts all option related 
 *   functions together 
 *
 * @short file support
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */

#include "arraylist.h"

#ifdef __cplusplus
extern "C" {
#endif

    /* not used, Keep kdoc happy for the moment :) */
    typedef struct _Options Options;
    typedef Options *OptionsPtr;
    struct _Options {
        int unused_variable;
    }

/* used to keep track of libxslt paramters 
 see Parameter related option functions near end of file*/
 typedef struct _ParameterItem ParameterItem;
    typedef ParameterItem *ParameterItemPtr;
    struct _ParameterItem {
        xmlChar *name;
        xmlChar *value;
    };

    /* what options are available */
    enum OptionTypeEnum {
        OPTIONS_XINCLUDE = 500, /* Use xinclude during xml parsing */
        OPTIONS_DOCBOOK,        /* Use of docbook sgml parsing */
        OPTIONS_TIMING,         /* Use of timing */
        OPTIONS_PROFILING,      /* Use of profiling */
        OPTIONS_NOVALID,        /* Disable file validation */
        OPTIONS_NOOUT,          /* Disables output to stdout */
        OPTIONS_HTML,           /* Enable the use of html parsing */
        OPTIONS_DEBUG,          /* Enable the use of xml tree debugging */
        OPTIONS_SHELL,          /* Enable the use of debugger shell */
        OPTIONS_GDB,            /* Run in gdb modem prints more messages) */
        OPTIONS_REPEAT,         /* The number of times to repeat */
        OPTIONS_TRACE,          /* Trace execution */
        OPTIONS_WALK_SPEED,     /* How fast do we walk through code */
        OPTIONS_VERBOSE,        /* Be verbose with messages */
        OPTIONS_OUTPUT_FILE_NAME = 550, /* what is the output file name */
        OPTIONS_SOURCE_FILE_NAME,       /*  the stylesheet source to use */
        OPTIONS_DOCS_PATH,      /* path of xsldbg's documentation */
        OPTIONS_DATA_FILE_NAME  /* xml data file to use */
    };

    /* define what tracing is used */
    enum TraceModeEnum {
        TRACE_OFF = 600,        /* disable tracing */
        TRACE_ON,               /* enable tracing */
        TRACE_RUNNING,          /* tracing is running */
        TRACE_FINISHED          /* not needed but just in case */
    };

    /* what speeds can we walk through a stylesheet */
    /* must start walkSpeed enums from zero !! */
    enum WalkSpeedEnum {
        WALKSPEED_0 = 0,
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


/* ---------------------------------------  
        Misc parsing related options
-------------------------------------------*/


/* how many microseconds is each speed increase worth */
#define WALKDELAY 250000


/* for non win32 environments see the macro in xsldebugger/Makefile.am
   Win32 tupe systems see  macro in libxslt/xsltwin32config.h
*/
#ifndef __riscos                /* JRF: Under RISC OS we'll use the sysvar */
#define USE_DOCS_MACRO
#endif


/** 
 * Allocate memory needed by options data structures
 *
 * @returns 1 on success,
 *          0 otherwise
 */
    int optionsInit(void);


/**
 * Free memory used by options data structures
 */
    void optionsFree(void);


/**
 * Set the state of a xsldbg option to @p value
 *
 * @returns 1 on success,
 *         0 otherwise
 *
 * @param optionType Is a valid integer option
 * @param value 1 to enable, 0 otherwise
 */
    int enableOption(OptionTypeEnum optionType, int value);


/**
 * @returns The state of a xsldbg option. ie 1 for enabled, 0 for disabled
 *
 * @param optionType Is a valid integer option to query

 */
    int isOptionEnabled(OptionTypeEnum optionType);


/**
 * Set the value of a xsldbg option to @p value
 *
 * @returns 1 on success,
 *         0 otherwise
 *
 * @param optionType Is a valid integer option
 * @param value Is the valid to adopt
 */
    int setIntOption(OptionTypeEnum optionType, int value);


/**
 * @returns The state of a xsldbg option
 *
 * @param optionType Is a valid integer option
 */
    int getIntOption(OptionTypeEnum optionType);


/**
 * Set value for a string xsldbg option to @p value. 
 * Any memory used currently by option @p optionType will be freed
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param optionType A valid string option
 * @param value The value to copy
 */
    int setStringOption(OptionTypeEnum optionType, const xmlChar * value);


/**
 * Get value for a string xsldbg option of @p optionType
 *
 * @returns current option value which may be NULL
 *
 * @param optionType A valid string option 

 */
    const xmlChar *getStringOption(OptionTypeEnum optionType);



/* ---------------------------------------------
          Parameter related options 
------------------------------------------------- */

/**
 * @returns The list of parameters to provide to libxslt when doing 
 *              stylesheet transformation if successful
 *          NULL otherwise
 */
    ArrayListPtr getParamItemList(void);


/**
 * Create a new libxslt parameter item
 *
 * @returns non-null if sucessful
 *          NULL otherwise
 *
 * @param name Is valid 
 * @param value Is valid 
 */
    ParameterItemPtr paramItemNew(const xmlChar * name,
                                  const xmlChar * value);


/**
 * Free memory used by libxslt parameter item @p item
 *
 * @param item Is valid
 */
    void paramItemFree(ParameterItemPtr item);


/**
 * Print parameter information
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param paramId: 0 =< @p paramID < arrayListCount(getParamList())
 */
    int printParam(int paramId);


/**
 * Prints all items in parameter list
 *
 * @returns 1 on success,
 *          0 otherwise
 */
    int printParamList(void);

#ifdef __cplusplus
}
#endif
#endif
