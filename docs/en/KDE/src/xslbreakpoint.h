
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

/**
 * A break point has been found so pass control to user
 *
 * @param templ The source node being executed
 * @param node The data node being processed
 * @param root The template being applied to "node"
 * @param ctxt transform context for stylesheet being processed
 */
    void xslDebugBreak(xmlNodePtr templ, xmlNodePtr node,
                       xsltTemplatePtr root, xsltTransformContextPtr ctxt);


/**
 * Initialize debugger allocating any memory needed by debugger
 *
 * @returns 1 on success,
 *          0 otherwise
 */
    int debugInit(void);


/**
 * Free up any memory taken by debugger
 */
    void debugFree(void);


/** 
 * Set flag that debuger has received control to value of @p reached
 *
 * @returns 1 if any breakpoint was reached previously,
 *         0 otherwise
 *
 * @param reached 1 if debugger has received control, -1 to read its value,
 *              0 to clear the flag
 */
    int xslDebugGotControl(int reached);



/**
 * @returns The last break point that we stoped at
 */
    xslBreakPointPtr activeBreakPoint(void);


/**
 * Set the active break point
 *
 * @param breakPoint Is valid break point or NULL
 */
    void setActiveBreakPoint(xslBreakPointPtr breakPoint);


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
    int xslAddBreakPoint(const xmlChar * url, long lineNumber,
                         const xmlChar * templateName,
                         BreakPointTypeEnum type);

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
    int deleteBreakPoint(xslBreakPointPtr breakPoint);


/**
 * @returns 1 if able to empty the break point list of its contents,
 *          0  otherwise
 */
    int xslEmptyBreakPoint(void);


/**
 * Enable or disable a break point
 *
 * @returns 1 if successful,
 *	    0 otherwise
 *
 * @param breakpoint A valid breakpoint
 * @param enable Enable break point if 1, disable if 0, toggle if -1
*/
    int enableBreakPoint(xslBreakPointPtr breakPoint, int enable);


/**
 * @returns the number of hash tables of break points with the same line number
 */
    int xslBreakPointLinesCount(void);


/**
 * @returns break point if break point exists at location specified,
 *	    NULL otherwise
 *
 * @param url Non-null, non-empty file name that has been loaded by
 *                    debugger
 * @param lineNumber @p lineNumber >= 0 and is available in url specified
*/
    xslBreakPointPtr getBreakPoint(const xmlChar * url, long lineNumber);


/**
 * Print the details of @p breakPoint to @p file
 *
 * @returns 1 if successful,
 *	    0 otherwise
 *
 * @param file Is valid
 * @param breakpoint A valid break point
 */
    int printBreakPoint(FILE * file, xslBreakPointPtr breakPoint);


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
    int xslIsBreakPoint(const xmlChar * url, long lineNumber);


/**
 * Determine if a node is a break point
 *
 * @returns 1 on success, 
 *          0 otherwise
 *
 * @param node Is valid
 */
    int xslIsBreakPointNode(xmlNodePtr node);

#ifdef __cplusplus
}
#endif
#endif
