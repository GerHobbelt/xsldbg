
/***************************************************************************
                          dbgsearch.c  -  description
                             -------------------
    begin                : Fri Nov 2 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/


#include "xsldbg.h"
#include "debugXSL.h"
#include "breakpointInternals.h"
#include "options.h"


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
---------------------------------------*

/**
 * searchNewInfo:
 * @type: what type of search is required
 * 
 * Returns valid search info pointer if succssfull
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

    }
    if (result)
        result->found = 0;
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

            }
            xmlFree(info->data);
        }
        xmlFree(info);
    }
}


/**
 * seachInit:
 *
 * Returns 1 if seach structures have been initialized properly and all
 *               memory required has been obtained,
 *         0 otherwise
*/
int
searchInit(void)
{
    searchDataBase = xmlNewDoc((xmlChar *) "1.0");
    lastQuery = NULL;
    if (searchDataBase == NULL) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xmlGenericError(xmlGenericErrorContext,
                        "Search init failed : memory error\n");
#endif
    } else {
        searchDataBaseRoot = xmlNewNode(NULL, (xmlChar *) "search");
        if (searchRootNode)
            xmlAddChild((xmlNodePtr) searchDataBase, searchDataBaseRoot);
    }
    return (searchDataBase != NULL) && (searchRootNode != NULL);
}


/**
 * searchFree:
 *
 * Free all memory used by searching 
 */
void
searchFree(void)
{
    if (searchDataBase) {
        xmlFreeDoc(searchDataBase);
        searchDataBase = NULL;
    }
}

/**
 * xslSearchEmpty:
 *
 * Empty the seach data base of its contents
 * Returns 1 on success,
 *         0 otherwise
 */
int
xslSearchEmpty(void)
{
    if (searchDataBase) {
        xmlFreeDoc(searchDataBase);
        searchDataBase = xmlNewDoc((xmlChar *) "1.0");
        if (searchDataBase) {
            searchDataBaseRoot = xmlNewNode(NULL, (xmlChar *) "search");
            if (searchRootNode)
                xmlAddChild((xmlNodePtr) searchDataBase,
                            searchDataBaseRoot);
        }
        if (lastQuery)
            xmlFree(lastQuery);
        lastQuery = NULL;
        if ((searchDataBase == NULL) || (searchRootNode == NULL)) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
            xmlGenericError(xmlGenericErrorContext,
                            "Seach Empty failed : memory error\n");
#endif
        }
    }
    return (searchDataBase != NULL) && (searchRootNode != NULL);
}


/**
 * searchDoc:
 *
 * Returns the document used for searching
 *         Dangerous function to use! Does NOT return a copy of 
 *             searchData  so don't free it.
 */
xmlDocPtr
searchDoc(void)
{
    return searchDataBase;
}


/**
 * searchRootNode:
 *
 * Returns the topmost xml node in search dataBase.
 *         Dangerous function to use! Does NOT return a copy of 
 *             searchRootNode  so don't free it
 */
xmlNodePtr
searchRootNode(void)
{
    return searchDataBaseRoot;
}


