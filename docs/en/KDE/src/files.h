
/***************************************************************************
                          files.h  -  define file related functions
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






/**
 * Provide a searching support
 *
 * @short search support
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */


/* We want skip most of these includes when building documentation*/







    /* what types of searches are there */

    /* keep kdoc happy */
    enum SearchEnum {
        SEARCH_BREAKPOINT = 400,
        SEARCH_NODE,
        SEARCH_XSL,
        SEARCH_VARIABLE
    };


    /* define a common structure to be used when searching */
    typedef struct _searchInfo searchInfo;
    typedef searchInfo *searchInfoPtr;
    struct _searchInfo {
        int found;              /* found is 1 if search is finished */
        int type;               /* what type of search see SearchEnum */
        int error;              /* did an error occur */
        void *data;             /* extra data to pass to walkFunc */
    };


    /* data to pass to via searchInfoPtr when searching for break points */
    typedef struct _breakPointSearchData breakPointSearchData;
    typedef breakPointSearchData *breakPointSearchDataPtr;
    struct _breakPointSearchData {
        int id;                 /* what id to look for, 
                                 * if -1 then ignore */
        xmlChar *templateName;  /* template to look for 
                                 * if NULL then ignore */
        xslBreakPointPtr breakPoint;    /* the break point found by search */
    };


    /* data to pass via searchInfoPtr when searching for nodes */
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

    /* data to pass to via searchInfoPtr when searching for variables points */
    typedef struct _variableSearchData variableSearchData;
    typedef variableSearchData *variableSearchDataPtr;
    struct _variableSearchData {
        xmlChar *name;
        xmlChar *nameURI;
        xmlChar *select;        /* new value to adopt if any */
    };




/**
 * Initialize the search module
 *
 * @returns 1 if search structures have been initialized properly and all
 *               memory required has been obtained,
 *          0 otherwise
*/


    int searchInit(void);





/**
 * Free all memory used by the search module
 */


    void searchFree(void);





/**
 * Create a new search
 *
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
 * Empty the seach dataBase of its contents
 *
 * @returns 1 on success,
 *          0 otherwise
 */


    int searchEmpty(void);





/** 
 * Return the document used for seaching ie the search dataBase
 *
 * @returns the document used for searching
 *         Dangerous function to use! Does NOT return a copy of 
 *             search data  so don't free it
 */


    xmlDocPtr searchDoc(void);





/**
 * Get the topmost node in the search dataBase
 *
 * @returns The topmost xml node in search dataBase.
 *         Dangerous function to use! Does NOT return a copy of 
 *             search root node  so don't free it
 */


    xmlNodePtr searchRootNode(void);





/**
 * Add a node to the search dataBase
 *
 * @returns 1 if able to add @p node to top node in search dataBase,
 *          0 otherwise
 *
 * @param node Is valid
 */


    int searchAdd(xmlNodePtr node);





/**
 * Save the search dataBase to @p fileName  
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param fileName Valid file name
 */


    int searchSave(const xmlChar * fileName);





/**
 * Send query as parameter for execution of search.xsl using
 *    data stored in @p tempFile 
 *
 * @returns 1 on success,
 *          0 otherwise   
 *
 * @param query The Query to run. If NULL then @p query defaults to "//search/ *"
 * @param tempFile Where do we load the search dataBase from to execute
 *             query. If @p tempFile is NULL default is "search.data"
 * @param outputFile Where do we store the result. If NULL
 *             then default to  "searchresult.html"
 */


    int searchQuery(const xmlChar * tempFile, const xmlChar * outputFile,
                    const xmlChar * query);





/**
 * Update the search dataBase
 * 
 * @returns 1 if able to update the search dataBase,
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
 * @param lineNumber @p lineNumber >= 0 and is available in @p url
*/


    xmlNodePtr findNodeByLineNo(xsltTransformContextPtr ctxt,
                                const xmlChar * url, long lineNumber);





