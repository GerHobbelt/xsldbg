
/***************************************************************************
                          dbgsearch.c  -  description
                             -------------------
    begin                : Fri Nov 2 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/


#include "xsldbg.h"
#include "debugXSL.h"
#include "xslbreakpoint.h"
#include "xslsearch.h"
#include "options.h"

#ifdef __riscos
/* Include for filename conversions */
#include "libxml/riscos.h"
#endif

/* our private function*/
void scanForBreakPoint(void *payload, void *data,
                       xmlChar * name ATTRIBUTE_UNUSED);

/* store all data in this document so we can write it to file*/
static xmlDocPtr searchDataBase;

/* the topmost node in document*/
static xmlNodePtr searchDataBaseRoot;

/* what was the last query that was run */
static xmlChar *lastQuery;

#define BUFFER_SIZE 500
static char buff[BUFFER_SIZE];

/* -----------------------------------------
   Private function declarations for dbgsearch.c
 -------------------------------------------*/

/**
 * findNodeByLineNoHelper:
 * @payload: valid xsltStylesheetPtr
 * @data: valid searchInfoPtr
 * @name: not used
 *
 * We are walking through stylesheets looking for a match 
 */
void
  findNodeByLineNoHelper(void *payload, void *data,
                         xmlChar * name ATTRIBUTE_UNUSED);

/**
 * globalVarHelper:
 * @payload: valid xsltStylesheetPtr
 * @data: is valid
 * @name: not used
 *
 * Helper to find the global variables. We are given control via
 *   walkStylesheets globalWalkFunc will always be set to the
 *    walkFunc to call
 */
void
  globalVarHelper(void **payload, void *data ATTRIBUTE_UNUSED,
                  xmlChar * name ATTRIBUTE_UNUSED);

/**
 * localVarHelper:
 * @payload: valid xsltTemplatePtr
 * @data: is valid
 * @name: not used
 *
 * Helper to find the local variables. We are given control via walkTemplates
 *    globalWalkFunc will always be set to the walkFunc to call
 *   localWalkFunc will always be set to the walkFunc to call
 */
void
  localVarHelper(void **payload, void *data ATTRIBUTE_UNUSED,
                 xmlChar * name ATTRIBUTE_UNUSED);


/* ------------------------------------- 
    End private functions
    ---------------------------------------*/

/**
 * searchNewInfo:
 * @type: What type of search is required
 * 
 * Create a new search
 *
 * Returns A valid search info pointer if successful
 *         NULL otherwise
 */
searchInfoPtr
searchNewInfo(SearchEnum type)
{
    searchInfoPtr result = NULL;
    int searchType = type;

    switch (searchType) {
        case SEARCH_BREAKPOINT:
            result = (searchInfoPtr) xmlMalloc(sizeof(searchInfo));
            if (result) {
                breakPointSearchDataPtr searchData;

                result->type = SEARCH_BREAKPOINT;
                searchData = (breakPointSearchDataPtr)
                    xmlMalloc(sizeof(breakPointSearchData));
                if (searchData) {
                    searchData->id = -1;
                    searchData->templateName = NULL;
                    searchData->breakPoint = NULL;
                    result->data = searchData;
                } else {
                    xmlFree(result);
                    result = NULL;
                }
            }
            break;

        case SEARCH_NODE:
            result = (searchInfoPtr) xmlMalloc(sizeof(searchInfo));
            if (result) {
                nodeSearchDataPtr searchData;

                result->type = SEARCH_NODE;
                searchData =
                    (nodeSearchDataPtr) xmlMalloc(sizeof(nodeSearchData));
                if (searchData) {
                    searchData->node = NULL;
                    searchData->lineNo = -1;
                    searchData->url = NULL;
                    searchData->nameInput = NULL;
                    searchData->guessedNameMatch = NULL;
                    searchData->absoluteNameMatch = NULL;
                    result->data = searchData;
                } else {
                    xmlFree(result);
                    result = NULL;
                }
            }
            break;

        case SEARCH_XSL:
            break;

    case SEARCH_VARIABLE:
            result = (searchInfoPtr) xmlMalloc(sizeof(searchInfo));
            if (result) {
                variableSearchDataPtr searchData;

                result->type = SEARCH_VARIABLE;
                searchData = (variableSearchDataPtr)
                    xmlMalloc(sizeof(variableSearchData));
                if (searchData) {
                    searchData->name = NULL;		    
                    searchData->nameURI = NULL;
                    searchData->select = NULL;
                    result->data = searchData;
                } else {
                    xmlFree(result);
                    result = NULL;
                }
            }
      break;

    }
    if (result){
        result->found = 0;
	result->error = 0;
    }
    return result;
}


/**
 * searchFreeInfo:
 * @info: valid search info
 *
 * Free memory used by @info
 */
