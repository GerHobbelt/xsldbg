
/***************************************************************************
                          breakpoint.h  -  public functions for the breakpoint API
                             -------------------
    begin                : Sun Sep 16 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/


#ifndef BREAKPOINT_NEW_H
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

/* sections commented out like below are just here to clarify the whole picture*/

/* ##!


#include <libxml/tree.h>
#include <libxslt/xsltInternals.h>
#include <libxml/xpath.h>
*/

#ifdef __cplusplus
extern "C" {
#endif

/* Define the types of status whilst debugging*/
    typedef enum {
        DEBUG_NONE,
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
    } DebugStatus2;

    typedef enum {
        DEBUG_BREAK_SOURCE = 1,
        DEBUG_BREAK_DATA
    } BreakPointType;

#define XSL_TOGGLE_BREAKPOINT -1
    extern int xslDebugStatus;

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


/* renamed to avoid clash with libxslt*/

/**
 * activeBreakPoint;
 *
 * Return the last breakPoint that we stoped at
 */
    xslBreakPointPtr activeBreakPoint(void);


/* renamed to avoid clash with libxslt */

/**
 * setActiveBreakPoint:
 * @breakoint : is valid breakpoint or NULL
 *
 */
    void setActiveBreakPoint(xslBreakPointPtr breakpoint);


/**
 * xslAddBreakPoint:
 * @url : a valid url that has been loaded by debugger
 * @lineNumber : number >= 0 and is available in url specified and points to 
 *               an xml element
 * @temlateName : the template name of breakpoint or NULL if not adding
 *                 a template break point
 * @type : DEBUG_BREAK_SOURCE if are we stopping at a xsl source line
 *         DEBUG_BREAK_DATA otherwise
 *
 * Add break point at file and line number specified
 * Returns break point number if successfull,
 *	    0 otherwise 
*/
    int xslAddBreakPoint(const xmlChar * url, long lineNumber,
                         const xmlChar * templateName, int type);

/**
 * xslDeleteBreakPoint:
 * @breakPoint : is valid
 *
 * Delete the break point specified if it can be found using 
 *    @breakPoint's url and lineNo
 * Returns 1 if successfull,
 *	    0 otherwise
*/
    int deleteBreakPoint(xslBreakPointPtr breakPoint);


/**
 * xslEmptyBreakPoint:
 *
 * Returns 1 if able to empty the breakpoint list of its contents,
 *         0  otherwise
 */
    int xslEmptyBreakPoint(void);


/**
 * xslEnableBreakPoint:
 * @breakpoint : a valid breakpoint
 * @enable : enable break point if 1, disable if 0, toggle if -1
 *
 * Enable or disable a break point
 * Returns 1 if successfull,
 *	    0 otherwise
*/
    int enableBreakPoint(xslBreakPointPtr breakPoint, int enable);


/**
 * xslBreakPointCount:
 *
 * Return the number of hash tables of breakPoints with the same line number
 */
    int xslBreakPointLinesCount(void);



/**
 * xslGetBreakPoint:
 * @url : url non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber : number >= 0 and is available in url specified
 * Returns break point if break point exists at location specified,
 *	    NULL otherwise
*/
    xslBreakPointPtr getBreakPoint(const xmlChar * url, long lineNumber);


/**
 * xslPrintBreakPoint:
 * @file : file != NULL
 * @breakpoint : a valid breakpoint
 *
 * Print the details of break point to file specified
 *
 * Returns 1 if successfull,
 *	   0 otherwise
 */
    int printBreakPoint(FILE * file, xslBreakPointPtr breakPoint);


/**
 * xslIsBreakPoint:
 * @url : url non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber : number >= 0 and is available in url specified
 *
 * Determine if there is a break point at file and line number specifiec
 * Returns 1  if successfull,  
 *         0 otherwise
*/
    int xslIsBreakPoint(const xmlChar * url, long lineNumber);


/**
 * xslIsBreakPointNode:
 * @node : node != NULL
 *
 * Determine if a node is a break point
 * Returns : 1 on success, 
 *           0 otherwise
 */
    int xslIsBreakPointNode(xmlNodePtr node);

/*
-----------------------------------------------------------
       Main debugger functions
-----------------------------------------------------------
*/

/**
 * xslDebugBreak:
 * @templ : The source node being executed
 * @node : The data node being processed
 * @root : The template being applide to "node"
 * @ctxt : transform context for stylesheet being processed
 *
 * A break point has been found so pass control to user
 */
    void xslDebugBreak(xmlNodePtr templ, xmlNodePtr node,
                       xsltTemplatePtr root, xsltTransformContextPtr ctxt);


/* renamed to avoid clash with libxslt*/

/**
 * debugInit :
 *
 * Initialize debugger allocating any memory needed by debugger
 * Returns 1 on success,
 *         0 otherwise
 */
    int debugInit(void);


/* renamed to avoid clash with libxslt*/

/**
 * debugFree :
 *
 * Free up any memory taken by debugger
 */
    void debugFree(void);


/** 
 * xslDebugGotControl :
 * @reached : 1 if debugger has received control, 0 otherwise
 *
 * Set flag that debuger has received control to value of @reached
 * Returns 1 if any breakpoint was reached previously,
 *         0 otherwise
 */
    int xslDebugGotControl(int reached);



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
 * xslAddCallInfo:
 * @templateName : template name to add
 * @url : url for the template
 *
 * Returns a reference to the added info if successfull, 
 *         NULL otherwise
 */
    xslCallPointInfoPtr xslAddCallInfo(const xmlChar * templateName,
                                       const xmlChar * url);

    typedef struct _xslCallPoint xslCallPoint;
    typedef xslCallPoint *xslCallPointPtr;

    struct _xslCallPoint {
        xslCallPointInfoPtr info;
        long lineNo;
        xslCallPointPtr next;
    };


/**
 * xslAddCall:
 * @templ : current template being applied
 * @source : the source node being processed
 *
 * Add template "call" to call stack
 * Returns 1 on success,
 *         0 otherwise 
 */
    int xslAddCall(xsltTemplatePtr templ, xmlNodePtr source);


/**
 * xslDropCall :
 *
 * Drop the topmost item off the call stack
 */
    void xslDropCall(void);


/** 
 * xslStepupToDepth :
 * @depth :the frame depth to step up to  
 *
 * Set the frame depth to step up to
 * Returns 1 on success,
 *         0 otherwise
 */
    int xslStepupToDepth(int depth);


/** 
 * xslStepdownToDepth :
 * @depth : the frame depth to step down to 
 *
 * Set the frame depth to step down to
 * Returns 1 on success, 
 *         0 otherwise
 */
    int xslStepdownToDepth(int depth);


/**
 * xslGetCall :
 * @depth : 0 < depth <= xslCallDepth()
 *
 * Retrieve the call point at specified call depth 

 * Return non-null a if depth is valid,
 *        NULL otherwise 
 */
    xslCallPointPtr xslGetCall(int depth);


/** 
 * xslGetCallStackTop :
 *
 * Returns the top of the call stack
 */
    xslCallPointPtr xslGetCallStackTop(void);


/**
 * xslCallDepth :
 *
 * Returns the depth of call stack
 */
    int xslCallDepth(void);


/*
-----------------------------------------------------------
             Search related functions
----------------------------------------------------------
*/


/**
 * xslSearchEmpty:
 *
 * Empty the seach data base of its contents
 * Returns 1 on success,
 *         0 otherwise
 */
    int xslSearchEmpty(void);


/**
 * xslSearchAdd:
 * @node : a valid node to be added to the topmost node in search dataBase
 *
 * Return 1 if able to add @node to top node in search dataBase,
 *        0 otherwise
 */
    int xslSearchAdd(xmlNodePtr node);

/**
 * xslSearchSave:
 * @fileName : valid fileName to save search dataBase to 
 *
 * Return 1 on success,
 *        0 otherwise
 */
    int xslSearchSave(const xmlChar * fileName);

  /**
   * xslSearchQuery:
   * @query: query to run . If NULL then query is "//search/ *"
   * @tempFile : where do we load the search dataBase from to execute
   *             query. If tempFile is NULL "search.data" is used
   * 
   * Send query as parameter for execution of search.xsl using
   *    data stored in @tempFile 
   * Return 1 on success,
   *        0 otherwise   
   */
    int xslSearchQuery(const xmlChar * tempFile, const xmlChar * query);

/**
 * xslFindBreakPointByLineNo:
 * @ctxt : valid ctxt to look into
 * @url : url non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber : number >= 0 and is available in url specified
 *
 * Find the closest line number in file specified that can be a point 
 * Returns  line number number if successfull,
 *	    0 otherwise
*/
    xmlNodePtr xslFindNodeByLineNo(xsltTransformContextPtr ctxt,
                                   const xmlChar * url, long lineNumber);

/**
 * xslFindTemplateNode: 
 * @style : valid stylesheet collection to look into 
 * @name : template name to look for
 *
 * Returns : template node found if successfull,
 *           NULL otherwise 
 */
    xmlNodePtr xslFindTemplateNode(const xsltStylesheetPtr style,
                                   const xmlChar * name);

/**
 * xslFindBreakPointByName:
 * @templateName : template name to look for
 *
 * Find the breakpoint at template with "match" or "name" equal 
 *    to templateName
 * Returns the break point number given the template name is found
 *          0 otherwise
*/
    xslBreakPointPtr findBreakPointByName(const xmlChar * templateName);


/**
 * xslFindBreakPointById:
 * @id : The break point id to look for
 *
 * Find the break point number for given break point id
 * Returns break point number found for given the break point id,
 *          0 otherwise 
 */
    xslBreakPointPtr findBreakPointById(int id);


/**
 * xslFindNodesByQuery:
 * @query: xpath query to run, see dbgsearch.c for more details
 * 
 * Return the nodes that match the given query on success,
 *        NULL otherwise 
 */
    xmlXPathObjectPtr xslFindNodesByQuery(const xmlChar * query);


/* renamed to avoid clash with libxslt*/

/**
 * walkBreakPoints:
 * @walkFunc: function to callback for each breakpoint found
 * @data : the extra data to pass onto walker
 *
 * Walks through all breakpoints calling walkFunc for each. The payload
 *  sent to walkFunc is of type xslBreakPointPtr 
 */
    void walkBreakPoints(xmlHashScanner walkFunc, void *data);


/**
 * walkTemplates:
 * @walkFunc: function to callback for each template found
 * @data : the extra data to pass onto walker
 * @style : the stylesheet to start from
 *
 * Walks through all templates calling walkFunc for each. The payload
 *   of walkFunc is of type xsltTemplatePtr
 */
    void walkTemplates(xmlHashScanner walkFunc, void *data,
                       xsltStylesheetPtr style);


/**
 * walkStylesheets:
 * @walkFunc: function to callback for each stylesheet found
 * @data : the extra data to pass onto walker
 * @style : the stylesheet to start from
 *
 * Walks through all templates calling walkFunc for each. The payload
 *   sent to walkFuc is of type xsltStylesheetPtr
 */
    void walkStylesheets(xmlHashScanner walkFunc, void *data,
                         xsltStylesheetPtr style);


/**
 * xslWalkGlobals:
 * @walkFunc: function to callback for each variable of type @type found
 * @data : the extra data to pass onto walker
 * @styleCtxt : the stylesheet to start from
 *
 * Call walkFunc for each global variable. The payload
 *   sent to walkFunc is of type  xmlNodePtr
 */
    void
     
        xslWalkGlobals(xmlHashScanner walkFunc,
                       void *data ATTRIBUTE_UNUSED,
                       xsltStylesheetPtr style);

/**
 * xslWalkLocals:
 * @walkFunc: function to callback for each template found
 * @data : the extra data to pass onto walker
 * @style : the stylesheet to start from
 *
 * Walks through all local variables calling walkFunc for each. The payload
 *   of walkFunc is of type xmlNodePtr
 */
    void
     
        xslWalkLocals(xmlHashScanner walkFunc, void *data,
                      xsltStylesheetPtr style);

/**
 * xslWalkIncludes:
 * @walkFunc: function to callback for each xsl:include found
 * @data : the extra data to pass onto walker
 * @style : the stylesheet to start from
 *
 * Walks through all xsl:include calling walkFunc for each. The payload
 *   of walkFunc is of type xmlNodePtr
 */
    void
     
        xslWalkIncludes(xmlHashScanner walkFunc, void *data,
                        xsltStylesheetPtr style);

/**
 * xslWalkChildNodes:
 * @walkFunc: function to callback for each child/sibling found
 * @data : the extra data to pass onto walker
 * @node : valid xmlNodePtr
 *
 * Call walkFunc for each child of @node the payload sent to walkFunc is
 *   a xmlNodePtr
 */
    void xslWalkChildNodes(xmlHashScanner walkFunc, void *data,
                           xmlNodePtr node);

#ifdef __cplusplus
}
#endif
#endif
