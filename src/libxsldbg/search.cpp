
/***************************************************************************
                          search.cpp  - search for variables and xml nodes
                             -------------------
    begin                : Mon May 2 2016
    copyright            : (C) 2016 by Keith Isdale
    email                : keithisdale@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "search.h"
#include "xsldbg.h"
//##TODO
//#include "config-kxsldbg.h"
#include "debugXSL.h"
#include "breakpoint.h"
#include "options.h"
#include "files.h"
#ifdef __riscos

/* Include for filename conversions */
#include "libxml/riscos.h"
#endif

/* our private function*/
void scanForBreakPoint(void *payload, void *data,
                       xmlChar * name);

/* store all data in this document so we can write it to file*/
static xmlDocPtr searchDataBase;

/* the topmost node in document*/
static xmlNodePtr searchDataBaseRoot;

/* what was the last query that was run */
static xmlChar *lastQuery;

#define BUFFER_SIZE 500
static xmlChar searchBuffer[BUFFER_SIZE];

/* -----------------------------------------
   Private function declarations for dbgsearch.c
 -------------------------------------------*/

/**
 * We are walking through stylesheets looking for a match 
 *
 * @param payload: valid xsltStylesheetPtr
 * @param data: valid searchInfoPtr
 * @param name: not used
 *
 */
void findNodeByLineNoHelper(void *payload, void *data,
                         xmlChar * name);

/**
 * Helper to find the global variables. We are given control via
 *   walkStylesheets globalWalkFunc will always be set to the
 *    walkFunc to call
 *
 * @param payload: valid xsltStylesheetPtr
 * @param data: is valid
 * @param name: not used
 *
 */
void globalVarHelper(void **payload, void *data,
                  xmlChar * name);

/**
 * Helper to find the local variables. We are given control via walkTemplates
 *    globalWalkFunc will always be set to the walkFunc to call
 *   localWalkFunc will always be set to the walkFunc to call
 *
 * @param payload: valid xsltTemplatePtr
 * @param data: is valid
 * @parm name: not used
 *
 */
void localVarHelper(void **payload, void *data,
                 xmlChar * name);


/* ------------------------------------- 
    End private functions
    ---------------------------------------*/


int searchInit(void)
{
    searchDataBase = NULL;
    searchDataBaseRoot = NULL;
    lastQuery = NULL;
    if (!searchEmpty()) {
        xsldbgGenericErrorFunc(QObject::tr("Error: Out of memory.\n"));
    }
    return (searchRootNode() != NULL);
}


void searchFree(void)
{
    if (searchDataBase) {
        xmlFreeDoc(searchDataBase);
        searchDataBase = NULL;
        searchDataBaseRoot = NULL;
    }
}


searchInfoPtr searchNewInfo(SearchEnum type)
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
                    searchData->breakPtr = NULL;
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
    if (result) {
        result->found = 0;
        result->error = 0;
    }
    return result;
}


void searchFreeInfo(searchInfoPtr info)
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


int searchEmpty(void)
{
    if (searchDataBase) {
        xmlFreeDoc(searchDataBase);
    }
    searchDataBase = xmlNewDoc((xmlChar *) "1.0");
    searchDataBaseRoot = NULL;
    if (searchDataBase) {
        xmlCreateIntSubset(searchDataBase,
                           (xmlChar *) "search", (xmlChar *)
                           "-//xsldbg//DTD search XML V1.1//EN",
                           (xmlChar *) "search_v1_1.dtd");
        searchDataBaseRoot = xmlNewNode(NULL, (xmlChar *) "search");
        if (searchDataBaseRoot)
            xmlAddChild((xmlNodePtr) searchDataBase, searchDataBaseRoot);
    }
    if (lastQuery)
        xmlFree(lastQuery);
    lastQuery = NULL;
    if (searchRootNode() == NULL) {
#ifdef WITH_XSLDBG_DEBUG_PROCESS
        xsltGenericError(xsltGenericErrorContext,
                        "Error: Unable to clear old search results, memory error?\n");
#endif
    }

    return (searchRootNode() != NULL);
}


xmlDocPtr searchDoc(void)
{
    return searchDataBase;
}


xmlNodePtr searchRootNode(void)
{
    return searchDataBaseRoot;
}