void
searchFreeInfo(searchInfoPtr info)
{
    if (info) {
        if (info->data) {
            switch (info->type) {
                case SEARCH_BREAKPOINT:
                    {
                        breakPointSearchDataPtr searchData =
                            (breakPointSearchDataPtr) info->data;

                        if (searchData->templateName)
                            xmlFree(searchData->templateName);
                    }
                    break;

                case SEARCH_NODE:
                    {
                        nodeSearchDataPtr searchData =
                            (nodeSearchDataPtr) info->data;

                        if (searchData->url)
                            xmlFree(searchData->url);

                        if (searchData->nameInput)
                            xmlFree(searchData->nameInput);

                        if (searchData->guessedNameMatch)
                            xmlFree(searchData->guessedNameMatch);

                        if (searchData->absoluteNameMatch)
                            xmlFree(searchData->absoluteNameMatch);

                        /* we never free searchData->node as we did not create it! */
                    }
                    break;

                case SEARCH_XSL:
                    break;

                case SEARCH_VARIABLE:
                    {
                        variableSearchDataPtr searchData =
                            (variableSearchDataPtr) info->data;

                        if (searchData->name)
                            xmlFree(searchData->name);

                        if (searchData->nameURI)
                            xmlFree(searchData->nameURI);

                        if (searchData->select)
                            xmlFree(searchData->select);
                    }
                    break;

            }
            xmlFree(info->data);
        }
        xmlFree(info);
    }
}


/**
 * searchInit:
 *
 * Initialize the search module
 *
 * Returns 1 if search structures have been initialized properly and all
 *               memory required has been obtained,
 *         0 otherwise
*/
int
searchInit(void)
{
    searchDataBase = NULL;
    searchDataBaseRoot = NULL;
    lastQuery = NULL;
    if (!searchEmpty()) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xmlGenericError(xmlGenericErrorContext,
                        "Search init failed : memory error\n");
#endif
    }
    return (searchRootNode() != NULL);
}


/**
 * searchFree:
 *
 * Free all memory used by the search module
 */
void
searchFree(void)
{
    if (searchDataBase) {
        xmlFreeDoc(searchDataBase);
        searchDataBase = NULL;
	searchDataBaseRoot = NULL;
    }
}


/**
 * searchEmpty:
 *
 * Empty the seach dataBase of its contents
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
searchEmpty(void)
{
    if (searchDataBase) {
        xmlFreeDoc(searchDataBase);
    }
    searchDataBase = xmlNewDoc((xmlChar *) "1.0");
    searchDataBaseRoot = NULL;
    if (searchDataBase) {
        xmlCreateIntSubset(searchDataBase,
                           (xmlChar *) "search",
                           (xmlChar *) "-//xsldbg//DTD search XML V1.0//EN",
                           (xmlChar *) "search.dtd");
        searchDataBaseRoot = xmlNewNode(NULL, (xmlChar *) "search");
        if (searchDataBaseRoot)
            xmlAddChild((xmlNodePtr) searchDataBase, searchDataBaseRoot);
    }
    if (lastQuery)
        xmlFree(lastQuery);
    lastQuery = NULL;
    if (searchRootNode() == NULL) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xmlGenericError(xmlGenericErrorContext,
                        "Seach Empty failed : memory error\n");
#endif
    }

    return (searchRootNode() != NULL);
}


/**
 * searchDoc:
 *
 * Return the document used for seaching ie the search dataBase
 *
 * Returns The document used for searching
 *         Dangerous function to use! Does NOT return a copy of 
 *             search data  so don't free it
 */
xmlDocPtr
searchDoc(void)
{
    return searchDataBase;
}


/**
 * searchRootNode:
 *
 * Get the topmost node in the search dataBase
 *
 * Returns The topmost xml node in search dataBase.
 *         Dangerous function to use! Does NOT return a copy of 
 *             search root node  so don't free it
 */
xmlNodePtr
searchRootNode(void)
{
    return searchDataBaseRoot;
}


/**
 * searchSave:
 * @fileName: A valid file name
 *
 * Save the search dataBase to @fileName
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
searchSave(const xmlChar * fileName)
{
    return xmlSaveFormatFile((char *) fileName, searchDataBase, 1);
}


/**
 * searchAdd:
 * @node: Is valid
 *
 * Add a node to the search dataBase
 *
 * Returns 1 if able to add @node to top node in search dataBase,
 *         0 otherwise
 */
int
searchAdd(xmlNodePtr node)
{
    int result = 0;

    if (node && searchDataBaseRoot) {
        xmlAddChild(searchDataBaseRoot, node);
        result++;
    }
    return result;
}


