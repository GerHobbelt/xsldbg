
/**************************************************************************
                          breakpoint.h  -  public functions for the
                                               breakpoint API
                             -------------------
    begin                : Fri Dec 7 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 **************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 **************************************************************************/







/**
 * Provide a basic break point support
 *
 * @short break point support
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */




/* BUILD_DOCS */



    /* indicate that we are to toggle a breakpoint , used for enableBreakPoint */


    /* Define the types of status whilst debugging */

    /* keep kdoc happy */
    enum DebugStatusEnum {
        DEBUG_NONE = 0,         /* must start at zero!! */
        DEBUG_INIT,
        DEBUG_STEP,
        DEBUG_STEPUP,
        DEBUG_STEPDOWN,
        DEBUG_NEXT,
        DEBUG_STOP,
        DEBUG_CONT,
        DEBUG_RUN,
        DEBUG_RUN_RESTART,
        DEBUG_QUIT,
        DEBUG_TRACE,
        DEBUG_WALK
    };


    enum BreakPointTypeEnum {
        DEBUG_BREAK_SOURCE = 300,
        DEBUG_BREAK_DATA
    };


/*Indicate what type of variable to print out. 
  Is used by print_variable and searching functions */
    enum VariableTypeEnum {
        DEBUG_GLOBAL_VAR = 200, /* pick a unique starting point */
        DEBUG_LOCAL_VAR,
        DEBUG_ANY_VAR
    } VariableTypeEnum;



    /* The main structure for holding breakpoints */
    typedef struct _breakPoint breakPoint;
    typedef breakPoint *breakPointPtr;
    struct _breakPoint {
        xmlChar *url;
        long lineNo;
        xmlChar *templateName;
        int enabled;
        BreakPointTypeEnum type;
        int id;
    };





/**
 * Intialized the breakpoint module 
 *
 * @returns 1 if breakpoint module has been initialized properly and all
 *               memory required has been obtained,
 *          0 otherwise
*/


    int breakPointInit(void);






/**
 * Free all memory used by breakpoint module
 */


    void breakPointFree(void);






/**
 * Get the active break point
 *
 * @returns The last break point that we stoped at
 *
 * Depreciated
 */


    breakPointPtr breakPointActiveBreakPoint(void);






/**
 * Set the active break point
 *
 * @param breakPoint Is valid break point or NULL
 *
 * Depreciated
 */


    void breakPointSetActiveBreakPoint(breakPointPtr breakPtr);






/**
 * Add break point at file and line number specified
 *
 * @returns 1 if successful,
 *	    0 otherwise
 *
 * @param url Non-null, non-empty file name that has been loaded by
 *                    debugger
 * @param lineNumber @p lineNumber >= 0 and is available in url specified and
 *                points to an xml element
 * @param temlateName The template name of break point or NULL
 * @param type Valid BreakPointTypeEnum
*/


    int breakPointAdd(const xmlChar * url, long lineNumber,
                      const xmlChar * templateName,
                      BreakPointTypeEnum type);






/**
 * Delete the break point specified if it can be found using 
 *    @p breakPtr's url and lineNo
 *
 * @returns 1 if successful,
 *	    0 otherwise
 *
 * @param breakPoint Is valid
 *
*/


    int breakPointDelete(breakPointPtr breakPtr);






/**
 * Empty the break point collection
 *
 * @returns 1 if able to empty the break point list of its contents,
 *          0  otherwise
 */


    int breakPointEmpty(void);






/**
 * Enable or disable a break point
 *
 * @returns 1 if successful,
 *	    0 otherwise
 *
 * @param breakPoint A valid breakpoint
 * @param enable Enable break point if 1, disable if 0, toggle if -1
*/


    int breakPointEnable(breakPointPtr breakPtr, int enable);






/**
 * Get a break point for the breakpoint collection
 *
 * @returns break point if break point exists at location specified,
 *	    NULL otherwise
 *
 * @param url Non-null, non-empty file name that has been loaded by
 *                    debugger
 * @param lineNumber @p lineNumber >= 0 and is available in url specified
*/


    breakPointPtr breakPointGet(const xmlChar * url, long lineNumber);






/**
 * Return A hash of breakpoints with same line number
 *
 * @param lineNo : Line number of of breakpoints of interest
 *
 * @returns A hash of breakpoints with a line number of @p lineNo
 */


    xmlHashTablePtr breakPointGetLineNoHash(long lineNo);






/** 
 * Create a new break point item
 *
 * @returns A valid break point with default values set if successful, 
 *          NULL otherwise
 */


    breakPointPtr breakPointItemNew(void);






/**
 * Free memory associated with this break point
 *
 * @param payload Valid breakPointPtr 
 * @param name not used
 *
 */


    void breakPointItemFree(void *payload, xmlChar * name);






/** 
 * Return the number of hash tables of break points with the same line number
 *
 * @returns the number of hash tables of break points with the same line number
 */


    int breakPointLinesCount(void);






/**
 * Print the details of @p breakPtr to @p file
 *
 * @returns 1 if successful,
 *	    0 otherwise
 *
 * @param file Is valid, or NULL to use libxslt's error display function
 * @param breakPoint A valid break point
 */


    int breakPointPrint(FILE * file, breakPointPtr breakPtr);






/**
 * Determine if there is a break point at file and line number specified
 *
 * @returns 1  if successful,  
 *          0 otherwise
 *
 * @param url Non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber @p lineNumber >= 0 and is available in url specified
*/


    int breakPointIsPresent(const xmlChar * url, long lineNumber);






/**
 * Determine if a node is a break point
 *
 * @returns 1 on success, 
 *          0 otherwise
 *
 * @param node Is valid
 */


    int breakPointIsPresentNode(xmlNodePtr node);






/**
 * Return The list of hash tables for break points
 *        Dangerous function to use!!
 *
 * Returns The list of hash tables for break points
 *        Dangerous function to use!! 
 */


    arrayListPtr breakPointLineList(void);