int searchAdd(xmlNodePtr node)
{
    int result = 0;

    if (node && searchDataBaseRoot
        && xmlAddChild(searchDataBaseRoot, node)) {
        result = 1;
    }

    return result;
}


int searchSave(const xmlChar * fileName)
{

    int result = 0;
    QString searchInput;

    if (fileName == NULL)
        searchInput = filesSearchFileName(FILES_SEARCHINPUT);
    else
        searchInput = xsldbgText(fileName);

    if (xmlSaveFormatFile(searchInput.toUtf8().constData(), searchDataBase, 1) != -1){
        result = 1;
    }else{
        xsldbgGenericErrorFunc(QObject::tr("Error: Unable to write search Database to file %1. Try setting the \"searchresultspath\" option to a writable path.\n").arg(searchInput));
    }


    return result;
}


int searchQuery(const xmlChar * tempFile, const xmlChar * outputFile,
            const xmlChar * query)
{
    int result = 0;

    /* The file name of where the input is coming from */
    QString searchInput;

    /* The XSL file name to use during transformation of searchInput */
    QString searchXSL;

    /* Where to store the result of transformation */
    QString searchOutput;


    /* if a tempFile is provided it is up to you to make sure that it is correct !! */
    if (tempFile == NULL)
        searchInput = filesSearchFileName(FILES_SEARCHINPUT);
    else
        searchInput = xsldbgText(tempFile);

    searchXSL = filesSearchFileName(FILES_SEARCHXSL);

    /* if an outputFile is provided it is up to you to make sure that it is correct */
    if (outputFile == NULL)
        searchOutput = filesSearchFileName(FILES_SEARCHRESULT);
    else
        searchOutput = xsldbgText(outputFile);

    if (!query || (xmlStrlen(query) == 0))
        query = (xmlChar *) "--param query //search/*";

    /* see configure.in for the definition of XSLDBG_BIN, the name of our binary */
    if (!searchInput.isEmpty() && !searchXSL.isEmpty() && !searchOutput.isEmpty()) {
        if (optionsGetIntOption(OPTIONS_CATALOGS) == 0)
            snprintf((char *) searchBuffer, sizeof(searchBuffer),
                     "%s -o %s %s %s %s", XSLDBG_BIN,
                     searchOutput.toUtf8().constData(),
                     query,
                     searchXSL.toUtf8().constData(),
                     searchInput.toUtf8().constData());
        else
            /* assume that we are to use catalogs as well in our query */
            snprintf((char *) searchBuffer, sizeof(searchBuffer),
                     "%s --catalogs -o %s %s %s %s", XSLDBG_BIN,
                     searchOutput.toUtf8().constData(),
                     query,
                     searchXSL.toUtf8().constData(),
                     searchInput.toUtf8().constData());
        result = xslDbgShellExecute(searchBuffer, 1);

        if (result && (optionsGetIntOption(OPTIONS_PREFER_HTML) == 0)) {
            /* try printing out the file */
            result = filesMoreFile((const xmlChar*)searchOutput.toUtf8().constData(), NULL);
        }

        xsldbgGenericErrorFunc(QObject::tr("Information: Transformed %1 using %2 and saved to %3.\n").arg(searchInput).arg(searchXSL).arg(searchOutput));
    } else {
        xsldbgGenericErrorFunc(QObject::tr("Error: Invalid arguments to command %1.\n").arg(QString("search")));
    }

    return result;
}


void scanForBreakPoint(void *payload, void *data,
                  xmlChar * name)
{
    Q_UNUSED(name);
    breakPointPtr breakPtr = (breakPointPtr) payload;
    searchInfoPtr searchInf = (searchInfoPtr) data;
    breakPointSearchDataPtr searchData = NULL;
    int found = 0;

    if (!payload || !searchInf || !searchInf->data
        || (searchInf->type != SEARCH_BREAKPOINT) || searchInf->found)
        return;

    searchData = (breakPointSearchDataPtr) searchInf->data;

    if (searchData->id && (breakPtr->id == searchData->id))
        found = 1;
    else if (searchData->templateName && breakPtr->templateName &&
             (xmlStrCmp(breakPtr->templateName, searchData->templateName)
              == 0))
        found = 1;

    if (found) {
        searchInf->found = 1;
        searchData->breakPtr = breakPtr;
    }
}