/**
 * scanForBreakPoint: 
 * @payload: A valid xslBreakPointPtr 
 * @data: The criteria to look for and a valid searchInfoPtr of
 *          type SEARCH_BREAKPOINT 
 * @name: Not used 
 *
 * Test if break point matches criteria given by @data. If so then 
 *      set @data->found to 1 and stores  reference to break point found in 
 *         @data->data->node
 *     otherwise @data is unchanged
*/
void
scanForBreakPoint(void *payload, void *data,
                  xmlChar * name ATTRIBUTE_UNUSED)
{
    xslBreakPointPtr breakPoint = (xslBreakPointPtr) payload;
    searchInfoPtr searchInf = (searchInfoPtr) data;
    breakPointSearchDataPtr searchData = NULL;
    int found = 0;

    if (!payload || !searchInf || !searchInf->data
        || (searchInf->type != SEARCH_BREAKPOINT) || searchInf->found)
        return;

    searchData = (breakPointSearchDataPtr) searchInf->data;

    if (searchData->id && (breakPoint->id == searchData->id))
        found = 1;
    else if (searchData->templateName && breakPoint->templateName &&
             (xmlStrCmp(breakPoint->templateName, searchData->templateName)
              == 0))
        found = 1;

    if (found) {
        searchInf->found = 1;
        searchData->breakPoint = breakPoint;
    }
}



/**
 * scanForNode: 
 * @payload: A valid xmlNodePtr
 * @data: The criteria to look for and a valid searchInfo of
 *          type SEARCH_NODE 
 * @name: Not used

 * Test if node matches criteria given by @data if so then set @data->found 
 *   to 1 and  stores reference to node found in @data->data->node
 *     otherwise @data is unchanged
 */
void
scanForNode(void *payload, void *data, xmlChar * name ATTRIBUTE_UNUSED)
{
    searchInfoPtr searchInf = (searchInfoPtr) data;
    nodeSearchDataPtr searchData = NULL;
    xmlNodePtr node = (xmlNodePtr) payload;
    int match = 1;

    if (!node || !node->doc || !node->doc->URL ||
        !searchInf || (searchInf->type != SEARCH_NODE))
        return;

    searchData = (nodeSearchDataPtr) searchInf->data;

    if (searchData->lineNo >= 0)
        match = searchData->lineNo == xmlGetLineNo(node);

    if (searchData->url)
        match = match
            && (strcmp((char *) searchData->url, (char *) node->doc->URL)
                == 0);

    if (match) {
        searchData->node = node;
        searchInf->found = 1;
    }

}


/**
 * findNodeByLineNoHelper:
 * @payload: A valid xsltStylesheetPtr
 * @data: A valid searchInfoPtr
 * @name: Not used
 *
 * We are walking through stylesheets looking for a match 
 */
void
findNodeByLineNoHelper(void *payload, void *data,
                       xmlChar * name ATTRIBUTE_UNUSED)
{
    xsltStylesheetPtr style = (xsltStylesheetPtr) payload;
    searchInfoPtr searchInf = (searchInfoPtr) data;

    if (!payload || !searchInf || !style->doc)
        return;

    walkChildNodes((xmlHashScanner) scanForNode, searchInf,
                   (xmlNodePtr) style->doc);

    /* try the included stylesheets */
    if (!searchInf->found)
        walkIncludes((xmlHashScanner) scanForNode, searchInf, style);
}


/**
 * findNodeByLineNo:
 * @ctxt: Valid ctxt to look into
 * @url: Non-null, non-empty file name that has been loaded by debugger
 * @lineNumber: @lineNumber >= 0 and is available in @url
 *
 * Finds the closest line number in file specified that can be a point 
 *
 * Returns The node at line number number specified if successfull,
 *          NULL otherwise
 */
xmlNodePtr
findNodeByLineNo(xsltTransformContextPtr ctxt,
                 const xmlChar * url, long lineNumber)
{
    xmlNodePtr result = NULL;
    searchInfoPtr searchInf = searchNewInfo(SEARCH_NODE);
    nodeSearchDataPtr searchData = NULL;

    if (!searchInf) {
        xsltGenericError(xsltGenericErrorContext,
                         "Unable to create searchInfo in findNodeByLineNo\n");
        return result;
    }

    if (!ctxt || !url || (lineNumber == -1)) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Invalid ctxt, url or line number to "
                         "findNodeByLineNo\n");
#endif
        return result;
    }

    searchData = (nodeSearchDataPtr) searchInf->data;
    searchData->url = (xmlChar *) xmlMemStrdup((char *) url);
    searchData->lineNo = lineNumber;
    walkStylesheets((xmlHashScanner) findNodeByLineNoHelper, searchInf,
                    ctxt->style);
    if (!searchInf->found) {
        /* try searching the document set */
        xsltDocumentPtr document = ctxt->document;

        while (document && !searchInf->found) {
            walkChildNodes((xmlHashScanner) scanForNode, searchInf,
                           (xmlNodePtr) document->doc);
            document = document->next;
        }
    }
    result = searchData->node;
    searchFreeInfo(searchInf);

    return result;
}


