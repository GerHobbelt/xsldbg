
/* *************************************************************************
                          xslsearch.h  -  public functions for
                                               searching
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

#ifndef XSLSEARCH_H
#define XSLSEARCH_H

/**
 * Provide a searching support
 *
 * @short search support
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */

#include "xslbreakpoint.h"

#ifdef WITH_XSLT_DEBUG
#ifndef WITH_XSLT_DEBUG_BREAKPOINTS
#define WITH_XSLT_DEBUG_BREAKPOINTS
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

    /* what types of searches are there */
    enum SearchEnum {
        SEARCH_BREAKPOINT = 400,
        SEARCH_NODE,
        SEARCH_XSL
    };

  /** define a common structure to be used when searching */
    typedef struct _searchInfo searchInfo;
    typedef searchInfo *searchInfoPtr;
    struct _searchInfo {
        int found;              /* found is 1 if search is finished */
        int type;               /* what type of search see SearchEnum */
        void *data;             /* extra data to pass to walkFunc */
    };


  /** data to pass to via searchInfoPtr when searching for break points */
    typedef struct _breakPointSearchData breakPointSearchData;
    typedef breakPointSearchData *breakPointSearchDataPtr;
    struct _breakPointSearchData {
        int id;                 /* what id to look for, 
                                 * if -1 then ignore */
        xmlChar *templateName;  /* template to look for 
                                 * if NULL then ignore */
        xslBreakPointPtr breakPoint;    /* the break point found by search */
    };


  /** data to pass via searchInfoPtr when searching for nodes */
    typedef struct _nodeSearchData nodeSearchData;
    typedef nodeSearchData *nodeSearchDataPtr;
    struct _nodeSearchData {
        long lineNo;            /* what line number to look for 
                                 * if < 0 then ignore */
        xmlChar *url;           /* what URl to look for 
                                 * if NULL then ignore */
        int fileSearch;         /* if true then we are trying 
                                 * to match a file name */
        xmlChar *nameInput;     /* what file/node name are we
                                 * trying to match */
        xmlChar *guessedNameMatch;      /* possible name match */
        xmlChar *absoluteNameMatch;     /* full name match */
        xmlNodePtr node;        /* the node that the match 
                                 * occured in */
    };


/**
 * @returns 1 if search structures have been initialized properly and all
 *               memory required has been obtained,
 *          0 otherwise
*/
    int searchInit(void);


/**
 * Free all memory used by searching 
 */
    void searchFree(void);


/**
 * @returns valid search info pointer if successful
 *          NULL otherwise
 *
 * @param type What type of search is required
 */
    searchInfoPtr searchNewInfo(SearchEnum type);


/**
 * Free memory used by @p info
 *
 * @param info A valid search info
 *
 */
    void searchFreeInfo(searchInfoPtr info);


/**
 * Empty the seach data base of its contents
 *
 * @returns 1 on success,
 *          0 otherwise
 */
    int searchEmpty(void);

/**
 * Returns the document used for searching
 *         Dangerous function to use! Does NOT return a copy of 
 *             search data  so don't free it
 */
    xmlDocPtr searchDoc(void);


/**
 * Returns the topmost xml node in search dataBase.
 *         Dangerous function to use! Does NOT return a copy of 
 *             search root node  so don't free it
 */
    xmlNodePtr searchRootNode(void);


/**
 * @returns 1 if able to add @p node to top node in search dataBase,
 *          0 otherwise
 *
 * @param node A valid node to be added to the topmost node in search dataBase
 */
    int xslSearchAdd(xmlNodePtr node);


/**
 * Save the search dataBase to @p fileName  
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param fileName Valid file name
 */
    int xslSearchSave(const xmlChar * fileName);


/**
 * Send query as parameter for execution of search.xsl using
 *    data stored in @p tempFile 
 *
 * @returns 1 on success,
 *          0 otherwise   
 *
 * @param query Query to run. If NULL then @p query defaults to "//search/ *"
 * @param tempFile Where do we load the search dataBase from to execute
 *             query. If @p tempFile is NULL default is "search.data"

 */
    int xslSearchQuery(const xmlChar * tempFile, const xmlChar * query);