void scanForNode(void *payload, void *data, xmlChar * name)
{
    Q_UNUSED(name);
    searchInfoPtr searchInf = (searchInfoPtr) data;
    nodeSearchDataPtr searchData = NULL;
    xmlNodePtr node = (xmlNodePtr) payload;
    xmlChar *baseUri = NULL;
    int match = 1;

    if (!node || !node->doc || !node->doc->URL ||
        !searchInf || (searchInf->type != SEARCH_NODE))
        return;

    searchData = (nodeSearchDataPtr) searchInf->data;

    if (searchData->lineNo >= 0)
        match = searchData->lineNo == xmlGetLineNo(node);

    if (searchData->url)
        baseUri = filesGetBaseUri(node);
    if (baseUri) {
        match = match && (xmlStrCmp(searchData->url, baseUri) == 0);
        xmlFree(baseUri);
    } else {
        match = match && (xmlStrcmp(searchData->url, node->doc->URL) == 0);
    }

    if (match) {
        searchData->node = node;
        searchInf->found = 1;
    }

}


void findNodeByLineNoHelper(void *payload, void *data,
                       xmlChar * name)
{
    Q_UNUSED(name);
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


xmlNodePtr findNodeByLineNo(xsltTransformContextPtr ctxt,
                 const xmlChar * url, long lineNumber)
{
    xmlNodePtr result = NULL;
    searchInfoPtr searchInf = searchNewInfo(SEARCH_NODE);
    nodeSearchDataPtr searchData = NULL;

    if (!searchInf) {
#ifdef WITH_XSLDBG_DEBUG_PROCESS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Unable to create searchInfo in findNodeByLineNo\n");
#endif
        return result;
    }

    if (!ctxt || !url || (lineNumber == -1)) {
#ifdef WITH_XSLDBG_DEBUG_PROCESS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Invalid ctxt, url or line number to findNodeByLineNo\n");
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


xmlNodePtr findTemplateNode(xsltStylesheetPtr style, const xmlChar * name)
{
    xmlNodePtr result = NULL;
    xmlChar *templName;
    xsltTemplatePtr templ;

    if (!style || !name) {
#ifdef WITH_XSLDBG_DEBUG_PROCESS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Invalid stylesheet or template name : findTemplateNode\n");
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
                if (!xmlStrCmp((char *) templName, (char *) name)) {
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

    if (!result)
        xsldbgGenericErrorFunc(QObject::tr("Error: XSLT template named \"%1\" was not found.\n").arg(xsldbgText(name)));
    return result;
}


breakPointPtr findBreakPointByName(const xmlChar * templateName)
{
    breakPointPtr result = NULL;
    searchInfoPtr searchInf = searchNewInfo(SEARCH_BREAKPOINT);
    breakPointSearchDataPtr searchData;

    if (!searchInf || (searchInf->type != SEARCH_BREAKPOINT))
        return result;

    searchData = (breakPointSearchDataPtr) searchInf->data;
    searchData->templateName = (xmlChar *) xmlStrdup(templateName);
    if (templateName) {
        walkBreakPoints((xmlHashScanner) scanForBreakPoint, searchInf);
        if (!searchInf->found) {
#ifdef WITH_XSLDBG_DEBUG_PROCESS
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Breakpoint with template name of \"%s\" not found\n",
                             templateName);
#endif
        } else
            result = searchData->breakPtr;
    }

    searchFreeInfo(searchInf);

    return result;
}


breakPointPtr findBreakPointById(int id)
{
    breakPointPtr result = NULL;
    searchInfoPtr searchInf = searchNewInfo(SEARCH_BREAKPOINT);
    breakPointSearchDataPtr searchData;

    if (!searchInf || !searchInf->data)
        return result;

    searchData = (breakPointSearchDataPtr) searchInf->data;
    if (id >= 0) {
        searchData->id = id;
        walkBreakPoints((xmlHashScanner) scanForBreakPoint, searchInf);
        if (!searchInf->found) {
#ifdef WITH_XSLDBG_DEBUG_PROCESS
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Breakpoint id %d not found\n", id);
#endif
        } else
            result = searchData->breakPtr;
    }

    searchFreeInfo(searchInf);
    return result;
}


xmlXPathObjectPtr findNodesByQuery(const xmlChar * query)
{
    Q_UNUSED(query);
    xmlXPathObjectPtr list = NULL;

    return list;
}



void walkBreakPoints(xmlHashScanner walkFunc, void *data)
{
    int lineNo;
    xmlHashTablePtr hashTable;

    if (!walkFunc)
        return;

    for (lineNo = 0; lineNo < breakPointLinesCount(); lineNo++) {
        hashTable = breakPointGetLineNoHash(lineNo);
        if (hashTable) {
            xmlHashScan(hashTable, walkFunc, data);
        }
    }
}


void walkTemplates(xmlHashScanner walkFunc, void *data, xsltStylesheetPtr style)
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


void walkStylesheets(xmlHashScanner walkFunc, void *data,
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

void globalVarHelper(void **payload, void *data,
                xmlChar * name)
{
    Q_UNUSED(data);
    Q_UNUSED(name);
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


void walkGlobals(xmlHashScanner walkFunc, void *data,
            xsltStylesheetPtr style)
{
    Q_UNUSED(data);
    if (!walkFunc || !style)
        return;

    globalWalkFunc = walkFunc;

    walkStylesheets((xmlHashScanner) globalVarHelper, data, style);
}



xmlHashScanner localWalkFunc = NULL;

void localVarHelper(void **payload, void *data,
               xmlChar * name)
{
    Q_UNUSED(data);
    Q_UNUSED(name);
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


void walkLocals(xmlHashScanner walkFunc, void *data, xsltStylesheetPtr style)
{
    if (!walkFunc || !style)
        return;

    localWalkFunc = walkFunc;

    walkTemplates((xmlHashScanner) localVarHelper, data, style);

}



void walkIncludes(xmlHashScanner walkFunc, void *data, xsltStylesheetPtr style)
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


void walkIncludeInst(xmlHashScanner walkFunc, void *data,
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


void walkChildNodes(xmlHashScanner walkFunc, void *data, xmlNodePtr node)
{
    xmlNodePtr child = NULL;
    searchInfoPtr searchInf = (searchInfoPtr) data;

    if (!walkFunc || !searchInf || !searchInf->data)
        return;

    while (node && !searchInf->found) {
        (walkFunc) (node, data, NULL);
        child = node->children;
        if (child && !searchInf->found) {
            walkChildNodes(walkFunc, data, child);
        }
        node = node->next;
    }
}


xmlNodePtr searchBreakPointNode(breakPointPtr breakPtr)
{

    xmlNodePtr node = NULL;
    int result = 1;

    if (breakPtr) {
        node = xmlNewNode(NULL, (xmlChar *) "breakpoint");
        if (node) {
            /* if unable to create any property failed then result will be equal to 0 */
            result = result
                && (xmlNewProp(node, (xmlChar *) "url", breakPtr->url) !=
                    NULL);
            sprintf((char *) searchBuffer, "%ld", breakPtr->lineNo);
            result = result
                &&
                (xmlNewProp(node, (xmlChar *) "line", (xmlChar *) searchBuffer)
                 != NULL);
            if (breakPtr->templateName) {
                result = result
                    &&
                    (xmlNewProp
                     (node, (xmlChar *) "template",
                      breakPtr->templateName) != NULL);
            }
            sprintf((char *) searchBuffer, "%d", breakPtr->flags & BREAKPOINT_ENABLED);
            result = result
                &&
                (xmlNewProp
                 (node, (xmlChar *) "enabled", (xmlChar *) searchBuffer)
                 != NULL);
            sprintf((char *) searchBuffer, "%d", breakPtr->type);
            result = result
                &&
                (xmlNewProp(node, (xmlChar *) "type", (xmlChar *) searchBuffer)
                 != NULL);
            sprintf((char *) searchBuffer, "%d", breakPtr->id);
            result = result
                && (xmlNewProp(node, (xmlChar *) "id", (xmlChar *) searchBuffer)
                    != NULL);
        } else
            result = 0;
        if (!result) {
            xsldbgGenericErrorFunc(QObject::tr("Error: Out of memory.\n"));
        }
    }
    return node;
}


xmlNodePtr searchTemplateNode(xmlNodePtr templNode)
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
            sprintf((char *) searchBuffer, "%ld", xmlGetLineNo(templNode));
            result = result
                &&
                (xmlNewProp(node, (xmlChar *) "line", (xmlChar *) searchBuffer)
                 != NULL);
            if (result) {
                xmlNodePtr textNode = searchCommentNode(templNode);

                if (textNode && !xmlAddChild(node, textNode))
                    result = 0;
            }
        } else
            result = 0;
	if (!result) {
	    xsldbgGenericErrorFunc(QObject::tr("Error: Out of memory.\n"));
        }
    }
    return node;
}


xmlNodePtr searchGlobalNode(xmlNodePtr variable)
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
                sprintf((char *) searchBuffer, "%ld", xmlGetLineNo(variable));
                result = result
                    && (xmlNewProp(node, (xmlChar *) "line",
                                   (xmlChar *) searchBuffer) != NULL);
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
            if (result) {
                xmlNodePtr textNode = searchCommentNode(variable);

                if (textNode && !xmlAddChild(node, textNode))
                    result = 0;
            }
        } else
            result = 0;
    }
    if (!result) {
        xsldbgGenericErrorFunc(QObject::tr("Error: Out of memory.\n"));
    }
    return node;
}


xmlNodePtr searchLocalNode(xmlNodePtr variable)
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
        xsldbgGenericErrorFunc(QObject::tr("Error: Out of memory.\n"));
    }
    return node;
}


