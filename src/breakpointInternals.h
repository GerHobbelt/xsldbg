
/***************************************************************************
                          breakpointInternal.h  -  Internal routines not 
                                                   normaly used outside 
                                                   of the debugger
                                                   . All will change the 
                                                    change data structures 
                                                   upon which the debugger
                                                   relys. 
                             -------------------
    begin                : Tue Nov 20 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

#ifndef BREAKPOINTINTERNAL_H
#define BREAKPOINTINTERNAL_H


/* awful hack to get rid of debugging messages */
#ifndef WITH_XSLDBG_DEBUG
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
#undef WITH_XSLT_DEBUG_BREAKPOINTS
#endif
#endif

#include "breakpoint.h"
#include "arraylist.h"

#ifdef __cplusplus
extern "C" {
#endif


/*Indicate what type of variable to print out. 
  Is used by print_variable and searching functions */
    typedef enum {
        DEBUG_GLOBAL_VAR = 200, /* pick a unique starting point */
        DEBUG_LOCAL_VAR,
        DEBUG_ANY_VAR
    } VariableTypeEnum;

/*
-----------------------------------------------------------
             Break point related functions
----------------------------------------------------------
*/

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


/**
 * xslBreakPointCount:
 *
 * Returns the number of hash tables of break points with the same line number
 */
    int xslBreakPointLinesCount(void);


/**
 * xslBreakPointLinesList:
 *
 * Returns the list of hash tables for break points
 *        Dangerous function to use!! 
 */
    ArrayListPtr xslBreakPointLineList(void);


/**
 * xslDbgShellEnable:
 * @arg: is valid
 * @enableType: enable break point if 1, disable if 0, toggle if -1
 *
 * Enable/disable break point specified by arg using enable 
 *      type of @enableType
 * Returns 1 if successful,
 *         0 otherwise
 */
    int xslDbgShellEnable(xmlChar * arg, int enableType);


/*
-----------------------------------------------------------
       Main debugger functions
-----------------------------------------------------------
*/


/* Empty for the moment*/



/*
------------------------------------------------------
                  Xsl call stack related
-----------------------------------------------------
*/

/**
 * callStackInit:
 *
 * Returns 1 if callStack has been initialized properly and all
 *               memory required has been obtained,
 *         0 otherwise
*/
    int callStackInit(void);


/**
 * callStackFree:
 *
 *
 * Free all memory used by callStack
 */
    void callStackFree(void);

/*
-----------------------------------------------------------
             Search related functions
----------------------------------------------------------
*/

/* what types of searches are there */
    typedef enum {
        SEARCH_BREAKPOINT = 400,        /* pick a unique starting point */
        SEARCH_NODE,
        SEARCH_XSL
    } SearchEnum;

    /* define a common structure to be used when searching */
    typedef struct _searchInfo searchInfo;
    typedef searchInfo *searchInfoPtr;
    struct _searchInfo {
        int found;              /* allow the walkFunc to indicate 
                                 * that its finished */
        int type;               /* what type of search are we 
                                 * @see : SearchEnum */
        void *data;
    };


    /* data to pass to via searchInfoPtr when searching for break points */
    typedef struct _breakPointSearchData breakPointSearchData;
    typedef breakPointSearchData *breakPointSearchDataPtr;
    struct _breakPointSearchData {
        int id;
        xmlChar *templateName;
        xslBreakPointPtr breakPoint;
    };


    /* data to pass via searchInfoPtr when searching for nodes */
    typedef struct _nodeSearchData nodeSearchData;
    typedef nodeSearchData *nodeSearchDataPtr;
    struct _nodeSearchData {
        long lineNo;
        xmlChar *url;
        int fileSearch;         /* if true then we are trying 
                                 * to match a file name */
        xmlChar *nameInput;     /*needed for matching names of 
                                 * files/node names */
        xmlChar *guessedNameMatch;
        xmlChar *absoluteNameMatch;
        xmlNodePtr node;
    };


/**
 * searchNewInfo:
 * @type: what type of search is required
 * 
 * Returns valid search info pointer if succssfull
 *         NULL otherwise
 */
    searchInfoPtr searchNewInfo(SearchEnum type);


/**
 * searchFreeInfo:
 * @info: valid search info
 *
 * Free memory used by @info
 */
    void searchFreeInfo(searchInfoPtr info);


/**
 * seachInit:
 *
 * Returns 1 if search structures have been initialized properly and all
 *               memory required has been obtained,
 *         0 otherwise
*/
    int searchInit(void);


/**
 * searchFree:
 *
 * Free all memory used by searching 
 */
    void searchFree(void);


/**
 * searchEmpty:
 *
 * Empty the seach data base of its contents
 * Returns 1 on success,
 *         0 otherwise
 */
    int searchEmpty(void);


/**
 * xslSearchAdd:
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
 * searchDoc:
 *
 * Returns the document used for searching
 *         Dangerous function to use! Does NOT return a copy of 
 *             search data  so don't free it
 */
    xmlDocPtr searchDoc(void);


/**
 * searchRootNode:
 *
 * Returns the topmost xml node in search dataBase.
 *         Dangerous function to use! Does NOT return a copy of 
 *             search root node  so don't free it
 */
    xmlNodePtr searchRootNode(void);


/**
 * searchBreakPointNode:
 * @breakPoint: valid break point 
 *
 * Returns @breakPoint as a new xmlNode in search dataBase format 
 *               if successful,
 *         NULL otherwise
 */
    xmlNodePtr searchBreakPointNode(xslBreakPointPtr breakPoint);


/**
 * searchTemplateNode:
 * @templNode: valid template node
 * 
 * Returns @templNode as a new xmlNode in search dataBase format 
 *               if successful,
 *         NULL otherwise
 */
    xmlNodePtr searchTemplateNode(xmlNodePtr templNode);


/** 
 * searchGlobalNode:
 * @variable: valid  xmlNodePtr node
 * 
 * Returns @style as a new xmlNode in search dataBase format if successful,
 *         NULL otherwise
 */
    xmlNodePtr searchGlobalNode(xmlNodePtr variable);


/** 
 * searchLocalNode:
 * @variable: valid xmlNodePtr node
 * 
 * Returns @style as a new xmlNode in search dataBase format if successful,
 *         NULL otherwise
 */
    xmlNodePtr searchLocalNode(xmlNodePtr variable);


/**
 * searchSourceNode:
 * @style: valid stylesheet
 * 
 * Returns @style as a new xmlNode in search dataBase format if successful,
 *         NULL otherwise
 */
    xmlNodePtr searchSourceNode(xsltStylesheetPtr style);


/**
 * searchIncludeNode:
 * @include: valid xsl:include instruction
 * 
 * Returns @include as a new xmlNode in search dataBase format if successful,
 *         NULL otherwise
 */
    xmlNodePtr searchIncludeNode(xmlNodePtr include);


/**
 * searchCallStackNode:
 * @callStackItem: valid callStack item
 * 
 * Returns @callStackItem as a new xmlNode in search dataBase format 
 *            if successful,
 *         NULL otherwise
 */
    xmlNodePtr searchCallStackNode(xslCallPointPtr callStackItem);


/**
 * walkLocals:
 * @walkFunc: function to callback for each template found
 * @data: the extra data to pass onto walker
 * @style: the stylesheet to start from
 *
 * Walks through all templates calling walkFunc for each. The payload
 *     of walkFunc is of type xmlNodePtr.
 */
    void walkLocals(xmlHashScanner walkFunc, void *data,
                    xsltStylesheetPtr style);


/**
 * walkIncludes:
 * @walkFunc: function to callback for each included stylesheet
 * @data: the extra data to pass onto walker
 * @style: the stylesheet to start from
 *
 * Walks through all included stylesheets calling walkFunc for each. 
 *    The payload of walkFunc is of type xmlNodePtr
 */
    void walkIncludes(xmlHashScanner walkFunc, void *data,
                      xsltStylesheetPtr style);


/**
 * walkIncludeInst:
 * @walkFunc: function to callback for each xsl:include instruction found
 * @data: the extra data to pass onto walker
 * @style: the stylesheet to start from
 *
 * Walks through all xsl:include's calling walkFunc for each. The payload
 *   of walkFunc is of type xmlNodePtr.
 */
    void walkIncludeInst(xmlHashScanner walkFunc, void *data,
                         xsltStylesheetPtr style);


/**
 * walkGlobals:
 * @walkFunc: function to callback for each variable of type @type found
 * @data: the extra data to pass onto walker
 * @styleCtxt: the stylesheet to start from
 *
 * Call walkFunc for each global variable. The payload
 *   sent to walkFunc is of type xmlNodePtr.
 */
    void walkGlobals(xmlHashScanner walkFunc, void *data,
                     xsltStylesheetPtr style);


/**
 * walkChildNodes:
 * @walkFunc: function to callback for each child/sibling found
 * @data: the extra data to pass onto walker
 * @node: valid xmlNodePtr
 *
 * Call walkFunc for each child/sibling of @node the payload sent 
 *     to walkFunc is  a xmlNodePtr
 */
    void walkChildNodes(xmlHashScanner walkFunc, void *data,
                        xmlNodePtr node);


/**
 *  scanForBreakPoint: 
 * @payload: a valid xslBreakPointPtr 
 * @data: the criteria to look for and a valid searchInfoPtr of
 *          type SEARCH_BREAKPOINT 
 * @name: not used 
 *
 * Test if break point matches criteria given by @data. If so then 
 *      set @data->found to 1 and stores  reference to break point found in 
 *         @data->data->node
 *     otherwise @data is unchanged
*/
    void scanForBreakPoint(void *payload, void *data, xmlChar * name);


/**
 *  scanForNode: 
 * @payload: a valid xmlNodePtr
 * @data: the criteria to look for and a valid searchInfo of
 *          type SEARCH_NODE 
 * @name: not used

 * Test if node matches criteria given by @data if so then set @data->found 
 *   to 1 and  stores reference to node found in @data->data->node
 *     otherwise @data is unchanged
 *
*/
    void scanForNode(void *payload, void *data, xmlChar * name);


/**
 * updateSearchData:
 * @styleCtxt: not used
 * @style: is valid
 * @data: not used but MUST be NULL for the moment
 * @variableTypes: what types of variables to look
 *
 * Returns 1 on success,
 *         0 otherwise
 */
    int updateSearchData(xsltTransformContextPtr styleCtxt,
                         xsltStylesheetPtr style,
                         void *data, VariableTypeEnum variableTypes);


#ifdef __cplusplus
}
#endif
#endif