/**
 * findTemplateNode: 
 * @style: A valid stylesheet collection to look into 
 * @name: A valid template name to look for
 *
 * Find a template node
 *
 * Returns The template node found if successful,
 *         NULL otherwise 
 */
xmlNodePtr
findTemplateNode(xsltStylesheetPtr style, const xmlChar * name)
{
    xmlNodePtr result = NULL;
    xmlChar *templName;
    xsltTemplatePtr templ;

    if (!style || !name) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Invalid stylesheet or template name : "
                         "findTemplateNode\n");
#endif
        return result;
    }

    while (style) {
        templ = style->templates;

        while (templ) {
            if (templ->match)
                templName = (xmlChar *) templ->match;
            else
                templName = (xmlChar *) templ->name;

            if (templName) {
                if (!strcmp((char *) templName, (char *) name)) {
                    return templ->elem;
                }
            }
            templ = templ->next;
        }
        if (style->next)
            style = style->next;
        else
            style = style->imports;
    }

#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
    if (!result)
        xsltGenericError(xsltGenericErrorContext,
                         "Template named '%s' not found :"
                         " findTemplateNode\n", name);
#endif
    return result;
}


/**
 * findBreakPointByName:
 * @templateName: The template name to look for
 *
 * Find the breakpoint at template with "match" or "name" equal 
 *    to templateName
 *
 * Returns The break point that matches @templateName
 *         NULL otherwise
*/
xslBreakPointPtr
findBreakPointByName(const xmlChar * templateName)
{
    xslBreakPointPtr result = NULL;
    searchInfoPtr searchInf = searchNewInfo(SEARCH_BREAKPOINT);
    breakPointSearchDataPtr searchData;

    if (!searchInf || (searchInf->type != SEARCH_BREAKPOINT))
        return result;

    searchData = (breakPointSearchDataPtr) searchInf->data;
    searchData->templateName = (xmlChar *) xmlStrdup(templateName);
    if (templateName) {
        walkBreakPoints((xmlHashScanner) scanForBreakPoint, searchInf);
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        if (!searchInf->found) {
            xsltGenericError(xsltGenericErrorContext,
                             "Break point with template name of \"%s\" "
                             "not found :findBreakPointByName\n",
                             templateName);
#endif
        } else
            result = searchData->breakPoint;
    }

    searchFreeInfo(searchInf);

    return result;
}


/**
 * findBreakPointById:
 * @id: The break point id to look for
 *
 * Find a break point by its id
 *
 * Returns The break point with given the break point id if found,
 *          NULL otherwise 
 */
xslBreakPointPtr
findBreakPointById(int id)
{
    xslBreakPointPtr result = NULL;
    searchInfoPtr searchInf = searchNewInfo(SEARCH_BREAKPOINT);
    breakPointSearchDataPtr searchData;

    if (!searchInf || !searchInf->data)
        return result;

    searchData = (breakPointSearchDataPtr) searchInf->data;
    if (id >= 0) {
        searchData->id = id;
        walkBreakPoints((xmlHashScanner) scanForBreakPoint, searchInf);
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        if (!searchInf->found) {
            xsltGenericError(xsltGenericErrorContext,
                             "Break point id %d not found :findBreakPointById\n",
                             id);
#endif
        } else
            result = searchData->breakPoint;
    }

    searchFreeInfo(searchInf);
    return result;
}


/**
 * findNodesByQuery:
 * @query: The xpath query to run, see dbgsearch.c for more details
 *  
 * Find nodes in search dataBase using an xpath query
 *
 * Returns The nodes that match the given query on success,
 *         NULL otherwise 
 */
xmlXPathObjectPtr
findNodesByQuery(const xmlChar * query ATTRIBUTE_UNUSED)
{
    xmlXPathObjectPtr list = NULL;

    return list;
}


/**
 * searchQuery:
 * @query: The query to run . If NULL then query is "//search/ *"
 * @tempFile: Where do we load the search dataBase from to execute
 *             query. If tempFile is NULL "search.data" is used
 * 
 * Send query as parameter for execution of search.xsl using
 *    data stored in @tempFile 
 *
 * Returns 1 on success,
 *         0 otherwise   
 */