/**
 * Find a template node
 *
 * @returns The template node found if successful,
 *          NULL otherwise 
 *
 * @param style A Valid stylesheet collection to look into 
 * @param name Valid template name to look for
 */


    xmlNodePtr findTemplateNode(const xsltStylesheetPtr style,
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
 * Find a break point by its id
 *
 * @returns The break point with given the break point id if found,
 *          NULL otherwise 
 *
 * @param id The break point id to look for
 */


    xslBreakPointPtr findBreakPointById(int id);





/**
 * Find nodes in search dataBase using an xpath query
 *
 * @returns The nodes that match the given query on success,
 *          NULL otherwise 
 *
 * @param query The xpath query to run, see dbgsearch.c or 
 *           search.dtd for more details
 */


    xmlXPathObjectPtr findNodesByQuery(const xmlChar * query);





/**
 * Walks through all break points calling walkFunc for each. The payload
 *  sent to walkFunc is of type xslBreakPointPtr 
 *
 * @param walkFunc The function to callback for each break point found
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


    void walkGlobals(xmlHashScanner walkFunc,
                     void *data, xsltStylesheetPtr style);





/**
 * Walks through all local variables found in @p style calling 
 *   walkFunc for each. The payload of walkFunc is of type xmlNodePtr
 *
 * @param walkFunc The function to callback for each local variable found
 * @param data The extra data to pass onto @p walkFunc
 * @param style The stylesheet to start from
 */


    void walkLocals(xmlHashScanner walkFunc, void *data,
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


    void walkIncludes(xmlHashScanner walkFunc, void *data,
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


    void walkChildNodes(xmlHashScanner walkFunc, void *data,
                        xmlNodePtr node);






/**
 * Convert @p breakPoint into search dataBase format
 *
 * @returns @p breakPoint as a new xmlNode in search dataBase format 
 *               if successful,
 *          NULL otherwise
 *
 * @param breakPoint Is valid
 */


    xmlNodePtr searchBreakPointNode(xslBreakPointPtr breakPoint);





/**
 * Convert @p templateNode into search dataBase format
 *
 * @returns @p templNode as a new xmlNode in search dataBase format 
 *               if successful,
 *          NULL otherwise
 *
 * @param templNode A valid template node
 */


    xmlNodePtr searchTemplateNode(xmlNodePtr templNode);





/**
 * Convert @p globalVariable into search dataBase format
 *
 * @returns @p globalVariable as a new xmlNode in search dataBase 
 *               format if successful,
 *          NULL otherwise
 *
 * @param globalVariable A valid xmlNodePtr node
 * 
 */


    xmlNodePtr searchGlobalNode(xmlNodePtr globalVariable);





/** 
 * Convert @p localVariable into search dataBase format
 *
 * @returns @p localVariable as a new xmlNode in search dataBase 
 *              format if successful,
 *         NULL otherwise
 *
 * @param localVariable Is valid
 * 
 */


    xmlNodePtr searchLocalNode(xmlNodePtr localVariable);





/**  
* Convert @p style into search dataBase format
*
 * @returns @p style as a new xmlNode in search dataBase format if successful,
 *         NULL otherwise
 *
 * @param style Is valid
 */


    xmlNodePtr searchSourceNode(xsltStylesheetPtr style);





/**
  * Convert @p include into search dataBase format
  *
 * @returns @p include as a new xmlNode in search dataBase format
 *              if successful,
 *         NULL otherwise
 *
 * @param include Is a valid xsl:include instruction
 * 
 */


    xmlNodePtr searchIncludeNode(xmlNodePtr include);





  /**
   *Convert @p include into search dataBase format
   *
   * @returns @p callStackItem as a new xmlNode in search dataBase
   *               format if successful,
   *            NULL otherwise  
   * @param callStackItem Is valid
   */


    xmlNodePtr searchCallStackNode(xslCallPointPtr callStackItem);







/**************************************************************************
                          arraylist.h  -  declare the functions for 
                                        implementation of the array list
                             -------------------
    begin                : Sat Nov 10 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 **************************************************************************/

/**************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 **************************************************************************/






/**
 * Provide a fast easy to use array list. Support the basic functions of add
 *  delete, empty, count, free
 *
 * @short Array list support
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */











    typedef void (*freeItemFunc) (void *item);
    /* A dynamic structure behave like a list */
    typedef struct _ArrayList ArrayList;
    typedef ArrayList *ArrayListPtr;
    struct _ArrayList {
        int size, count;
        void **data;
        freeItemFunc deleteFunction;
    };

/* what size of the list do we stop automatic doubling of capacity
    if array list size growth is needed */







/**
 * Create a new list with a size of @p initialSize
 *
 * @returns Non-null on success,
 *          NULL otherwise
 *
 * @param initial The initial size of list
 * @param deleteFunction the Function to call to free items in the list
 */


    ArrayListPtr arrayListNew(int initialSize,
                              freeItemFunc deleteFunction);




/**
 * Free memory assocated with array list, if the array list 
 *   has a valid deleteFunction then content with be freed with 
 *    using that deleteFunction
 *
 * @param list A valid list
 */


    void arrayListFree(ArrayListPtr list);






/**
 * Empties the list of its content
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param list A valid list
 */


    int arrayListEmpty(ArrayListPtr list);





/**
 * Return the maximum number elements this list can contain
 *
 * @returns The maximum number elements this list can contain
 *
 * @param list A valid list
 */


    int arrayListSize(ArrayListPtr list);





/**
 * Return the count of number items in list
 * @returns The count of number items in list
 *
 * @param list A valid list
 */



    int arrayListCount(ArrayListPtr list);





/**
 * Add @p item to @p list
 *
 * @returns 1 if able to add @p item to end of @p list,
 *          0 otherwise
 *
 * @param list A valid list
 * @param item A valid item
 */


    int arrayListAdd(ArrayListPtr list, void *item);





/**
 * @returns 1 if able to delete element in @p list at position @p position,
 *          0 otherwise 
 *
 * @param list A valid list
 * @param position  0 =< @p position < arrayListCount(@p list)
 */


    int arrayListDelete(ArrayListPtr list, int position);





/**
 * @returns Non-null if able to retrieve element in @p list at position
 *          @p position,
 *         NULL otherwise
 *
 * @param list A valid list
 * @param position  0 =< @p position < arrayListCount(@p list)
 */


    void *arrayListGet(ArrayListPtr list, int position);








/**
 * Provide a file support
 *
 * @short file support
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */






    extern FILE *terminalIO;



    typedef struct _entityInfo entityInfo;
    typedef entityInfo *entityInfoPtr;
    struct _entityInfo {
        xmlChar *SystemID;
        xmlChar *PublicID;
    };



  /*-----------------------------------------------------------
    File commands
    -----------------------------------------------------------*/






    int xslDbgEntities(void);





  /**
   * Print what a system file @p arg maps to via the current xml catalog
   *
   * @param arg Is valid in UTF-8
   * 
   * @returns 1 on sucess,
   *          0 otherwise
   */


    int xslDbgSystem(const xmlChar * arg);





  /**
   * Print what a public ID @p arg maps to via the current xml catalog
   *
   * @param arg Is valid PublicID in UTF-8
   * 
   * @returns 1 on sucess,
   *          0 otherwise
   */


    int xslDbgPublic(const xmlChar * arg);





  /**
   * Set current encoding to use for output to standard output
   *
   * @param arg Is valid encoding supported by libxml2
   *
   *
   * Returns 1 on sucess,
   */


    int xslDbgEncoding(xmlChar * arg);




  /*-----------------------------------------------------------
    General function for working with files
    -----------------------------------------------------------*/




  /**
   * Fixes the nodes from firstNode to lastNode so that debugging can occur
   *
   * @param uri Is valid
   * @param firstNode Is valid
   * @param lastNode Is Valid
   */


    void filesEntityRef(xmlEntityPtr ent, xmlNodePtr firstNode,
                        xmlNodePtr lastNode);





  /**
   * Return the list entity names used for documents loaded
   *
   * @returns The list entity names used for documents loaded
   */


    ArrayListPtr filesEntityList(void);





  /**
   * Set the base uri for this node. Function is used when xml file
   *    has external entities in its DTD
   * 
   * @param node Is valid and has a doc parent
   * @param uri Is Valid
   * 
   * @returns 1 if successful,
   *          0 otherwise
   */


    int filesSetBaseUri(xmlNodePtr node, const xmlChar * uri);





  /**
   * Get a copy of the base uri for this node. Function is most usefull 
   *  used when xml file  has external entities in its DTD
   * 
   * @param node : Is valid and has a doc parent
   * 
   * @returns The a copy of the base uri for this node,
   *          NULL otherwise
   */


    xmlChar *filesGetBaseUri(xmlNodePtr node);





  /**
   * Return the name of tempfile requested.
   * @param fleNumber : Number of temp file required
   *     where @p fileNumber is 
   *      0 : file name used by cat command
   *      1 : file name used by profiling output
   *
   *  This is a platform specific interface
   *
   * Returns The name of temp file to be used for temporary results if sucessful,
   *         NULL otherwise
   */


    const char *filesTempFileName(int fileNumber);





  /*-----------------------------------------------------------
    Platform specific file functions
    -----------------------------------------------------------*/





  /**
   * Intialize the platform specific files module
   *
   *  This is a platform specific interface
   *
   * @returns 1 if sucessful
   *          0 otherwise  
   */


    int filesPlatformInit(void);





  /**
   * Free memory used by the platform specific files module
   *
   *  This is a platform specific interface
   *
   */


    void filesPlatformFree(void);



  /*--------------------------------
    Misc functions
    ----------------------------------*/





  /**
   * Load the catalogs specifed by OPTIONS_CATALOG_NAMES if 
   *      OPTIONS_CATALOGS is enabled
   *
   * @returns 1 if sucessful
   *          0 otherwise   
   */


    int filesLoadCatalogs(void);





  /**
   * Return  A  string of converted @text
   *
   * @param text Is valid, text to translate from UTF-8, 
   *
   * Returns  A  string of converted @text, may be NULL
   */


    xmlChar *filesEncode(const xmlChar * text);





  /**
   * Return  A  string of converted @text
   *
   * @param test Is valid, text to translate from current encoding to UTF-8, 
   *
   * Returns  A  string of converted @text, may be NULL
   */


    xmlChar *filesDecode(const xmlChar * text);





  /**
   * Opens encoding for all standard output to @p encoding. If  @p encoding 
   *        is NULL then close current encoding and use UTF-8 as output encoding
   *
   * @param encoding Is a valid encoding supported by the iconv library or NULL
   *
   * Returns 1 if successful in setting the encoding of all standard output
   *           to @p encoding
   *         0 otherwise
   */


    int filesSetEncoding(const char *encoding);






    /* used by loadXmlFile, freeXmlFile functions */
    enum FileTypeEnum {
        FILES_XMLFILE_TYPE = 100,       /* pick a unique starting point */
        FILES_SOURCEFILE_TYPE,
        FILES_TEMPORARYFILE_TYPE
    };






  /**
   * Open communications to the terminal device @p device
   *
   * @param device Terminal to redirect i/o to , will not work under win32
   *
   * @returns 1 if sucessful
   *          0 otherwise
   */


    int openTerminal(xmlChar * device);






  /**
   * Select the terminal for i/o
   *
   * @returns 1 if able to use prevously opened terminal 
   *          0 otherwise
   */


    int selectTerminalIO(void);





  /** 
   * Select standard i/o
   *
   * @returns 1 if able to select orginal stdin, stdout, stderr
   *          0 otherwise
   */


    int selectNormalIO(void);





  /**
   * Try to find a matching stylesheet name
   * Sets the values in @p searchinf depending on outcome of search
   *
   * @param searchInf Is valid
   */


    void guessStylesheetName(searchInfoPtr searchInf);






  /**
   * Return the base path for the top stylesheet ie
   *        ie URL minus the actual file name
   *
   * @returns The base path for the top stylesheet ie
   *        ie URL minus the actual file name
   */


    xmlChar *stylePath(void);






  /** 
   * Return the working directory as set by changeDir function
   *
   * @return the working directory as set by changeDir function
   */


    xmlChar *workingPath(void);






  /**
   * Change working directory to path 
   *
   * @param path The operating system path(directory) to adopt as 
   *         new working directory
   *
   * @returns 1 on success,
   *          0 otherwise
   */


    int changeDir(const xmlChar * path);





  /**
   * Load specified file type, freeing any memory previously used 
   *
   * @returns 1 on success,
   *         0 otherwise 
   *
   * @param path The xml file to load
   * @param fileType A valid FileTypeEnum
   */


    int loadXmlFile(const xmlChar * path, FileTypeEnum fileType);





  /**
   * Free memory associated with the xml file 
   *
   * @returns 1 on success,
   *         0 otherwise
   *
   * @param fileType : A valid FileTypeEnum
   */


    int freeXmlFile(FileTypeEnum fileType);





  /**
   * Return the topmost stylesheet 
   *
   * @returns Non-null on success,
   *         NULL otherwise
   */


    xsltStylesheetPtr getStylesheet(void);





  /** 
   * Return the current "temporary" document
   *
   * @returns non-null on success,
   *          NULL otherwise
   */


    xmlDocPtr getTemporaryDoc(void);





  /** 
   * Return the main docment
   *
   * @returns the main document
   */


    xmlDocPtr getMainDoc(void);





  /**
   * @returns 1 if stylesheet or its xml data file has been "flaged" as reloaded,
   *         0 otherwise
   *
   * @param reloaded If = -1 then ignore @p reloaded
   *             otherwise change the status of files to value of @p reloaded
   */


    int filesReloaded(int reloaded);





  /**
   * Initialize the file module
   *
   * @returns 1 on success,
   *          0 otherwise
   */


    int filesInit(void);





  /**
   * Free memory used by file related structures
   */


    void filesFree(void);







    int isSourceFile(xmlChar * fileName);