xmlNodePtr searchSourceNode(xsltStylesheetPtr style)
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
                if (result) {
                    xmlNodePtr textNode =
                        searchCommentNode((xmlNodePtr) style->doc);
                    if (textNode && !xmlAddChild(node, textNode))
                        result = 0;
                }
            }
        } else
            result = 0;
    }
    if (!result) {
        xsldbgGenericErrorFunc(QObject::tr("Error: Out of memory.\n"));
    }
    return node;
}


xmlNodePtr searchIncludeNode(xmlNodePtr include)
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
                    sprintf((char *) searchBuffer, "%ld", xmlGetLineNo(include));
                    result = result
                        && (xmlNewProp(node, (xmlChar *) "line",
                                       (xmlChar *) searchBuffer) != NULL);
                }
                if (result) {
                    xmlNodePtr textNode = searchCommentNode(include);

                    if (textNode && !xmlAddChild(node, textNode))
                        result = 0;
                }
            }
        } else
            result = 0;
    }
    if (!result) {
        xsldbgGenericErrorFunc(QObject::tr("Error: Out of memory.\n"));
    }
    return node;
}


xmlNodePtr searchCallStackNode(callPointPtr callStackItem)
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
            sprintf((char *) searchBuffer, "%ld", callStackItem->lineNo);
            result = result
                &&
                (xmlNewProp(node, (xmlChar *) "line", (xmlChar *) searchBuffer)
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
            xsldbgGenericErrorFunc(QObject::tr("Error: Out of memory.\n"));
        }
    }
    return node;
}


static xmlChar *commentText(xmlNodePtr node);

/**
 * Returns A copy of comment text that applies to node,
 *         NULL otherwise
 */
xmlChar * commentText(xmlNodePtr node)
{
    xmlChar *result = NULL;

    if (node) {
        if (node->type == XML_COMMENT_NODE)
            result = xmlNodeGetContent(node);
    }

    return result;
}


xmlNodePtr searchCommentNode(xmlNodePtr sourceNode)
{
    xmlNodePtr node = NULL, textChild = NULL;
    xmlChar *text = NULL;
    int result = 0;

    if (sourceNode) {
        text = commentText(sourceNode->prev);
        if (!text) {
            text = commentText(sourceNode->children);
        }

        if (text) {
            node = xmlNewNode(NULL, (xmlChar *) "comment");
            textChild = xmlNewText(text);
            if (node && textChild && xmlAddChild(node, textChild)) {
                result = 1;
            }
            if (!result) {
                if (node) {
                    xmlFreeNode(node);
                    node = NULL;
                }
                if (textChild)
                    xmlFreeNode(textChild);
            }

            xmlFree(text);
        }
    }
    return node;
}
