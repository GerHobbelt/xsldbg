  
/**************************************************************************
                          xslbreakpoint.h  -  public functions for the
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


#ifndef XSLBREAKPOINT_2_H
#define XSLBREAKPOINT_2_H

#ifdef USE_KDE_DOCS
/**
 * Provide a basic break point support
 *
 * @short break point support
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */
#endif

#ifdef WITH_XSLT_DEBUG
#ifndef WITH_XSLT_DEBUG_BREAKPOINTS
#define WITH_XSLT_DEBUG_BREAKPOINTS
#endif
#endif

#include <libxml/tree.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/xsltutils.h>
#include <libxml/xpath.h>

#ifdef __cplusplus
extern "C" {
#endif

  /* indicate that we are to toggle a breakpoint , used for enableBreakPoint*/
#define XSL_TOGGLE_BREAKPOINT -1

  /* Define the types of status whilst debugging*/
  #ifndef USE_KDOC
     typedef enum {
        DEBUG_NONE = 0,             /* must start at zero!! */
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
    } DebugStatusEnum;
  

    typedef enum {
        DEBUG_BREAK_SOURCE = 300,
        DEBUG_BREAK_DATA
    } BreakPointTypeEnum;


/*Indicate what type of variable to print out. 
  Is used by print_variable and searching functions */
    typedef enum {
        DEBUG_GLOBAL_VAR = 200, /* pick a unique starting point */
        DEBUG_LOCAL_VAR,
        DEBUG_ANY_VAR
    } VariableTypeEnum;

  #else
  /* keep kdoc happy */
     enum DebugStatusEnum{
        DEBUG_NONE = 0,             /* must start at zero!! */
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
  

    enum BreakPointTypeEnum{
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

  #endif

    /* The main structure for holding breakpoints*/
    typedef struct _xslBreakPoint xslBreakPoint;
    typedef xslBreakPoint *xslBreakPointPtr;
    struct _xslBreakPoint {
        xmlChar *url;
        long lineNo;
        xmlChar *templateName;
        int enabled;
        BreakPointTypeEnum type;
        int id;
    };


#ifdef USE_GNOME_DOCS
/**
 * debugBreak:
 * @templ: The source node being executed
 * @node: The data node being processed
 * @root: The template being applied to "node"
 * @ctxt: The transform context for stylesheet being processed
 *
 * A break point has been found so pass control to user
 */
#else
#ifdef USE_KDE_DOCS
/**
 * A break point has been found so pass control to user
 *
 * @param templ The source node being executed
 * @param node The data node being processed
 * @param root The template being applied to "node"
 * @param ctxt transform context for stylesheet being processed
 */
#endif
#endif
    void debugBreak(xmlNodePtr templ, xmlNodePtr node,
                       xsltTemplatePtr root, xsltTransformContextPtr ctxt);



#ifdef USE_GNOME_DOCS
/**
 * debugInit:
 *
 * Initialize debugger allocating any memory needed by debugger
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Initialize debugger allocating any memory needed by debugger
 *
 * @returns 1 on success,
 *          0 otherwise
 */
#endif
#endif
    int debugInit(void);



#ifdef USE_GNOME_DOCS
/**
 * debugFree:
 *
 * Free up any memory taken by debugger
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Free up any memory taken by debugger
 */
#endif
#endif
    void debugFree(void);


#ifdef USE_GNOME_DOCS
/** 
 * debugGotControl:
 * @reached: 1 if debugger has received control, -1 to read its value,
               0 to clear the flag
 *
 * Set flag that debuger has received control to value of @reached
 *
 * Returns 1 if any break point was reached previously,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/** 
 * Set flag that debuger has received control to value of @p reached
 *
 * @returns 1 if any breakpoint was reached previously,
 *         0 otherwise
 *
 * @param reached 1 if debugger has received control, -1 to read its value,
 *              0 to clear the flag
 */
#endif
#endif
    int debugGotControl(int reached);




#ifdef USE_GNOME_DOCS
/**
 * activeBreakPoint:
 * 
 * Get the active break point
 *
 * Returns the last break point that we stoped at
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Get the active break point
 *
 * @returns The last break point that we stoped at
 */
#endif
#endif
    xslBreakPointPtr activeBreakPoint(void);



#ifdef USE_GNOME_DOCS
/**
 * setActiveBreakPoint:
 * @breakPoint: Is valid break point or NULL
 *
 * Set the active break point
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Set the active break point
 *
 * @param breakPoint Is valid break point or NULL
 */
#endif
#endif
    void setActiveBreakPoint(xslBreakPointPtr breakPoint);



#ifdef USE_GNOME_DOCS
/**
 * addBreakPoint:
 * @url: Non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber: @lineNumber >= 0 and is available in url specified and
 *                points to an xml element
 * @templateName: The template name of breakPoint or NULL
 * @type: Valid BreakPointTypeEnum
 *
 * Add break point at file and line number specified
 *
 * Returns 1 if successful,
 *	   0 otherwise
*/
#else
#ifdef USE_KDE_DOCS
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
#endif
#endif
    int addBreakPoint(const xmlChar * url, long lineNumber,
                         const xmlChar * templateName,
                         BreakPointTypeEnum type);


#ifdef USE_GNOME_DOCS
/**
 * deleteBreakPoint:
 * @breakPoint: Is valid
 *
 * Delete the break point specified if it can be found using 
 *    @breakPoint's url and lineNo
 *
 * Returns 1 if successful,
 *	   0 otherwise
*/
#else
#ifdef USE_KDE_DOCS
/**
 * Delete the break point specified if it can be found using 
 *    @p breakPoint's url and lineNo
 *
 * @returns 1 if successful,
 *	    0 otherwise
 *
 * @param breakPoint Is valid
 *
*/
#endif
#endif
    int deleteBreakPoint(xslBreakPointPtr breakPoint);



#ifdef USE_GNOME_DOCS
/**
 * emptyBreakPoint:
 *
 * Empty the break point collection
 *
 * Returns 1 if able to empty the breakpoint list of its contents,
 *         0  otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Empty the break point collection
 *
 * @returns 1 if able to empty the break point list of its contents,
 *          0  otherwise
 */
#endif
#endif
    int emptyBreakPoint(void);



#ifdef USE_GNOME_DOCS
/**
 * enableBreakPoint:
 * @breakPoint: A valid breakpoint
 * @enable: Enable break point if 1, disable if 0, toggle if -1
 *
 * Enable or disable a break point
 *
 * Returns 1 if successful,
 *	   0 otherwise
*/
#else
#ifdef USE_KDE_DOCS
/**
 * Enable or disable a break point
 *
 * @returns 1 if successful,
 *	    0 otherwise
 *
 * @param breakPoint A valid breakpoint
 * @param enable Enable break point if 1, disable if 0, toggle if -1
*/
#endif
#endif
    int enableBreakPoint(xslBreakPointPtr breakPoint, int enable);



/**
 * lineNoItemNew:
 * 
 * Returns a new hash table for break points
 */
    xmlHashTablePtr lineNoItemNew(void);


/**
 * lineNoItemFree:
 * @item: valid hashtable of break points
 * 
 * Free @item and all its contents
 */
    void lineNoItemFree(void *item);


/**
 * lineNoItemDelete:
 * @breakPointHash: is valid
 * @breakPoint: is valid
 * 
 * Returns 1 if able to delete @breakPoint from @breakPointHash,
 *         0 otherwise
 */
    int lineNoItemDelete(xmlHashTablePtr breakPointHash,
                         xslBreakPointPtr breakPoint);


/**
 * lineNoItemAdd:
 * @breakPointHash: is valid
 * @breakPoint: is valid
 *
 * Returns 1 if able to add @breakPoint to @breakPointHash,
 *         0 otherwise
 */
    int lineNoItemAdd(xmlHashTablePtr breakPointHash,
                      xslBreakPointPtr breakPoint);


/**
 * lineNoItemGet:
 * @lineNo: blagh
 *
 * Returns fred
 */
    xmlHashTablePtr lineNoItemGet(long lineNo);


/**
 * breakPointInit:
 *
 * Returns 1 if break points have been initialized properly and all
 *               memory required has been obtained,
 *         0 otherwise
*/
    int breakPointInit(void);


/**
 * breakPointFree:
 *
 * Free all memory used by break points 
 */
    void breakPointFree(void);


/** 
 * breakPointItemNew:
 * 
 * Create a new break point item
 * Returns valid break point with default values set if successful, 
 *         NULL otherwise
 */
    xslBreakPointPtr breakPointItemNew(void);


/**
 * breakPointItemFree:
 * @payload: valid xslBreakPointPtr 
 * @name: not used
 *
 * Free memory associated with this break point
 */
    void breakPointItemFree(void *payload, xmlChar * name);



#ifdef USE_GNOME_DOCS
/**
 * breakPointLinesCount:
 *
 * Return the number of hash tables of break points with the same line number
 *
 * Returns the number of hash tables of break points with the same line number
 */
#else
#ifdef USE_KDE_DOCS
/** 
 * Return the number of hash tables of break points with the same line number
 *
 * @returns the number of hash tables of break points with the same line number
 */
#endif
#endif
    int breakPointLinesCount(void);


#ifdef USE_GNOME_DOCS
/**
 * getBreakPoint:
 * @url: Non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber: lineNumber >= 0 and is available in @url
 *
 * Get a break point for the breakpoint collection
 *
 * Returns break point if break point exists at location specified,
 *	   NULL otherwise
*/
#else
#ifdef USE_KDE_DOCS
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
#endif
#endif
    xslBreakPointPtr getBreakPoint(const xmlChar * url, long lineNumber);


#ifdef USE_GNOME_DOCS
/**
 * printBreakPoint:
 * @file: Is valid
 * @breakPoint: A valid break point
 *
 * Print the details of @breakPoint to @file
 *
 * Returns 1 if successful,
 *	   0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Print the details of @p breakPoint to @p file
 *
 * @returns 1 if successful,
 *	    0 otherwise
 *
 * @param file Is valid
 * @param breakPoint A valid break point
 */
#endif
#endif
    int printBreakPoint(FILE * file, xslBreakPointPtr breakPoint);


#ifdef USE_GNOME_DOCS
/**
 * isBreakPoint:
 * @url: Non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber: @lineNumber >= 0 and is available in @url
 *
 * Determine if there is a break point at file and line number specified
 *
 * Returns 1  if successful,  
 *         0 otherwise
*/
#else
#ifdef USE_KDE_DOCS
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
#endif
#endif
    int isBreakPoint(const xmlChar * url, long lineNumber);


#ifdef USE_GNOME_DOCS
/**
 * isBreakPointNode:
 * @node: node != NULL
 *
 * Determine if a node is a break point
 *
 * Returns 1 on success, 
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Determine if a node is a break point
 *
 * @returns 1 on success, 
 *          0 otherwise
 *
 * @param node Is valid
 */
#endif
#endif
    int isBreakPointNode(xmlNodePtr node);

#ifdef __cplusplus
}
#endif
#endif

 
