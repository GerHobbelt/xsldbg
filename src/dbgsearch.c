
/***************************************************************************
                          dbgsearch.c  -  description
                             -------------------
    begin                : Fri Nov 2 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/


#include "config.h"

#ifdef WITH_XSL_DEBUGGER

/*
-----------------------------------------------------------
       Search/Find debugger functions
-----------------------------------------------------------
*/

#include <libxslt/xsltutils.h>
#include <breakpoint/breakpoint.h>

/**
 * xslFindTemplateNode: 
 * @style : valid stylesheet collection context to look into
 * @name : template name to look for
 *
 * Returns : template node found if successfull
 *           NULL otherwise 
 */
xmlNodePtr
xslFindTemplateNode(xsltStylesheetPtr style, const xmlChar * name)
{
    xmlNodePtr result = NULL;
    char *templName;
    xsltTemplatePtr templ;

    if (!style || !name) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Invalid stylesheet or template name : "
                         "xslFindTemplateNode\n");
#endif
        return result;
    }

    while (style){
      templ = style->templates;

      while (templ) {
        if (templ->match)
	  templName = templ->match;
        else
	  templName = templ->name;
	
        if (templName) {
	  if (!strcmp(templName, name)) {
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
xslFindNodeByLineNo(xsltTransformContextPtr ctxt,
                    const xmlChar * url, long lineNumber)
{
    xmlNodePtr result = NULL;

    if (!ctxt || !url || (lineNumber == -1)) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Invalid ctxt, url or line number to "
                         "xslFindNodeByLineNo\n");
#endif
        return result;
    }

    return result;
}


/**
 * xslFindBreakPointById:
 * @id : The break point id to look for
 *
 * Find the break point number for given break point id
 * Returns break point number can be found for given the break point id
 *          0 otherwise 
 */
int
xslFindBreakPointById(int id)
{
    int result = 0, index;
    xslBreakPointPtr breakpoint;
    for (index = 1; index <= xslBreakPointCount(); index++) { 
        breakpoint = xslGetBreakPoint(index);
	if (!breakpoint)
	  break;
        if (breakpoint->id == id) {
            result = index;
            break;
        }
    }

#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
    if (!result)
        xsltGenericError(xsltGenericErrorContext,
                         "Break point id %d not found :xslFindBreakPointById\n",
                         id);
#endif
    return result;
}


/**
 * xslFindBreakPointByLineNo:
 * @url :  a valid url that has been loaded by debugger
 * @lineNumber : lineNumber >= 0 and is available in url specified
 *
 * Find the break point number for a given url and line number
 * Returns break point number number if successfull,
 *	    0 otherwise
*/
int
xslFindBreakPointByLineNo(const xmlChar * url, long lineNumber)
{
    int result = 0, index;
    xslBreakPointPtr breakpoint;
    if (!url || (lineNumber == -1)) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Invalid url or line number to "
                         "xslFindBreakPointByLineNo\n");
#endif
        return result;
    }

    for (index = 1; index <= xslBreakPointCount(); index++) {
      breakpoint = xslGetBreakPoint(index);
      	if (!breakpoint)
	  break;
        if ((breakpoint->lineNo == lineNumber) &&
            (!strcmp(breakpoint->url, url))) {
            result = index;
            break;
        }
    }

    return result;
}


/**
 * xslFindBreakPointByName:
 * @templateName : template name to look for
 *
 * Find the breakpoint at template with "match" or "name" or equal to 
 *    templateName
 * Returns break point number given the template name is found
 *          0 otherwise
*/
int
xslFindBreakPointByName(const xmlChar * templateName)
{
    int result = 0, index;
    xslBreakPointPtr breakpoint;
    if (!templateName) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Template null :xslFindBreakPointByName\n");
#endif
        return result;
    }

    for (index = 1; index <= xslBreakPointCount(); index++) {
      breakpoint = xslGetBreakPoint(index);
	if (!breakpoint)
	  break;
        if (breakpoint->templateName &&
            !strcmp(breakpoint->templateName, templateName)) {
            result = index + 1;
            break;
        }
    }

    return result;
}


#endif