/**
 * Returns 1 if able to update the search dataBase,
 *         0 otherwise

 * @param styleCtxt Not used
 * @param style Is valid
 * @param data Not used but MUST be NULL for the moment
 * @param variableTypes What types of variables to look
 */
    int updateSearchData(xsltTransformContextPtr styleCtxt,
                         xsltStylesheetPtr style,
                         void *data, VariableTypeEnum variableTypes);


/**
 * Test if break point matches criteria given by @p data. If so then 
 *      set @p data->found to 1 and stores  reference to break point found in 
 *         @p data->data->node
 *     otherwise @p data is unchanged
 *
 * @param payload A valid xslBreakPointPtr 
 * @param data The criteria to look for and a valid searchInfoPtr of
 *          type SEARCH_BREAKPOINT 
 * @param name Not used 
 *
*/
    void scanForBreakPoint(void *payload, void *data, xmlChar * name);


/**
 * Test if node matches criteria given by @p data if so then 
 *     set @p data->found to 1 and  stores reference to node found in
 *     @p data->data->node.
 *  otherwise @p data is unchanged
 *
 * @param payload A valid xmlNodePtr
 * @param data The criteria to look for and a valid searchInfo of
 *          type SEARCH_NODE 
 * @param name Not used

*/
    void scanForNode(void *payload, void *data, xmlChar * name);


/**
 * Find the closest line number in file specified that can be a point 
 *
 * @returns The node at line number specified if successful,
 *	    NULL otherwise
 *
 * @param ctxt Valid ctxt to look into
 * @param url Non-null, non-empty file name that has been loaded by
 *           debugger
 * @param lineNumber @p lineNumber >= 0 and is available in url specified
*/
    xmlNodePtr xslFindNodeByLineNo(xsltTransformContextPtr ctxt,
                                   const xmlChar * url, long lineNumber);


/**
 * @returns The template node found if successful,
 *          NULL otherwise 
 *
 * @param style Valid stylesheet collection to look into 
 * @param name Valid template name to look for
 */
    xmlNodePtr xslFindTemplateNode(const xsltStylesheetPtr style,
                                   const xmlChar * name);


/**
 * Find the breakpoint at template with "match" or "name" equal 
 *    to templateName
 *
 * @returns The break point that matches @p templateName
 *          NULL otherwise
 *
 * @param templateName Valid template name to look for
*/
    xslBreakPointPtr findBreakPointByName(const xmlChar * templateName);


/**
 * @returns The break point with given the break point id if found,
 *          NULL otherwise 
 *
 * @param id The break point id to look for
 */
    xslBreakPointPtr findBreakPointById(int id);


/**
 * @returns The nodes that match the given query on success,
 *          NULL otherwise 
 *
 * @param query The xpath query to run, see dbgsearch.c or 
 *           search.dtd for more details
 */
    xmlXPathObjectPtr xslFindNodesByQuery(const xmlChar * query);


/**
 * Walks through all break points calling walkFunc for each. The payload
 *  sent to walkFunc is of type xslBreakPointPtr 
 *
 * @param walkFunc The function to callback for each breakpoint found
 * @param data The extra data to pass onto @p walkFunc
 */
    void walkBreakPoints(xmlHashScanner walkFunc, void *data);


/**
 * Walks through all templates found in @p style calling walkFunc for each.
 *   The payload of walkFunc is of type xsltTemplatePtr
 *
 * @param walkFunc The function to callback for each template found
 * @param data The extra data to pass onto @p walkFunc
 * @param style The stylesheet to start from
 */
    void walkTemplates(xmlHashScanner walkFunc, void *data,
                       xsltStylesheetPtr style);