int
searchQuery(const xmlChar * tempFile, const xmlChar * query)
{
    int result = 0;
    xmlChar buffer[DEBUG_BUFFER_SIZE];
    const xmlChar *docDirPath = getStringOption(OPTIONS_DOCS_PATH);
    const xmlChar *searchXSL = NULL;


    if (!docDirPath)
        return result;

    xmlStrCpy(buffer, docDirPath);
#ifdef __riscos
    /* RISC OS paths don't end in directory separators */
    xmlStrCat(buffer, ".search/xsl");
#else
    xmlStrCat(buffer, "search.xsl");
#endif
    searchXSL = buffer;
#ifdef __riscos
    /* We're going to pass a native filename to a command that takes URIs,
     * so we need to convert it */
    searchXSL = (xmlChar *) unixfilename((char *) buffer);
#endif


    if (!tempFile)
        tempFile = (xmlChar *) "search.data";
    if (!query || (xmlStrlen(query) == 0))
        query = (xmlChar *) "--param query //search/*";
    /* see configure.in for the defintion of XSLDBG_BIN, the name of our binary*/
    if (snprintf
        ((char *) buff, DEBUG_BUFFER_SIZE - 1,
         "%s  %s %s %s", XSLDBG_BIN, query, searchXSL, tempFile)) {
        result = !xslDbgShellExecute((xmlChar *) buff, 1);
    }
    return result;
}


/**
 * walkBreakPoints:
 * @walkFunc: The function to callback for each break point found
 * @data: The extra data to pass onto walkFunc
 *
 * Walks through all break points calling walkFunc for each. The payload
 *  sent to walkFunc is of type xslBreakPointPtr 
 */
void
walkBreakPoints(xmlHashScanner walkFunc, void *data)
{
    int lineNo;
    xmlHashTablePtr hashTable;

    if (!walkFunc)
        return;

    for (lineNo = 0; lineNo < breakPointLinesCount(); lineNo++) {
        hashTable = lineNoItemGet(lineNo);
        if (hashTable) {
            xmlHashScan(hashTable, walkFunc, data);
        }
    }
}


/**
 * walkTemplates:
 * @walkFunc: The function to callback for each template found
 * @data: The extra data to pass onto walkFunc
 * @style: The stylesheet to start from
 *
 * Walks through all templates calling walkFunc for each. The payload
 *   of walkFunc is of type xsltTemplatePtr
 */
void
walkTemplates(xmlHashScanner walkFunc, void *data, xsltStylesheetPtr style)
{
    xsltTemplatePtr templ;

    if (!walkFunc || !style)
        return;

    while (style) {
        templ = style->templates;
        while (templ) {
            (*walkFunc) (templ, data, NULL);
            templ = templ->next;
        }
        if (style->next)
            style = style->next;
        else
            style = style->imports;
    }
}


/**
 * walkStylesheets:
 * @walkFunc: The function to callback for each stylesheet found
 * @data: The extra data to pass onto walkFunc
 * @style: The stylesheet to start from
 *
 * Walks through all templates calling walkFunc for each. The payload
 *   sent to walkFuc is of type xsltStylesheetPtr
 */
void
walkStylesheets(xmlHashScanner walkFunc, void *data,
                xsltStylesheetPtr style)
{
    xsltStylesheetPtr next;

    if (!walkFunc || !style)
        return;

    next = style->next;
    while (style) {
        (*walkFunc) (style, data, NULL);
        if (style->imports)
            style = style->imports;
        else
            style = next;
    }
}



xmlHashScanner globalWalkFunc = NULL;

/**
 * globalVarHelper:
 * @payload: valid xsltStylesheetPtr
 * @data: is valid
 * @name: not used
 *
 * Helper to find the global variables. We are given control via
 *   walkStylesheets globalWalkFunc will always be set to the
 *    walkFunc to call
 */
void
globalVarHelper(void **payload, void *data ATTRIBUTE_UNUSED,
                xmlChar * name ATTRIBUTE_UNUSED)
{
    xsltStylesheetPtr style = (xsltStylesheetPtr) payload;
    xsltStackElemPtr global;

    if (style) {
        global = style->variables;

        while (global &&global->comp) {
            (*globalWalkFunc) (global->comp->inst, data, NULL);
            global = global->next;
        }
    }
}


/**
 * walkGlobals:
 * @walkFunc: The function to callback for each gobal variable found
 * @data: The extra data to pass onto walkFunc
 * @style: The stylesheet to start from
 *
 * Call walkFunc for each global variable. The payload
 *   sent to walkFunc is of type  xmlNodePtr
 */
void
walkGlobals(xmlHashScanner walkFunc, void *data ATTRIBUTE_UNUSED,
            xsltStylesheetPtr style)
{
    if (!walkFunc || !style)
        return;

    globalWalkFunc = walkFunc;

    walkStylesheets((xmlHashScanner) globalVarHelper, data, style);
}



xmlHashScanner localWalkFunc = NULL;

/**
 * localVarHelper:
 * @payload: valid xsltTemplatePtr
 * @data: is valid
 * @name: not used
 *
 * Helper to find the local variables. We are given control via walkTemplates
 *    globalWalkFunc will always be set to the walkFunc to call
 *   localWalkFunc will always be set to the walkFunc to call
 */
