
/***************************************************************************
                          dbgsearch.c  -  description
                             -------------------
    begin                : Fri Nov 2 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/


#include "config.h"

#include <breakpoint/breakpointInternals.h>

/* our private function*/
void scanForBreakPoint (void *payload, void *data,
			xmlChar * name ATTRIBUTE_UNUSED);

/* data to pass to walk function when searching 
   it is rarely used */
typedef struct _xslBreakPointSearch xslBreakPointSearch;
typedef xslBreakPointSearch *xslBreakPointSearchPtr;
struct _xslBreakPointSearch
{
  const xmlChar *templateName;
  int id;
  int found;  /* allow the walkFunc to indicate that its finished */
  xslBreakPointPtr breakPoint;
};

/* store all data in this document so we can write it to file*/
xmlDocPtr searchDataBase;

/* the topmost node in document*/
xmlNodePtr searchDataBaseRoot;

/* what was the last query that was run */
xmlChar *lastQuery;

#define BUFFER_SIZE 500
char buff[BUFFER_SIZE];

/**
 * seachInit:
 *
 * Returns 1 if seach structures have been initialized properly and all
 *               memory required has been obtained,
 *         0 otherwise
*/
int
searchInit (void)
{
  searchDataBase = xmlNewDoc ((xmlChar *) "1.0");
  lastQuery = NULL;
  if (searchDataBase == NULL)
    {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
      xmlGenericError (xmlGenericErrorContext,
		       "Search init failed : memory error\n");
#endif
    }
  else
    {
      searchDataBaseRoot = xmlNewNode (NULL, (xmlChar *) "search");
      if (searchRootNode)
	xmlAddChild ((xmlNodePtr) searchDataBase, searchDataBaseRoot);
    }
  return (searchDataBase != NULL) && (searchRootNode != NULL);
}


/**
 * searchFree:
 *
 * Free all memory used by searching 
 */
