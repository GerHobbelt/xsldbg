
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

#ifdef USE_KDE_DOCS

/**
 * Provide a mechanism to change option. The Options structure is not in use,
 *   it has been added so that kdoc puts all option related 
 *   functions together 
 *
 * @short file support
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */
#endif

/* We want skip most of these includes when building documentation */
#ifndef BUILD_DOCS
#include "arraylist.h"
#include <libxslt/xslt.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------------------  
        Misc options
-------------------------------------------*/

#ifndef USE_KDOC
    typedef enum {
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
        OPTIONS_OUTPUT_FILE_NAME = 520, /* what is the output file name */
        OPTIONS_SOURCE_FILE_NAME,       /*  the stylesheet source to use */
        OPTIONS_DOCS_PATH,      /* path of xsldbg's documentation */
        OPTIONS_DATA_FILE_NAME  /* xml data file to use */
    } OptionTypeEnum;



    /* define what tracing is used */
    typedef enum {
        TRACE_OFF = 600,        /* disable tracing */
        TRACE_ON,               /* enable tracing */
        TRACE_RUNNING,          /* tracing is running */
        TRACE_FINISHED          /* not needed but just in case */
    } TraceModeEnum;

    /* what speeds can we walk through a stylesheet */
    /* must start at zero !! */
    typedef enum {
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
    } WalkSpeedEnum;

#else
    /* keep kdoc happy */
    enum OptionsTypeEnum{
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
    } unusedOpt3;
#endif

/* how many microseconds is each speed increase worth*/
#define WALKDELAY 250000


/* for non win32 environments see the macro in xsldebugger/Makefile.am
   Win32 tupe systems see  macro in libxslt/xsltwin32config.h
*/
#ifndef __riscos                /* JRF: Under RISC OS we'll use the sysvar */
#define USE_DOCS_MACRO
#endif

/* used to keep track of libxslt paramters 
 see Parameter related options near end of file
*/
    typedef struct _ParameterItem ParameterItem;
    typedef ParameterItem *ParameterItemPtr;
    struct _ParameterItem {
      xmlChar *name; /* libxslt parameter name*/ 
      xmlChar *value; /* libxslt parameter value*/
    };



#ifdef USE_GNOME_DOCS
/** 
 * optionsInit:
 *
 * Intialize the options module
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/** 
 * Initialized the options module
 *
 * @returns 1 on success,
 *          0 otherwise
 */
#endif
#endif
    int optionsInit(void);



#ifdef USE_GNOME_DOCS
/**
 * optionsFree:
 *
 * Free memory used by options data structures
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Free memory used by options data structures
 */
#endif
#endif
    void optionsFree(void);



#ifdef USE_GNOME_DOCS
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
#else
#ifdef USE_KDE_DOCS
/**
 * Set the state of a boolean xsldbg option to @p value
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param optionType Is a valid boolean option
 * @param value 1 to enable, 0 otherwise
 */
#endif
#endif
    int enableOption(OptionTypeEnum optionType, int value);



#ifdef USE_GNOME_DOCS
/**
 * isOptionEnabled:
 * @optionType: A valid boolean option to query
 *
 * Return the state of a boolean option
 *
 * Returns The state of a boolean xsldbg option. 
 *         ie 1 for enabled , 0 for disabled
 */
#else
#ifdef USE_KDE_DOCS
/**
 * @returns The state of a boolean xsldbg option. 
 *            ie 1 for enabled, 0 for disabled
 *
 * @param optionType Is a valid boolean option to query

 */
#endif
#endif
    int isOptionEnabled(OptionTypeEnum optionType);



#ifdef USE_GNOME_DOCS
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
#else
#ifdef USE_KDE_DOCS
/**
 * Set the value of an integer xsldbg option to @p value
 *
 * @returns 1 on success,
 *         0 otherwise
 *
 * @param optionType Is a valid integer option
 * @param value Is the valid to adopt
 */
#endif
#endif
    int setIntOption(OptionTypeEnum optionType, int value);



#ifdef USE_GNOME_DOCS
/**
 * getIntOption:
 * @optionType: A valid integer option
 *
 * Return the state of an integer option
 *
 * Returns The state of a integer xsldbg option
 */
#else
#ifdef USE_KDE_DOCS
/**
 * @returns The state of a integer xsldbg option
 *
 * @param optionType Is a valid integer option
 */
#endif
#endif
    int getIntOption(OptionTypeEnum optionType);



#ifdef USE_GNOME_DOCS
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
#else
#ifdef USE_KDE_DOCS
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
#endif
#endif
    int setStringOption(OptionTypeEnum optionType, const xmlChar * value);



#ifdef USE_GNOME_DOCS
/**
 * getStringOption:
 * @optionType: A valid string option 
 *
 * Get value for a string xsldbg option of @optionType

 * Returns current option value which may be NULL
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Get value for a string xsldbg option of @p optionType
 *
 * @returns current option value which may be NULL
 *
 * @param optionType A valid string option 
 */
#endif
#endif
    const xmlChar *getStringOption(OptionTypeEnum optionType);



/* ---------------------------------------------
          Parameter related options 
-------------------------------------------------*/


#ifdef USE_GNOME_DOCS
/**
 * getParamItemList:
 *
 * Return the list of libxlt parameters
 *
 * Returns The list of parameters to provide to libxslt when doing 
 *           stylesheet transformation if successful
 *        NULL otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Return the list of libxlt parameters
 *
 * @returns The list of parameters to provide to libxslt when doing 
 *              stylesheet transformation if successful
 *          NULL otherwise
 */
#endif
#endif
    ArrayListPtr getParamItemList(void);



#ifdef USE_GNOME_DOCS
/**
 * paramItemNew:
 * @name: Is valid 
 * @value: Is valid 
 *
 * Create a new libxslt parameter item
 * Returns non-null if sucessful
 *         NULL otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Create a new libxslt parameter item
 *
 * @returns non-null if sucessful
 *          NULL otherwise
 *
 * @param name Is valid 
 * @param value Is valid 
 */
#endif
#endif
    ParameterItemPtr paramItemNew(const xmlChar * name,
                                  const xmlChar * value);



#ifdef USE_GNOME_DOCS
/**
 * paramItemFree:
 * @item: Is valid
 *
 * Free memory used by libxslt parameter item @item
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Free memory used by libxslt parameter item @p item
 *
 * @param item Is valid
 */
#endif
#endif
    void paramItemFree(ParameterItemPtr item);



#ifdef USE_GNOME_DOCS
/**
 * printParam:
 * @paramId: 0 =< paramID < arrayListCount(getParamList())
 * 
 * Print parameter information
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Prints all items in parameter list
 *
 * @returns 1 on success,
 *          0 otherwise
 */
#endif
#endif
    int printParam(int paramId);



#ifdef USE_GNOME_DOCS
/**
 * printParamList:
 *
 * Prints all items in parameter list
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Prints all items in parameter list
 *
 * @returns 1 on success,
 *          0 otherwise
 */
#endif
#endif
    int printParamList(void);

#ifdef __cplusplus
}
#endif
#endif