/**
 * xslSearchSave:
 * @fileName: valid fileName to save search dataBase to 
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
xslSearchSave(const xmlChar * fileName)
{
    return xmlSaveFormatFile((char *) fileName, searchDataBase, 1);
}


/**
 * xslSearchAdd:
 * @node: a valid node to be added to the topmost node in search dataBase
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
xslSearchAdd(xmlNodePtr node)
{
    int result = 0;

    if (node && searchDataBaseRoot) {
        xmlAddChild(searchDataBaseRoot, node);
        result++;
    }
    return result;
}


/**
 *  scanForBreakPoint: 
 * @payload: a valid xslBreakPointPtr 
 * @data: the criteria to look for and a valid searchInfo of
 *          type SEARCH_BREAKPOINT 
 * @name: not used
 *
 * Test if breakpoint matches given criteria
 * sets data->found to 1 if criteria is met, and data->data changes
 *      to values of item found,
 * Otherwise no changes are made
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
 *  scanForNode: 
 * @payload: a valid xmlNodePtr
 * @data: the criteria to look for and a valid searchInfo of
 *          type SEARCH_NODE 
 * @name: not used

 * Test if node matches given criteria if so then set found to 1 stores
 *       reference to node found in @data
 *     otherwise @data is unchanged
 *
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
 * @payload: valid xsltStylesheetPtr
 * @data: valid searchInfoPtr
 * @name: not used
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
 * xslFindBreakPointByLineNo:
 * @ctxt: valid ctxt to look into
 * @url: url non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber: number >= 0 and is available in url specified
 *
 * Find the closest line number in file specified that can be a point 
 * Returns  node at line number number specified if successfull,
 *	    NULL otherwise
*/
xmlNodePtr
xslFindNodeByLineNo(xsltTransformContextPtr ctxt,
                    const xmlChar * url, long lineNumber)
{
    xmlNodePtr result = NULL;
    searchInfoPtr searchInf = searchNewInfo(SEARCH_NODE);
    nodeSearchDataPtr searchData = NULL;

    if (!searchInf) {
        xsltGenericError(xsltGenericErrorContext,
                         "Unable to create searchInfo in xslFindNodeByLineNo\n");
        return result;
    }

    if (!ctxt || !url || (lineNumber == -1)) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Invalid ctxt, url or line number to "
                         "xslFindNodeByLineNo\n");
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
 * xslFindTemplateNode: 
 * @style: valid stylesheet collection context to look into
 * @name: template name to look for
 *
 * Returns template node found if successfull
 *         NULL otherwise 
 */
xmlNodePtr
xslFindTemplateNode(xsltStylesheetPtr style, const xmlChar * name)
{
    xmlNodePtr result = NULL;
    xmlChar *templName;
    xsltTemplatePtr templ;

    if (!style || !name) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Invalid stylesheet or template name : "
                         "xslFindTemplateNode\n");
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
                         " xslFindTemplateNode\n", name);
#endif
    return result;
}

/**
 * xslFindBreakPointByName:
 * @templateName: template name to look for
 *
 * Find the breakpoint at template with "match" or "name" equal 
 *    to templateName
 * Returns the break point that has a template name
 *           matching @templateName is found
 *          NULL otherwise
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
                             "not found :xslFindBreakPointByName\n",
                             templateName);
#endif
        } else
            result = searchData->breakPoint;
    }

    searchFreeInfo(searchInf);

    return result;
}


/**
 * xslFindBreakPointById:
 * @id: The break point id to look for
 *
 * Returns the break point with given the break point id if found,
 *         NULL otherwise 
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
                             "Break point id %d not found :xslFindBreakPointById\n",
                             id);
#endif
        } else
            result = searchData->breakPoint;
    }

    searchFreeInfo(searchInf);
    return result;
}


/**
 * xslFindNodesByQuery:
 * @query: xpath query to run, see dbgsearch.c for more details
 * 
 * Returns the nodes that match the given query on success,
 *         NULL otherwise 
 */
xmlXPathObjectPtr
xslFindNodesByQuery(const xmlChar * query ATTRIBUTE_UNUSED)
{
    xmlXPathObjectPtr list = NULL;

    return list;
}


/**
 * xslSearchQuery:
 * @query: query to run . If NULL then query is "//search/*"
 * @tempFile: where do we load the search dataBase from to execute
 *             query. If tempFile is NULL "search.data" is used
 * 
 * Send query as parameter for execution of search.xsl using
 *    data stored in @tempFile 
 * Returns 1 on success,
 *        0 otherwise   
 */