void
searchFree (void)
{
  if (searchDataBase)
    {
      xmlFreeDoc (searchDataBase);
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
xslSearchEmpty (void)
{
  if (searchDataBase)
    {
      xmlFreeDoc (searchDataBase);
      searchDataBase = xmlNewDoc ((xmlChar *) "1.0");
      if (searchDataBase)
	{
	  searchDataBaseRoot = xmlNewNode (NULL, (xmlChar *) "search");
	  if (searchRootNode)
	    xmlAddChild ((xmlNodePtr) searchDataBase, searchDataBaseRoot);
	}
      if (lastQuery)
	xmlFree (lastQuery);
      lastQuery = NULL;
      if ((searchDataBase == NULL) || (searchRootNode == NULL))
	{
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
	  xmlGenericError (xmlGenericErrorContext,
			   "Seach Empty failed : memory error\n");
#endif
	}
    }
  return (searchDataBase != NULL) && (searchRootNode != NULL);
}

/**
 * searchDoc:
 *
 * Return the document used for searching
 *         Dangerous function to use! Does NOT return a copy of 
 *             searchData  so don't free it.
 */
xmlDocPtr
searchDoc (void)
{
  return searchDataBase;
}


/**
 * searchRootNode:
 *
 * Return the topmost xml node in search dataBase.
 *         Dangerous function to use! Does NOT return a copy of 
 *             searchRootNode  so don't free it
 */
xmlNodePtr
searchRootNode (void)
{
  return searchDataBaseRoot;
}


/**
 * xslSearchSave:
 * @fileName : valid fileName to save search dataBase to 
 *
 * Return 1 on success,
 *        0 otherwise
 */
int
xslSearchSave (const xmlChar * fileName)
{
  return xmlSaveFormatFile ((char *) fileName, searchDataBase, 1);
}

/**
 * xslSearchAdd:
 * @node : a valid node to be added to the topmost node in search dataBase
 *
 * Return 1 on success,
 *        0 otherwise
 */
int
xslSearchAdd (xmlNodePtr node)
{
  int result = 0;

  if (node && searchDataBaseRoot)
    {
      xmlAddChild (searchDataBaseRoot, node);
      result++;
    }
  return result;
}

/**
 *  scanForBreakPoint : 
 * Test if breakpoint matches given criteria
 * @payload : a valid xslBreakPointPtr 
 * @data : the criteria to look for and a valid xslBreakPointSearchPtr 
 * @name
 *
*/
void
scanForBreakPoint (void *payload, void *data, xmlChar * name ATTRIBUTE_UNUSED)
{
  xslBreakPointPtr breakPoint = (xslBreakPointPtr) payload;
  xslBreakPointSearchPtr searchData = (xslBreakPointSearchPtr) data;
  int found = 0;

  if (!payload || !data || searchData->found)
    return;

  if (searchData->id && (breakPoint->id == searchData->id))
    found = 1;
  else if (searchData->templateName && breakPoint->templateName &&
	   (strcmp
	    ((char *) breakPoint->templateName,
	     (char *) searchData->templateName) == 0))
    found = 1;

  if (found)
    {
      searchData->found = 1;
      searchData->breakPoint = breakPoint;
    }
}




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
xmlNodePtr
xslFindNodeByLineNo (xsltTransformContextPtr ctxt,
		     const xmlChar * url, long lineNumber)
{
  xmlNodePtr result = NULL;

  if (!ctxt || !url || (lineNumber == -1))
    {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
      xsltGenericError (xsltGenericErrorContext,
			"Invalid ctxt, url or line number to "
			"xslFindNodeByLineNo\n");
#endif
      return result;
    }

  return result;
}


/**
 * xslFindTemplateNode: 
 * @style : valid stylesheet collection context to look into
 * @name : template name to look for
 *
 * Returns : template node found if successfull
 *           NULL otherwise 
 */
xmlNodePtr
xslFindTemplateNode (xsltStylesheetPtr style, const xmlChar * name)
{
  xmlNodePtr result = NULL;
  xmlChar *templName;
  xsltTemplatePtr templ;

  if (!style || !name)
    {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
      xsltGenericError (xsltGenericErrorContext,
			"Invalid stylesheet or template name : "
			"xslFindTemplateNode\n");
#endif
      return result;
    }

  while (style)
    {
      templ = style->templates;

      while (templ)
	{
	  if (templ->match)
	    templName = (xmlChar *) templ->match;
	  else
	    templName = (xmlChar *) templ->name;

	  if (templName)
	    {
	      if (!strcmp ((char *) templName, (char *) name))
		{
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
    xsltGenericError (xsltGenericErrorContext,
		      "Template named '%s' not found :"
		      " xslFindTemplateNode\n", name);
#endif
  return result;
}

/**
 * xslFindBreakPointByName:
 * @templateName : template name to look for
 *
 * Find the breakpoint at template with "match" or "name" equal 
 *    to templateName
 * Returns the break point number given the template name is found
 *          0 otherwise
*/
xslBreakPointPtr
xslFindBreakPointByName (const xmlChar * templateName)
{
  xslBreakPointSearch searchData;

  searchData.found = 0;
  searchData.id = -1;
  searchData.templateName = templateName;
  searchData.breakPoint = NULL;
  if (templateName)
    {
      xslWalkBreakPoints ((xmlHashScanner) scanForBreakPoint, &searchData);
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
      if (!searchData.found)
	xsltGenericError (xsltGenericErrorContext,
			  "Break point with template name of \"%s\" "
			  "not found :xslFindBreakPointByName\n",
			  templateName);
#endif
    }
  return searchData.breakPoint;
}


/**
 * xslFindBreakPointById:
 * @id : The break point id to look for
 *
 * Find the break point number for given break point id
 * Returns break point number found for given the break point id
 *          0 otherwise 
 */
xslBreakPointPtr
xslFindBreakPointById (int id)
{
  xslBreakPointSearch searchData;

  searchData.found = 0;
  searchData.id = id;
  searchData.templateName = NULL;
  searchData.breakPoint = NULL;
  if (id)
    {
      xslWalkBreakPoints ((xmlHashScanner) scanForBreakPoint, &searchData);
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
      if (!searchData.found)
	xsltGenericError (xsltGenericErrorContext,
			  "Break point id %d not found :xslFindBreakPointById\n",
			  id);
#endif
    }
  return searchData.breakPoint;
}


/**
 * xslFindNodesByQuery:
 * @query: xpath query to run, see dbgsearch.c for more details
 * 
 * Return the nodes that match the given query on success,
 *        NULL otherwise 
 */
xmlXPathObjectPtr
xslFindNodesByQuery (const xmlChar * query ATTRIBUTE_UNUSED)
{
  xmlXPathObjectPtr list = NULL;

  return list;
}

/**
 * xslSearchQuery:
 * @query: query to run . If NULL then use "//search *"
 * @tempFile : where do we load the search dataBase from to execute
 *             query. If tempFile is NULL "search.data" is used
 * 
 * Send query as parameter for execution of search.xsl using
 *    data stored in @tempFile 
 * Return 1 on success,
 *        0 otherwise   
 */
int
xslSearchQuery (const xmlChar * tempFile ATTRIBUTE_UNUSED,
		const xmlChar * query ATTRIBUTE_UNUSED)
{

  /* this must be overloaded in debugger */
  return 0;
}


/**
 * xslWalkBreakPoints:
 * @walkFunc: function to callback for each breakpoint found
 * @data : the extra data to pass onto walker
 *
 * Walks through all breakpoints calling walkFunc for each. The payload
 *  sent to walkFunc is of type xslBreakPointPtr 
 */
void
xslWalkBreakPoints (xmlHashScanner walkFunc, void *data)
{
  int lineNo;
  xmlHashTablePtr hashTable;

  if (!walkFunc)
    return;

  for (lineNo = 0; lineNo < xslBreakPointLinesCount (); lineNo++)
    {
      hashTable = lineNoItemGet (lineNo);
      if (hashTable)
	{
	  xmlHashScan (hashTable, walkFunc, data);
	}
    }
}



/**
 * xslWalkTemplates:
 * @walkFunc: function to callback for each template found
 * @data : the extra data to pass onto walker
 * @style : the stylesheet to start from
 *
 * Walks through all templates calling walkFunc for each. The payload
 *   of walkFunc is of type xsltTemplatePtr
 */
void
xslWalkTemplates (xmlHashScanner walkFunc, void *data,
		  xsltStylesheetPtr style)
{
  xsltTemplatePtr templ;

  if (!walkFunc || !style)
    return;

  while (style)
    {
      templ = style->templates;
      while (templ)
	{
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
 * xslWalkStylesheets:
 * @walkFunc: function to callback for each stylesheet found
 * @data : the extra data to pass onto walker
 * @style : the stylesheet to start from
 *
 * Walks through all templates calling walkFunc for each. The payload
 *   sent to walkFuc is of type xsltStylesheetPtr
 */
void
xslWalkStylesheets (xmlHashScanner walkFunc, void *data,
		    xsltStylesheetPtr style)
{
  xsltStylesheetPtr next;
  if (!walkFunc || !style)
    return;

  next = style->next;
  while (style)
    {
      (*walkFunc) (style, data, NULL);      
      if (style->imports)
	style = style->imports;
      else
	style = next;
    }
}



xmlHashScanner globalWalkFunc = NULL;

/* Our payload is a xsltStylesheetPtr given to us via xslWalkStylesheets. 
   globalWalkFunc will always be set to the walkFunc to call
*/
void 
globalVarHelper(void* *payload, void *data ATTRIBUTE_UNUSED,
		xmlChar * name ATTRIBUTE_UNUSED){
  xsltStylesheetPtr style = (xsltStylesheetPtr)payload;
  xsltStackElemPtr global;

  if (style){
    global = style->variables;
  
    while(global && global->comp ){
      (*globalWalkFunc)(global->comp->inst, data, NULL);
      global = global->next;
    }
  }
}


/**
 * xslWalkGlobals:
 * @walkFunc: function to callback for each variable of type @type found
 * @data : the extra data to pass onto walker
 * @styleCtxt : the stylesheet to start from
 *
 * Call walkFunc for each global variable. The payload
 *   sent to walkFunc is of type xmlNodePtr
 */
void
xslWalkGlobals (xmlHashScanner walkFunc, void *data ATTRIBUTE_UNUSED,
		   xsltStylesheetPtr style)
{
  xsltStackElemPtr global;
  if (!walkFunc || !style)
    return;

  globalWalkFunc = walkFunc;

  xslWalkStylesheets((xmlHashScanner)globalVarHelper, data, style);  
}



xmlHashScanner  localWalkFunc = NULL;

/* Our payload is a xsltTemplatePtr given to us via xslWalkTemplates. 
   localWalkFunc will always be set to the walkFunc to call
*/
void 
localVarHelper(void* *payload, void *data ATTRIBUTE_UNUSED,
		xmlChar * name ATTRIBUTE_UNUSED){
  xsltTemplatePtr templ = (xsltTemplatePtr)payload;
  xmlNodePtr node;
  if (templ && templ->elem){
    node = templ->elem->children;
    
    while(node){
      if (IS_XSLT_NAME(node,"param") || IS_XSLT_NAME(node,"variable")){
	(*localWalkFunc)(node, data, NULL);
	node = node->next;
      }
      else
	break;
    }
  }
}


/**
 * xslWalkLocals:
 * @walkFunc: function to callback for each template found
 * @data : the extra data to pass onto walker
 * @style : the stylesheet to start from
 *
 * Walks through all templates calling walkFunc for each. The payload
 *   of walkFunc is of type xmlNodePtr
 */
void
xslWalkLocals (xmlHashScanner walkFunc, void *data,
		  xsltStylesheetPtr style)
{
  xsltTemplatePtr templ;

  if (!walkFunc || !style)
    return;

  localWalkFunc = walkFunc;

  xslWalkTemplates((xmlHashScanner)localVarHelper, data, style);

}


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
xslWalkIncludes (xmlHashScanner walkFunc, void *data,
		  xsltStylesheetPtr style)
{
  xmlNodePtr node = NULL, styleChild = NULL;

  if (!walkFunc || !style)
    return;

  while (style)
    {
      /*look for stylesheet node */
      if (style->doc){
	node = (xmlNodePtr)style->doc->children;
	while (node){
	  /* not need but just in case :) */
	  if (IS_XSLT_NAME(node, "stylesheet") || IS_XSLT_NAME(node, "transform")){
	    styleChild = node->children; /* get the topmost elements */
	    break;
	  }else
	    node = node->next;
	}

	/* look for includes */
	while(styleChild){
	  if (IS_XSLT_NAME(styleChild, "include"))
	    (*walkFunc)(styleChild, data, NULL);
	styleChild = styleChild ->next;
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
 * xslWalkChildNodes:
 * @walkFunc: function to callback for each child/sibling found
 * @data : the extra data to pass onto walker
 * @node : valid xmlNodePtr
 *
 * Call walkFunc for each child of @node the payload sent to walkFunc is
 *   a xmlNodePtr
 */
void
xslWalkChildNodes (xmlHashScanner walkFunc, void *data, xmlNodePtr node)
{
  xsltGenericError (xsltGenericErrorContext,
		    "xslWalkChildNodes not overloaded\n");

}


/**
 * searchBreakPointNode:
 * @breakPoint : valid breakPoint 
 *
 * Return breakpoint as a new xmlNode in search dataBase format if successful,
 *        NULL otherwise
 */
xmlNodePtr
searchBreakPointNode (xslBreakPointPtr breakPoint)
{

  xmlNodePtr node = NULL;
  int result = 1;

  if (breakPoint)
    {
      node = xmlNewNode (NULL, (xmlChar *) "breakpoint");
      if (node)
	{
	  /* if unable to create any property failed then result will be equal to 0 */
	  result = result
	    && (xmlNewProp (node, (xmlChar *) "url", breakPoint->url) !=
		NULL);
	  sprintf (buff, "%ld", breakPoint->lineNo);
	  result = result
	    && (xmlNewProp (node, (xmlChar *) "line", (xmlChar *) buff) !=
		NULL);
	  if (breakPoint->templateName)
	    {
	      result = result
		&&
		(xmlNewProp
		 (node, (xmlChar *) "template",
		  breakPoint->templateName) != NULL);
	    }
	  sprintf (buff, "%d", breakPoint->enabled);
	  result = result
	    && (xmlNewProp (node, (xmlChar *) "enabled", (xmlChar *) buff) !=
		NULL);
	  sprintf (buff, "%d", breakPoint->type);
	  result = result
	    && (xmlNewProp (node, (xmlChar *) "type", (xmlChar *) buff) !=
		NULL);
	  sprintf (buff, "%d", breakPoint->id);
	  result = result
	    && (xmlNewProp (node, (xmlChar *) "id", (xmlChar *) buff) !=
		NULL);
	}
      else
	result = 0;
      if (!result)
	{
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
	  xsltGenericError (xsltGenericErrorContext,
			    "Error out of Memory for function searchBreakPointNode\n");
#endif
	}
    }
  return node;
}


/**
 * searchTemplateNode:
 * @templNode : valid template node
 * 
 * Returns @templNode as a new xmlNode in search dataBase format if successful,
 *        NULL otherwise
 */
xmlNodePtr
searchTemplateNode (xmlNodePtr templNode)
{
  xmlNodePtr node = NULL;
  xmlChar *value;
  int result = 1;

  if (templNode)
    {
      node = xmlNewNode (NULL, (xmlChar *) "template");
      if (node)
	{
	  /* if unable to create any property failed then result will be equal to 0 */
	  value = xmlGetProp (templNode, (xmlChar *) "match");
	  if (value)
	    {
	      result = result
		&& (xmlNewProp (node, (xmlChar *) "match", value) != NULL);
	      xmlFree (value);
	    }
	  value = xmlGetProp (templNode, (xmlChar *) "name");
	  if (value)
	    {
	      result = result
		&& (xmlNewProp (node, (xmlChar *) "name", value) != NULL);
	      xmlFree (value);
	    }
	  if (templNode->doc){
	    result = result
	      && (xmlNewProp (node, (xmlChar *) "url", templNode->doc->URL) !=
		  NULL);
	  }
	  sprintf (buff, "%ld", xmlGetLineNo (templNode));
	  result = result
	    && (xmlNewProp (node, (xmlChar *) "line", (xmlChar *) buff) !=
		NULL);
	}
      else
	result = 0;
      if (!result)
	{
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
	  xsltGenericError (xsltGenericErrorContext,
			    "Error out of Memory for function searchTemplateNode\n");
#endif
	}
    }
  return node;
}


/** 
 * searchGlobalNode:
 * @variable: valid  xmlNodePtr node
 * 
 * Returns @style as a new xmlNode in search dataBase format if successful,
 *        NULL otherwise
 */
xmlNodePtr searchGlobalNode(xmlNodePtr variable){
    xmlNodePtr node = NULL;
  int result = 1;
  xmlChar *value;
  if (variable){
    node = xmlNewNode (NULL, (xmlChar *)"variable");
    if (node){
      /* if unable to create any property failed then result will be equal to 0 */
      if (variable->doc){
	result = result &&
	  (xmlNewProp (node, (xmlChar *) "url", 
		       variable->doc->URL) !=  NULL);
	sprintf (buff, "%ld", xmlGetLineNo (variable));
	result = result
	  && (xmlNewProp (node, (xmlChar *) "line", 
			  (xmlChar *) buff) != NULL);
      }      
      value = xmlGetProp (variable, (xmlChar *) "name");
      if (value)
	{
	  result = result
	    && (xmlNewProp (node, (xmlChar *) "name", value) != NULL);
	  xmlFree (value);
	}
      value = xmlGetProp (variable, (xmlChar *) "select");
      if (value)
	{
	  result = result
	    && (xmlNewProp (node, (xmlChar *) "select", value) != NULL);
	  xmlFree (value);
	}
    }else
      result = 0;
  }
  if (!result)
    {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
      xsltGenericError (xsltGenericErrorContext,
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
 *        NULL otherwise
 */
xmlNodePtr searchLocalNode(xmlNodePtr variable){
    xmlNodePtr node = NULL;
  int result = 1;
  xmlChar *value;
  xmlNodePtr parent;
  if (variable){
    node = searchGlobalNode(variable);
    if (node){
      /* if unable to create any property failed then result will be equal to 0 */      
      parent = variable->parent;
      /* try to find out what template this variable belongs to */
      if (parent && IS_XSLT_NAME(parent, "template")){
	value = xmlGetProp (parent, (xmlChar *) "name");
	if (value)
	  {
	    result = result
	      && (xmlNewProp (node, (xmlChar *) "templname", value) != NULL);
	  xmlFree (value);
	  }
	value = xmlGetProp (parent, (xmlChar *) "match");
	if (value)
	  {
	    result = result
	      && (xmlNewProp (node, (xmlChar *) "templmatch", value) != NULL);
	  xmlFree (value);
	  }
      }
    }else
      result = 0;
  }
  if (!result)
    {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
      xsltGenericError (xsltGenericErrorContext,
			"Error out of Memory for function searchLocalNode\n");
#endif
    }
  return node;
} 


/**
 * searchSourceNode:
 * @style : valid stylesheet item
 * 
 * Returns @style as a new xmlNode in search dataBase format if successful,
 *        NULL otherwise
 */
xmlNodePtr
searchSourceNode (xsltStylesheetPtr style)
{
  xmlNodePtr node = NULL;
  int result = 1;

  if (style)
    {
      node = xmlNewNode (NULL, (xmlChar *) "source");
      if (node)
	{
	  /* if unable to create any property failed then result will be equal to 0 */
	  if (style->doc)
	    {
	      result = result &&
		(xmlNewProp (node, (xmlChar *) "href", style->doc->URL) !=
		 NULL);
	      if (style->parent && style->parent->doc)
		{
		  result = result &&
		    (xmlNewProp (node, (xmlChar *) "parent",
				 style->parent->doc->URL) != NULL);
		}
	    }
	}
      else
	result = 0;
    }
  if (!result)
    {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
      xsltGenericError (xsltGenericErrorContext,
			"Error out of Memory for function searchSourceNode\n");
#endif
    }
  return node;
}


/**
 * searchIncludeNode:
 * @include : valid include element
 * 
 * Returns @include as a new xmlNode in search dataBase format if successful,
 *        NULL otherwise
 */
xmlNodePtr searchIncludeNode (xmlNodePtr include){
 xmlNodePtr node = NULL;
  int result = 1;
  xmlChar *value;
  if (include)
    {
      node = xmlNewNode (NULL, (xmlChar *) "source");
      if (node)
	{
	  /* if unable to create any property failed then result will be equal to 0 */
	  if (include->doc)
	    {
	      value = xmlGetProp (include, (xmlChar *) "href");
	      if (value)
		{
		  result = result
		    && (xmlNewProp (node, (xmlChar *) "href", value) != NULL);
		  xmlFree (value);
		}
	      if (include->parent && include->parent->doc)
		{
		  result = result &&
		    (xmlNewProp (node, (xmlChar *) "url",
				 include->parent->doc->URL) != NULL);
		  sprintf (buff, "%ld", xmlGetLineNo (include));
		  result = result
		    && (xmlNewProp (node, (xmlChar *) "line", 
			  (xmlChar *) buff) != NULL);
		}
	    }
	}
      else
	result = 0;
    }
  if (!result)
    {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
      xsltGenericError (xsltGenericErrorContext,
			"Error out of Memory for function searchIncludeNode\n");
#endif
    }
  return node;
}
/**
 * searchCallStackNode:
 * @callStackItem : valid callStack item
 * 
 * Returns @callStackItem as a new xmlNode in search dataBase format if successful,
 *        NULL otherwise
 */
xmlNodePtr
searchCallStackNode (xslCallPointPtr callStackItem)
{
  xmlNodePtr node = NULL;
  int result = 1;

  if (callStackItem)
    {
      node = xmlNewNode (NULL, (xmlChar *) "callstack");
      if (node)
	{
	  /* if unable to create any property failed then result will be equal to 0 */
	  if (callStackItem->info && callStackItem->info->url)
	    result = result
	      &&
	      (xmlNewProp (node, (xmlChar *) "url", callStackItem->info->url)
	       != NULL);
	  sprintf (buff, "%ld", callStackItem->lineNo);
	  result = result
	    && (xmlNewProp (node, (xmlChar *) "line", (xmlChar *) buff) !=
		NULL);
	  if (callStackItem->info && callStackItem->info->templateName)
	    {
	      result = result &&
		(xmlNewProp
		 (node, (xmlChar *) "template",
		  callStackItem->info->templateName) != NULL);
	    }
	}
      else
	result = 0;
      if (!result)
	{
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
	  xsltGenericError (xsltGenericErrorContext,
			    "Error out of Memory for function searchBreakPointNode\n");
#endif
	}
    }
  return node;
}