/**
 * Walks through all stylesheets found in @p style calling walkFunc for
 *   each. The payload sent to walkFunc is of type xsltStylesheetPtr
 *
 * @param walkFunc The function to callback for each stylesheet found
 * @param data The extra data to pass onto @p walkFunc
 * @param style The stylesheet to start from
 */
    void walkStylesheets(xmlHashScanner walkFunc, void *data,
                         xsltStylesheetPtr style);


/**
 * Call walkFunc for each global variable found in @p style. The payload
 *   sent to walkFunc is of type xmlNodePtr
 *
 * @param walkFunc The function to callback for each gobal variable found
 * @param data The extra data to pass onto @p walkFunc
 * @param style The stylesheet to start from
 */
    void xslWalkGlobals(xmlHashScanner walkFunc,
                        void *data, xsltStylesheetPtr style);


/**
 * Walks through all local variables found in @p style calling 
 *   walkFunc for each. The payload of walkFunc is of type xmlNodePtr
 *
 * @param walkFunc The function to callback for each local variable found
 * @param data The extra data to pass onto @p walkFunc
 * @param style The stylesheet to start from
 */
    void xslWalkLocals(xmlHashScanner walkFunc, void *data,
                       xsltStylesheetPtr style);


/**
 * Walks through all included stylesheets found in @p style,
 *   calling walkFunc for each. The payload of walkFunc is of
 *   type xmlNodePtr
 *
 * @param walkFunc The function to callback for each included stylesheet
 * @param data The extra data to pass onto @p walkFunc
 * @param style The stylesheet to start from
 */
    void xslWalkIncludes(xmlHashScanner walkFunc, void *data,
                         xsltStylesheetPtr style);


/**
 * Walks through all xsl:include calling walkFunc for each. The payload
 *   of walkFunc is of type xmlNodePtr
 *
 * @param walkFunc The function to callback for each xsl:include instruction found
 * @param data The extra data to pass onto @p walkFunc
 * @param style The stylesheet to start from
 */
    void walkIncludeInst(xmlHashScanner walkFunc, void *data,
                         xsltStylesheetPtr style);


/**
 * Call walkFunc for each child of @p node the payload sent to walkFunc is
 *   a xmlNodePtr
 *
 * @param walkFunc The function to callback for each child/sibling found
 * @param data The extra data to pass onto @p walkFunc
 * @param node Valid xmlNodePtr
 */
    void xslWalkChildNodes(xmlHashScanner walkFunc, void *data,
                           xmlNodePtr node);



/**
 * @returns @p breakPoint as a new xmlNode in search dataBase format 
 *               if successful,
 *          NULL otherwise
 *
 * @param breakPoint A valid break point 
 */
    xmlNodePtr searchBreakPointNode(xslBreakPointPtr breakPoint);


/**
 * @returns @p templNode as a new xmlNode in search dataBase format 
 *               if successful,
 *          NULL otherwise
 *
 * @param templNode A valid template node
 */
    xmlNodePtr searchTemplateNode(xmlNodePtr templNode);


/** 
 * @returns @p globalVariable as a new xmlNode in search dataBase 
 *               format if successful,
 *          NULL otherwise
 *
 * @param globalVariable A valid xmlNodePtr node
 * 
 */
    xmlNodePtr searchGlobalNode(xmlNodePtr globalVariable);


/** 
 * @returns @p localVariable as a new xmlNode in search dataBase 
 *              format if successful,
 *         NULL otherwise
 *
 * @param localVariable A valid xmlNodePtr node
 * 
 */
    xmlNodePtr searchLocalNode(xmlNodePtr localVariable);


/**
 * @returns @p style as a new xmlNode in search dataBase format if successful,
 *         NULL otherwise
 *
 * @param style A valid stylesheet
 */
    xmlNodePtr searchSourceNode(xsltStylesheetPtr style);


/**
 * @returns @p include as a new xmlNode in search dataBase format
 *              if successful,
 *         NULL otherwise
 *
 * @param include A valid xsl:include instruction
 * 
 */
    xmlNodePtr searchIncludeNode(xmlNodePtr include);


#ifdef __cplusplus
}
#endif
#endif