void
localVarHelper(void **payload, void *data ATTRIBUTE_UNUSED,
               xmlChar * name ATTRIBUTE_UNUSED)
{
    xsltTemplatePtr templ = (xsltTemplatePtr) payload;
    xmlNodePtr node;

    if (templ && templ->elem) {
        node = templ->elem->children;

        while (node) {
            if (IS_XSLT_NAME(node, "param")
                || IS_XSLT_NAME(node, "variable")) {
                (*localWalkFunc) (node, data, NULL);
                node = node->next;
            } else
                break;
        }
    }
}


/**
 * walkLocals:
 * @walkFunc: The function to callback for each local variable found
 * @data: The extra data to pass onto walkFunc
 * @style: The stylesheet to start from
 *
 * Walks through all local variables calling walkFunc for each. The payload
 *   of walkFunc is of type xmlNodePtr
 */
void
walkLocals(xmlHashScanner walkFunc, void *data, xsltStylesheetPtr style)
{
    if (!walkFunc || !style)
        return;

    localWalkFunc = walkFunc;

    walkTemplates((xmlHashScanner) localVarHelper, data, style);

}

/**
 * walkIncludes:
 * @walkFunc: The function to callback for each included stylesheet
 * @data: The extra data to pass onto walkFunc
 * @style: The stylesheet to start from
 *
 * Walks through all included stylesheets calling walkFunc for each. 
 * The payload of walkFunc is of type xmlNodePtr
 */

void
walkIncludes(xmlHashScanner walkFunc, void *data, xsltStylesheetPtr style)
{
    xsltDocumentPtr document;   /* included xslt documents */

    if (!walkFunc || !style)
        return;

    while (style) {
        document = style->docList;
        /* look at included documents */
        while (document) {
            (*walkFunc) ((xmlNodePtr) document->doc, data, NULL);
            document = document->next;
        }
        /* try next stylesheet */
        if (style->next)
            style = style->next;
        else
            style = style->imports;
    }
}


/**
 * walkIncludeInst:
 * @walkFunc: The function to callback for each xsl:include instruction found
 * @data: The extra data to pass onto walkFunc
 * @style: The stylesheet to start from
 *
 * Walks through all xsl:include calling walkFunc for each. The payload
 *   of walkFunc is of type xmlNodePtr
 */
void
walkIncludeInst(xmlHashScanner walkFunc, void *data,
                xsltStylesheetPtr style)
{
    xmlNodePtr node = NULL, styleChild = NULL;

    if (!walkFunc || !style)
        return;

    while (style) {
        /*look for stylesheet node */
        if (style->doc) {
            node = (xmlNodePtr) style->doc->children;
            while (node) {
                /* not need but just in case :) */
                if (IS_XSLT_NAME(node, "stylesheet")
                    || IS_XSLT_NAME(node, "transform")) {
                    styleChild = node->children;        /* get the topmost elements */
                    break;
                } else
                    node = node->next;
            }

            /* look for includes */
            while (styleChild) {
                if (IS_XSLT_NAME(styleChild, "include"))
                    (*walkFunc) (styleChild, data, NULL);
                styleChild = styleChild->next;
            }
        }
        /* try next stylesheet */
        if (style->next)
            style = style->next;
        else
            style = style->imports;
    }
}


/**
 * walkChildNodes:
 * @walkFunc: The function to callback for each child/sibling found
 * @data: The extra data to pass onto walker
 * @node: Is valid
 *
 * Call walkFunc for each child of @node the payload sent to walkFunc is
 *   a xmlNodePtr
 */
void
walkChildNodes(xmlHashScanner walkFunc, void *data, xmlNodePtr node)
{
    xmlNodePtr child = NULL;
    searchInfoPtr searchInf = (searchInfoPtr) data;

    if (!walkFunc || !searchInf || !searchInf->data)
        return;

    while (node && !searchInf->found) {
        (walkFunc) (node, data, NULL);
        child = node->children;
        if  (child && !searchInf->found) {
            walkChildNodes(walkFunc, data, child);
        }
        node = node->next;
    }
}


/**
 * searchBreakPointNode:
 * @breakPoint: Is valid
 *
 * Convert @p breakPoint into search dataBase format
 *
 * Returns @breakPoint as a new xmlNode in search dataBase format 
 *               if successful,
 *         NULL otherwise
 */