int
xslSearchQuery(const xmlChar * tempFile, const xmlChar * query)
{
    int result = 0;
    xmlChar buffer[DEBUG_BUFFER_SIZE];
    const xmlChar *docDirPath = getStringOption(OPTIONS_DOCS_PATH);
    const xmlChar *searchXSL = NULL;


    if (!docDirPath)
        return result;

    xmlStrCpy(buffer, docDirPath);
    xmlStrCat(buffer, "search.xsl");
    searchXSL = buffer;


    if (!tempFile)
        tempFile = (xmlChar *) "search.data";
    if (!query || (xmlStrlen(query) == 0))
        query = (xmlChar *) "--param query //search/*";
    if (snprintf
        ((char *) buff, DEBUG_BUFFER_SIZE - 1,
         "xsldbg  %s %s %s", query, searchXSL, tempFile)) {
        result = !xslDbgShellExecute((xmlChar *) buff, 1);
    }
    return result;
}


/**
 * walkBreakPoints:
 * @walkFunc: function to callback for each breakpoint found
 * @data: the extra data to pass onto walker
 *
 * Walks through all breakpoints calling walkFunc for each. The payload
 *  sent to walkFunc is of type xslBreakPointPtr 
 */
void
walkBreakPoints(xmlHashScanner walkFunc, void *data)
{
    int lineNo;
    xmlHashTablePtr hashTable;

    if (!walkFunc)
        return;

    for (lineNo = 0; lineNo < xslBreakPointLinesCount(); lineNo++) {
        hashTable = lineNoItemGet(lineNo);
        if (hashTable) {
            xmlHashScan(hashTable, walkFunc, data);
        }
    }
}


/**
 * walkTemplates:
 * @walkFunc: function to callback for each template found
 * @data: the extra data to pass onto walker
 * @style: the stylesheet to start from
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
 * @walkFunc: function to callback for each stylesheet found
 * @data: the extra data to pass onto walker
 * @style: the stylesheet to start from
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
 * @walkFunc: function to callback for each variable of type @type found
 * @data: the extra data to pass onto walker
 * @style: the stylesheet to start from
 *
 * Call walkFunc for each global variable. The payload
 *   sent to walkFunc is of type xmlNodePtr
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
 * @walkFunc: function to callback for each template found
 * @data: the extra data to pass onto walker
 * @style: the stylesheet to start from
 *
 * Walks through all templates calling walkFunc for each. The payload
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
 * @walkFunc: function to callback for each included stylesheet
 * @data: the extra data to pass onto walker
 * @style: the stylesheet to start from
 *
 * Walks through all included stylesheets calling walkFunc for each. The payload
 *   of walkFunc is of type xmlNodePtr
 */
void
walkIncludes(xmlHashScanner walkFunc, void *data, xsltStylesheetPtr style)
{
    xmlNodePtr node = NULL;
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
 * @walkFunc: function to callback for each xsl:include instruction found
 * @data: the extra data to pass onto walker
 * @style: the stylesheet to start from
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
 * @walkFunc: function to callback for each child/sibling found
 * @data: the searchInfoPtr to pass onto walker
 * @node: valid xmlNodePtr
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
        while (child && !searchInf->found) {
            walkChildNodes(walkFunc, data, child);
            child = child->next;
        }
        node = node->next;
    }
}


/**
 * searchBreakPointNode:
 * @breakPoint: valid breakPoint 
 *
 * Returns breakpoint as a new xmlNode in search dataBase format if successful,
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
 * @templNode: valid template node
 * 
 * Returns @templNode as a new xmlNode in search dataBase format if successful,
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
 * @variable: valid xmlNodePtr node
 * 
 * Returns @style as a new xmlNode in search dataBase format if successful,
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
 * @variable: valid  xmlNodePtr node
 * 
 * Returns @style as a new xmlNode in search dataBase format if successful,
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
 * @style: valid stylesheet item
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
 * @include: valid include element
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
 * @callStackItem: valid callStack item
 * 
 * Returns @callStackItem as a new xmlNode in search dataBase format if successful,
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
