 
/* *************************************************************************
                          xslbreakpoint.h  -  public functions for the
                                               breakpoint API
                             -------------------
    begin                : Fri Dec 7 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ************************************************************************* */

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */


#ifndef XSLBREAKPOINT_H
#define XSLBREAKPOINT_H

/**
 * Provide a basic break point support
 *
 * @short break point support
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */


#ifdef WITH_XSLT_DEBUG
#ifndef WITH_XSLT_DEBUG_BREAKPOINTS
#define WITH_XSLT_DEBUG_BREAKPOINTS
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

    /** The main structure for holding breakpoints*/
    typedef struct _xslBreakPoint xslBreakPoint;
    typedef xslBreakPoint *xslBreakPointPtr;
    struct _xslBreakPoint {
        xmlChar *url;
        long lineNo;
        xmlChar *templateName;
        int enabled;
        int type;
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
 * @param ctxt The transform context for stylesheet being processed
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
 * @reached : 1 if debugger has received control, -1 to read its value,
               0 to clear the flag
 *
 * Set flag that debuger has received control to value of @reached
 *
 * Returns 1 if any breakpoint was reached previously,
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
 * Retrieve the active break point
 *
 * Returns the last break point that we stoped at
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Retrieve the active break point
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
 * @lineNumber: lineNumber >= 0 and is available in url specified and
 *                points to an xml element
 * @temlateName: the template name of breakPoint or NULL
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
 * @breakpoint: A valid breakpoint
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
 * @param breakpoint A valid breakpoint
 * @param enable Enable break point if 1, disable if 0, toggle if -1
*/
#endif
#endif
    int enableBreakPoint(xslBreakPointPtr breakPoint, int enable);


#ifdef USE_GNOME_DOCS
/**
 * xslBreakPointCount:
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
 * @lineNumber: lineNumber >= 0 and is available in url specified
 *
 * Get a break point from the break point collection
 *
 * Returns break point if break point exists at location specified,
 *	   NULL otherwise
*/
#else
#ifdef USE_KDE_DOCS
/**
 * Get a break point from the break point collection
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
 * xslPrintBreakPoint:
 * @file: Is valid
 * @breakpoint: A valid break point
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
 * @param breakpoint A valid break point
 */
#endif
#endif
    int printBreakPoint(FILE * file, xslBreakPointPtr breakPoint);


#ifdef USE_GNOME_DOCS
/**
 * isBreakPoint:
 * @url: Non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber: lineNumber >= 0 and is available in url specified
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
 * @node: Is valid
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