xmlNodePtr
searchBreakPointNode(xslBreakPointPtr breakPoint)
{

    xmlNodePtr node = NULL;
    int result = 1;

    if (breakPoint) {
        node = xmlNewNode(NULL, (xmlChar *) "breakpoint");
        if (node) {
            /* if unable to create any property failed then result will be equal to 0 */
            result = result
                && (xmlNewProp(node, (xmlChar *) "url", breakPoint->url) !=
                    NULL);
            sprintf(buff, "%ld", breakPoint->lineNo);
            result = result
                && (xmlNewProp(node, (xmlChar *) "line", (xmlChar *) buff)
                    != NULL);
            if (breakPoint->templateName) {
                result = result
                    &&
                    (xmlNewProp
                     (node, (xmlChar *) "template",
                      breakPoint->templateName) != NULL);
            }
            sprintf(buff, "%d", breakPoint->enabled);
            result = result
                &&
                (xmlNewProp(node, (xmlChar *) "enabled", (xmlChar *) buff)
                 != NULL);
            sprintf(buff, "%d", breakPoint->type);
            result = result
                && (xmlNewProp(node, (xmlChar *) "type", (xmlChar *) buff)
                    != NULL);
            sprintf(buff, "%d", breakPoint->id);
            result = result
                && (xmlNewProp(node, (xmlChar *) "id", (xmlChar *) buff) !=
                    NULL);
        } else
            result = 0;
        if (!result) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
            xsltGenericError(xsltGenericErrorContext,
                             "Error out of Memory for function searchBreakPointNode\n");
#endif
        }
    }
    return node;
}


/**
 * searchTemplateNode:
 * @templNode: Is valid
 * 
 * Convert @templateNode into search dataBase format
 *
 * Returns @templNode as a new xmlNode in search dataBase format 
 *               if successful,
 *         NULL otherwise
 */
xmlNodePtr
searchTemplateNode(xmlNodePtr templNode)
{
    xmlNodePtr node = NULL;
    xmlChar *value;
    int result = 1;

    if (templNode) {
        node = xmlNewNode(NULL, (xmlChar *) "template");
        if (node) {
            /* if unable to create any property failed then result will be equal to 0 */
            value = xmlGetProp(templNode, (xmlChar *) "match");
            if (value) {
                result = result
                    && (xmlNewProp(node, (xmlChar *) "match", value) !=
                        NULL);
                xmlFree(value);
            }
            value = xmlGetProp(templNode, (xmlChar *) "name");
            if (value) {
                result = result
                    && (xmlNewProp(node, (xmlChar *) "name", value) !=
                        NULL);
                xmlFree(value);
            }
            if (templNode->doc) {
                result = result
                    &&
                    (xmlNewProp
                     (node, (xmlChar *) "url",
                      templNode->doc->URL) != NULL);
            }
            sprintf(buff, "%ld", xmlGetLineNo(templNode));
            result = result
                && (xmlNewProp(node, (xmlChar *) "line", (xmlChar *) buff)
                    != NULL);
        } else
            result = 0;
        if (!result) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
            xsltGenericError(xsltGenericErrorContext,
                             "Error out of Memory for function searchTemplateNode\n");
#endif
        }
    }
    return node;
}


/** 
 * searchGlobalNode:
 * @globalVariable: Is valid
 *
  * Convert @globalVariable into search dataBase format
  *
 * Returns @globalVariable as a new xmlNode in search dataBase format 
 *             if successful,
 *         NULL otherwise
 */
xmlNodePtr
searchGlobalNode(xmlNodePtr variable)
{
    xmlNodePtr node = NULL;
    int result = 1;
    xmlChar *value;

    if (variable) {
        node = xmlNewNode(NULL, (xmlChar *) "variable");
        if (node) {
            /* if unable to create any property failed then result will be equal to 0 */
            if (variable->doc) {
                result = result &&
                    (xmlNewProp(node, (xmlChar *) "url",
                                variable->doc->URL) != NULL);
                sprintf(buff, "%ld", xmlGetLineNo(variable));
                result = result
                    && (xmlNewProp(node, (xmlChar *) "line",
                                   (xmlChar *) buff) != NULL);
            }
            value = xmlGetProp(variable, (xmlChar *) "name");
            if (value) {
                result = result
                    && (xmlNewProp(node, (xmlChar *) "name", value) !=
                        NULL);
                xmlFree(value);
            }
            value = xmlGetProp(variable, (xmlChar *) "select");
            if (value) {
                result = result
                    && (xmlNewProp(node, (xmlChar *) "select", value) !=
                        NULL);
                xmlFree(value);
            }
        } else
            result = 0;
    }
    if (!result) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error out of Memory for function searchGlobalNode\n");
#endif
    }
    return node;
}


/** 
 * searchLocalNode:
 * @localvariable: Is valid
 *
 * Convert @localVariable into search dataBase format
 *
 * Returns @localVariable as a new xmlNode in search dataBase format 
 *             if successful,
 *         NULL otherwise
 */
