
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






/**
 * Provide a mechanism to change option. The Options structure is not in use,
 *   it has been added so that kdoc puts all option related 
 *   functions together 
 *
 * @short file support
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */


/* We want skip most of these includes when building documentation */





/* ---------------------------------------  
        Misc options
-------------------------------------------*/


    /* keep kdoc happy */
    enum OptionsTypeEnum {
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
        OPTIONS_CATALOGS,       /* Get the catalogs from SGML_CATALOG_FILES and
                                 * store it in OPTIONS_CATALOG_NAMES */
        OPTIONS_PREFER_HTML,    /* Prefer html output for search results */
        OPTIONS_AUTOENCODE,     /* try to use the encoding from the stylesheet */
        OPTIONS_VERBOSE,        /* Be verbose with messages */
        OPTIONS_OUTPUT_FILE_NAME,       /* what is the output file name */
        OPTIONS_SOURCE_FILE_NAME,       /*  the stylesheet source to use */
        OPTIONS_DOCS_PATH,      /* path of xsldbg's documentation */
        OPTIONS_CATALOG_NAMES,  /* the names of the catalogs to use when catalogs option is active */
        OPTIONS_ENCODING,       /* What encoding to use for standard output */
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


/* how many microseconds is each speed increase worth*/



/* for non win32 environments see the macro in xsldebugger/Makefile.am
   Win32 type systems see macro in libxslt/xsltwin32config.h
*/


/* The environment variable name on normal systems */



/* used to keep track of libxslt paramters 
 see Parameter related options near end of file
*/
    typedef struct _ParameterItem ParameterItem;
    typedef ParameterItem *ParameterItemPtr;
    struct _ParameterItem {
        xmlChar *name;          /* libxslt parameter name */
        xmlChar *value;         /* libxslt parameter value */
    };






/** 
 * Initialized the options module
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
 * Set the state of a boolean xsldbg option to @p value
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param optionType Is a valid boolean option
 * @param value 1 to enable, 0 otherwise
 */


    int enableOption(OptionTypeEnum optionType, int value);






/**
 * Return the state of a boolean option
 *
 * @returns The state of a boolean xsldbg option. 
 *            ie 1 for enabled, 0 for disabled
 *
 * @param optionType Is a valid boolean option to query

 */


    int isOptionEnabled(OptionTypeEnum optionType);






/**
 * Set the value of an integer xsldbg option to @p value
 *
 * @returns 1 on success,
 *         0 otherwise
 *
 * @param optionType Is a valid integer option
 * @param value Is the valid to adopt
 */


    int setIntOption(OptionTypeEnum optionType, int value);






/**
 * @returns The state of a integer xsldbg option
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





  /**
   * Copy volitile options to the working area for xsldbg
   */


    void copyVolitleOptions(void);





  /**
   * Convert option into a xmlNodePtr
   *
   * @returns The option @p optionType as a xmlNodePtr if successful,
   *          NULL otherwise
   *
   * @param optionType Is valid, option to convert to a xmlNodePtr 
   *
   */


  xmlNodePtr optionNode(OptionTypeEnum optionType);






  /**
   * Read options from document provided. 
   *
   * @returns 1 if able to read @p doc and load options found,
   *          0 otherwise
   *
   * @param doc Is valid xsldbg config document, in the format as indicated 
   *        by config.dtd
   */


  int optionsReadDoc(xmlDocPtr doc);


/* ---------------------------------------------
          Platform specific option functions
-------------------------------------------------*/




  /**
   * Intialize the platform specific options module
   *
   *  This is a platform specific interface
   *
   * @returns 1 if sucessful
   *          0 otherwise  
   */


  int optionsPlatformInit(void);





  /**
   * Free memory used by the platform specific options module
   *
   *  This is a platform specific interface
   *
   */


  void optionsPlatformFree(void);





  /**
   * Return xsldbg's the configuration file name 
   *
   * Returns A copy of the file name that will be used to load xsldbgs
   *           configuration from,
   *         NULL otherwise
   *
   *  This is a platform specific interface
   *
   */


  xmlChar* optionsConfigFileName(void);





  /**
   * Load options from configuration file/registry
   *
   * This is a platform specific interface
   * 
   * Returns 1 if able to load options
   *         0 otherwise
   */


  int optionsLoad(void);


/*---------------------------------------------
          Parameter related options 
-------------------------------------------------*/





/**
 * Return the list of libxlt parameters
 *
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
 * Prints all items in parameter list
 *
 * @returns 1 on success,
 *          0 otherwise
 */


    int printParam(int paramId);






/**
 * Prints all items in parameter list
 *
 * @returns 1 on success,
 *          0 otherwise
 */


    int printParamList(void);



