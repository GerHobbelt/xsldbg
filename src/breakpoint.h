
/***************************************************************************
                          breakpoint.h  -  public functions for the
                                               breakpoint API
                             -------------------
    begin                : Sun Sep 16 2001
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
 ************************************************************************* */

#ifndef  REAKPOINT_NEW_H
#define BREAKPOINT_NEW_H


#include <string.h>
#include <libxml/hash.h>
#include <libxml/debugXML.h>    /* needed for xmlGetLineNo(node) */
#include <libxml/xpath.h>
#include <libxml/tree.h>
#include <libxslt/xsltutils.h>
#include <libxslt/xsltInternals.h>


#ifdef WITH_XSLT_DEBUG
#ifndef WITH_XSLT_DEBUG_BREAKPOINTS
#define WITH_XSLT_DEBUG_BREAKPOINTS
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif


/* Define the types of status whilst debugging*/
     typedef enum {
        DEBUG_NONE,             /* must start at zero !! */
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


#define XSL_TOGGLE_BREAKPOINT -1
    extern int xslDebugStatus;  /* defined in libxslt/xsltutils.h  */

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

/*
-----------------------------------------------------------
             Break point related functions
----------------------------------------------------------
*/


/**
 * activeBreakPoint:
 *
 * Returns the last break point that we stoped at
 */
    xslBreakPointPtr activeBreakPoint(void);


/**
 * setActiveBreakPoint:
 * @breakPoint: is valid break point or NULL
 *
 * Set the active break point
 */
    void setActiveBreakPoint(xslBreakPointPtr breakPoint);


/**
 * addBreakPoint:
 * @url: url non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber: lineNumber >= 0 and is available in url specified and
 *                points to an xml element
 * @temlateName: the template name of breakPoint or NULL
 * @type: DEBUG_BREAK_SOURCE if are we stopping at a xsl source line
 *         DEBUG_BREAK_DATA otherwise
 *
 * Add break point at file and line number specified
 *
 * Returns 1 if successful,
 *	   0 otherwise
*/
    int addBreakPoint(const xmlChar * url, long lineNumber,
                         const xmlChar * templateName,
                         BreakPointTypeEnum type);

/**
 * deleteBreakPoint:
 * @breakPoint: is valid
 *
 * Delete the break point specified if it can be found using 
 *    @breakPoint's url and lineNo
 * Returns 1 if successful,
 *	   0 otherwise
*/
    int deleteBreakPoint(xslBreakPointPtr breakPoint);


/**
 * emptyBreakPoint:
 *
 * Returns 1 if able to empty the breakpoint list of its contents,
 *         0  otherwise
 */
    int emptyBreakPoint(void);


/**
 * enableBreakPoint:
 * @breakpoint: a valid breakpoint
 * @enable: enable break point if 1, disable if 0, toggle if -1
 *
 * Enable or disable a break point
 * Returns 1 if successful,
 *	   0 otherwise
*/
    int enableBreakPoint(xslBreakPointPtr breakPoint, int enable);


/**
 * breakPointLinesCount:
 *
 * Returns the number of hash tables of break points with the same line number
 */
    int breakPointLinesCount(void);


/**
 * getBreakPoint:
 * @url: url non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber: lineNumber >= 0 and is available in url specified
 * Returns break point if break point exists at location specified,
 *	   NULL otherwise
*/
    xslBreakPointPtr getBreakPoint(const xmlChar * url, long lineNumber);


/**
 * xslPrintBreakPoint:
 * @file: file != NULL
 * @breakpoint: a valid break point
 *
 * Print the details of @breakPoint to @file
 *
 * Returns 1 if successful,
 *	   0 otherwise
 */
    int printBreakPoint(FILE * file, xslBreakPointPtr breakPoint);


/**
 * isBreakPoint:
 * @url: url non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber: lineNumber >= 0 and is available in url specified
 *
 * Determine if there is a break point at file and line number specified
 * Returns 1  if successful,  
 *         0 otherwise
*/
    int isBreakPoint(const xmlChar * url, long lineNumber);


/**
 * isBreakPointNode:
 * @node: node != NULL
 *
 * Determine if a node is a break point
 * Returns 1 on success, 
 *         0 otherwise
 */
    int isBreakPointNode(xmlNodePtr node);

/*
-----------------------------------------------------------
       Main debugger functions
-----------------------------------------------------------
*/

/**
 * debugBreak:
 * @templ: The source node being executed
 * @node: The data node being processed
 * @root: The template being applied to "node"
 * @ctxt: The transform context for stylesheet being processed
 *
 * A break point has been found so pass control to user
 */
    void debugBreak(xmlNodePtr templ, xmlNodePtr node,
                       xsltTemplatePtr root, xsltTransformContextPtr ctxt);


/**
 * debugInit:
 *
 * Initialize debugger allocating any memory needed by debugger
 * Returns 1 on success,
 *         0 otherwise
 */
    int debugInit(void);


/**
 * debugFree:
 *
 * Free up any memory taken by debugger
 */
    void debugFree(void);


/** 
 * xlDebugGotControl:
 * @reached : 1 if debugger has received control, -1 to read its value,
               0 to clear the flag
 *
 * Set flag that debuger has received control to value of @reached
 * Returns 1 if any breakpoint was reached previously,
 *         0 otherwise
 */
    int debugGotControl(int reached);



/*
------------------------------------------------------
                  Xsl call stack related
-----------------------------------------------------
*/

    typedef struct _xslCallPointInfo xslCallPointInfo;
    typedef xslCallPointInfo *xslCallPointInfoPtr;

    struct _xslCallPointInfo {
        xmlChar *templateName;
        xmlChar *url;
        xslCallPointInfoPtr next;
    };


/**
 * addCallInfo:
 * @templateName: template name to add
 * @url: url for the template
 *
 * Returns a reference to the added info if successful, 
 *         NULL otherwise
 */
    xslCallPointInfoPtr addCallInfo(const xmlChar * templateName,
                                       const xmlChar * url);

    typedef struct _xslCallPoint xslCallPoint;
    typedef xslCallPoint *xslCallPointPtr;

    struct _xslCallPoint {
        xslCallPointInfoPtr info;
        long lineNo;
        xslCallPointPtr next;
    };


/**
 * addCall:
 * @templ: Template name to add
 * @source: The url for the template
 *
 * Add template "call" to call stack
 * Returns 1 on success,
 *         0 otherwise 
 */
    int addCall(xsltTemplatePtr templ, xmlNodePtr source);


/**
 * dropCall:
 *
 * Drop the topmost item off the call stack
 */
    void dropCall(void);


/** 
 * stepupToDepth:
 * @depth:the frame depth to step up to  
 *             0 < @depth <= callDepth()
 *
 * Set the frame depth to step up to
 * Returns 1 on success,
 *         0 otherwise
 */
    int stepupToDepth(int depth);


/** 
 * stepdownToDepth:
 * @depth: the frame depth to step down to, 
 *             0 < @depth <= callDepth()
 *
 * Set the frame depth to step down to
 * Returns 1 on success, 
 *         0 otherwise
 */
    int stepdownToDepth(int depth);


/**
 * getCall:
 * @depth: 0 < @depth <= callDepth()
 *
 * Retrieve the call point at specified call depth 

 * Returns non-null a if depth is valid,
 *         NULL otherwise 
 */
    xslCallPointPtr getCall(int depth);


/** 
 * getCallStackTop:
 *
 * Returns the top of the call stack
 */
    xslCallPointPtr getCallStackTop(void);


/**
 * callDepth:
 *
 * Returns the depth of call stack
 */
    int callDepth(void);


/*
-----------------------------------------------------------
             Search related functions
----------------------------------------------------------
*/


/**
 * searchEmpty:
 *
 * Empty the seach data base of its contents
 * Returns 1 on success,
 *         0 otherwise
 */
    int searchEmpty(void);


/**
 * searchAdd:
 * @node: a valid node to be added to the topmost node in search dataBase
 *
 * Returns 1 if able to add @node to top node in search dataBase,
 *         0 otherwise
 */
    int searchAdd(xmlNodePtr node);


/**
 * searchSave:
 * @fileName: valid fileName to save search dataBase to 
 *
 * Returns 1 on success,
 *         0 otherwise
 */
    int searchSave(const xmlChar * fileName);


/**
 * searchQuery:
 * @query: query to run . If NULL then query is "//search/ *"
 * @tempFile: where do we load the search dataBase from to execute
 *             query. If tempFile is NULL "search.data" is used
 * 
 * Send query as parameter for execution of search.xsl using
 *    data stored in @tempFile 
 * Returns 1 on success,
 *         0 otherwise   
 */
    int searchQuery(const xmlChar * tempFile, const xmlChar * query);


/**
 * findNodeByLineNo:
 * @ctxt: valid ctxt to look into
 * @url: url non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber: number >= 0 and is available in url specified
 *
 * Find the closest line number in file specified that can be a point 
 * Returns  node at line number number specified if successfull,
 *	    NULL otherwise
*/
    xmlNodePtr findNodeByLineNo(xsltTransformContextPtr ctxt,
                                   const xmlChar * url, long lineNumber);

/**
 * findTemplateNode: 
 * @style: valid stylesheet collection to look into 
 * @name: template name to look for
 *
 * Returns template node found if successful,
 *         NULL otherwise 
 */
    xmlNodePtr findTemplateNode(const xsltStylesheetPtr style,
                                   const xmlChar * name);

/**
 * findBreakPointByName:
 * @templateName: template name to look for
 *
 * Find the breakpoint at template with "match" or "name" equal 
 *    to templateName
 * Returns the break point that matches @templateName
 *         NULL otherwise
*/
    xslBreakPointPtr findBreakPointByName(const xmlChar * templateName);


/**
 * findBreakPointById:
 * @id: The break point id to look for
 *
 * Returns the break point with given the break point id if found,
 *          NULL otherwise 
 */
    xslBreakPointPtr findBreakPointById(int id);


/**
 * findNodesByQuery:
 * @query: xpath query to run, see dbgsearch.c for more details
 * 
 * Returns the nodes that match the given query on success,
 *         NULL otherwise 
 */
    xmlXPathObjectPtr findNodesByQuery(const xmlChar * query);


/**
 * walkBreakPoints:
 * @walkFunc: function to callback for each breakpoint found
 * @data: the extra data to pass onto walker
 *
 * Walks through all break points calling walkFunc for each. The payload
 *  sent to walkFunc is of type xslBreakPointPtr 
 */
    void walkBreakPoints(xmlHashScanner walkFunc, void *data);


/**
 * walkTemplates:
 * @walkFunc: function to callback for each template found
 * @data: the extra data to pass onto walker
 * @style: the stylesheet to start from
 *
 * Walks through all templates calling walkFunc for each. The payload
 *   of walkFunc is of type xsltTemplatePtr
 */
    void walkTemplates(xmlHashScanner walkFunc, void *data,
                       xsltStylesheetPtr style);


/**
 * walkStylesheets:
 * @walkFunc: function to callback for each stylesheet found
 * @data: the extra data to pass onto walker
 * @style: the stylesheet to start from
 *
 * Walks through all templates calling walkFunc for each. The payload
 *   sent to walkFuc is of type xsltStylesheetPtr
 */
    void walkStylesheets(xmlHashScanner walkFunc, void *data,
                         xsltStylesheetPtr style);


/**
 * walkGlobals:
 * @walkFunc: function to callback for each gobal variable found
 * @data: the extra data to pass onto walker
 * @style: the stylesheet to start from
 *
 * Call walkFunc for each global variable. The payload
 *   sent to walkFunc is of type  xmlNodePtr
 */
    void walkGlobals(xmlHashScanner walkFunc,
                        void *data, xsltStylesheetPtr style);


/**
 * walkLocals:
 * @walkFunc: function to callback for each local variable found
 * @data: the extra data to pass onto walker
 * @style: the stylesheet to start from
 *
 * Walks through all local variables calling walkFunc for each. The payload
 *   of walkFunc is of type xmlNodePtr
 */
    void walkLocals(xmlHashScanner walkFunc, void *data,
                       xsltStylesheetPtr style);


/**
 * walkIncludes:
 * @walkFunc: function to callback for each included stylesheet
 * @data: the extra data to pass onto walker
 * @style: the stylesheet to start from
 *
 * Walks through all included stylesheets calling walkFunc for each. The payload
 *   of walkFunc is of type xmlNodePtr
 */
    void walkIncludes(xmlHashScanner walkFunc, void *data,
                         xsltStylesheetPtr style);


/**
 * walkIncludeInst:
 * @walkFunc: function to callback for each xsl:include instruction found
 * @data: the extra data to pass onto walker
 * @style: the stylesheet to start from
 *
 * Walks through all xsl:include calling walkFunc for each. The payload
 *   of walkFunc is of type xmlNodePtr
 */
    void walkIncludeInst(xmlHashScanner walkFunc, void *data,
                         xsltStylesheetPtr style);


/**
 * walkChildNodes:
 * @walkFunc: function to callback for each child/sibling found
 * @data: the extra data to pass onto walker
 * @node: valid xmlNodePtr
 *
 * Call walkFunc for each child of @node the payload sent to walkFunc is
 *   a xmlNodePtr
 */
    void walkChildNodes(xmlHashScanner walkFunc, void *data,
                           xmlNodePtr node);

#ifdef __cplusplus
}
#endif
#endif