xmlNodePtr
searchLocalNode(xmlNodePtr variable)
{
    xmlNodePtr node = NULL;
    int result = 1;
    xmlChar *value;
    xmlNodePtr parent;

    if (variable) {
        node = searchGlobalNode(variable);
        if (node) {
            /* if unable to create any property failed then result will be equal to 0 */
            parent = variable->parent;
            /* try to find out what template this variable belongs to */
            if (parent && IS_XSLT_NAME(parent, "template")) {
                value = xmlGetProp(parent, (xmlChar *) "name");
                if (value) {
                    result = result
                        &&
                        (xmlNewProp(node, (xmlChar *) "templname", value)
                         != NULL);
                    xmlFree(value);
                }
                value = xmlGetProp(parent, (xmlChar *) "match");
                if (value) {
                    result = result
                        &&
                        (xmlNewProp(node, (xmlChar *) "templmatch", value)
                         != NULL);
                    xmlFree(value);
                }
            }
        } else
            result = 0;
    }
    if (!result) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error out of Memory for function searchLocalNode\n");
#endif
    }
    return node;
}


/**
 * searchSourceNode:
 * @style: Is valid
 * 
  * Convert @style into search dataBase format
  *
 * Returns @style as a new xmlNode in search dataBase format if successful,
 *         NULL otherwise
 */
xmlNodePtr
searchSourceNode(xsltStylesheetPtr style)
{
    xmlNodePtr node = NULL;
    int result = 1;

    if (style) {
        if (style->parent == NULL)
            node = xmlNewNode(NULL, (xmlChar *) "source");
        else
            node = xmlNewNode(NULL, (xmlChar *) "import");
        if (node) {
            /* if unable to create any property failed then result will be equal to 0 */
            if (style->doc) {
                result = result &&
                    (xmlNewProp(node, (xmlChar *) "href", style->doc->URL)
                     != NULL);
                if (style->parent && style->parent->doc) {
                    result = result &&
                        (xmlNewProp(node, (xmlChar *) "parent",
                                    style->parent->doc->URL) != NULL);
                }
            }
        } else
            result = 0;
    }
    if (!result) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error out of Memory for function searchSourceNode\n");
#endif
    }
    return node;
}


/**
 * searchIncludeNode:
 * @include: Is a valid xsl:include instruction
 *   
 * Convert @include into search dataBase format
 *
 * Returns @include as a new xmlNode in search dataBase format if successful,
 *         NULL otherwise
 */
xmlNodePtr
searchIncludeNode(xmlNodePtr include)
{
    xmlNodePtr node = NULL;
    int result = 1;
    xmlChar *value;

    if (include) {
        node = xmlNewNode(NULL, (xmlChar *) "include");
        if (node) {
            /* if unable to create any property failed then result will be equal to 0 */
            if (include->doc) {
                value = xmlGetProp(include, (xmlChar *) "href");
                if (value) {
                    result = result
                        && (xmlNewProp(node, (xmlChar *) "href", value) !=
                            NULL);
                    xmlFree(value);
                }
                if (include->parent && include->parent->doc) {
                    result = result &&
                        (xmlNewProp(node, (xmlChar *) "url",
                                    include->parent->doc->URL) != NULL);
                    sprintf(buff, "%ld", xmlGetLineNo(include));
                    result = result
                        && (xmlNewProp(node, (xmlChar *) "line",
                                       (xmlChar *) buff) != NULL);
                }
            }
        } else
            result = 0;
    }
    if (!result) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error out of Memory for function searchIncludeNode\n");
#endif
    }
    return node;
}


/**
 * searchCallStackNode:
 * @callStackItem: Is valid
 * 
 * Convert @callStackItem into search dataBase format
 *
 * Returns @callStackItem as a new xmlNode in search dataBase format 
 *            if successful,
 *         NULL otherwise
 */
xmlNodePtr
searchCallStackNode(xslCallPointPtr callStackItem)
{
    xmlNodePtr node = NULL;
    int result = 1;

    if (callStackItem) {
        node = xmlNewNode(NULL, (xmlChar *) "callstack");
        if (node) {
            /* if unable to create any property failed then result will be equal to 0 */
            if (callStackItem->info && callStackItem->info->url)
                result = result
                    &&
                    (xmlNewProp
                     (node, (xmlChar *) "url", callStackItem->info->url)
                     != NULL);
            sprintf(buff, "%ld", callStackItem->lineNo);
            result = result
                && (xmlNewProp(node, (xmlChar *) "line", (xmlChar *) buff)
                    != NULL);
            if (callStackItem->info && callStackItem->info->templateName) {
                result = result &&
                    (xmlNewProp
                     (node, (xmlChar *) "template",
                      callStackItem->info->templateName) != NULL);
            }
        } else
            result = 0;
        if (!result) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
            xsltGenericError(xsltGenericErrorContext,
                             "Error out of Memory for function searchBreakPointNode\n");
#endif
        }
    }
    return node;
}
