
/***************************************************************************
                          breakpoint.c  -  description
                             -------------------
    begin                : Fri Nov 2 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

#include "config.h"

#ifdef WITH_XSLT_DEBUGGER

/*
-----------------------------------------------------------
       Breakpoint debugger functions
-----------------------------------------------------------
*/

#include <libxslt/xsltutils.h>
#include <breakpoint/breakpoint.h>
#include <string.h>
#define MAX_BREAKPOINTS 500
#define MAX_TEMP_NAME_SIZE 40

xslBreakPoint breakList[MAX_BREAKPOINTS];

/* how many elements are valid*/
int breakIndex = 0;

/* keep track of what break point id we're up to*/
static breakpointCounter = 0;

int activeBreakPoint = 0;


void breakpointInit(){
  int index;
  for (index = 0; index < MAX_BREAKPOINTS; index++) {
    breakList[index].url = NULL;
    breakList[index].lineNo = -1;
    breakList[index].id = -1;
    breakList[index].type = -1;
  }
}


void breakpointFree(){
    /* remove all from file/line break point list */
    while (xslBreakPointCount())
        xslDeleteBreakPoint(1);
}



/**
 * xslActiveBreakPoint();
 * Return the break point number that we stoped at
 */
int
xslActiveBreakPoint()
{
    return activeBreakPoint;
}


/**
 * xslSetActiveBreakPoint:
 * @breakPointNumber : 0 < breakPointNumber <= xslBreakPointCount()
 *
 * Return 1 on success o on failure
 */
int
xslSetActiveBreakPoint(int breakPointNumber)
{
    int result;

    if ((breakPointNumber < 0)
        || (breakPointNumber > xslBreakPointCount()))
        return result;

    activeBreakPoint = breakPointNumber;

    return ++result;
}


/**
 * xslAddBreakPoint:
 * @url : url non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber : number >= 0 and is available in url specified and points to 
 *               an xml element
 * @temlateName : the template name of breakpoint or NULL
 * @type : DEBUG_BREAK_SOURCE if are we stopping at a xsl source line
 *         DEBUG_BREAK_DATA otherwise
 *
 * Add break point at file and line number specifiec
 * Returns  break point number if successfull,
 *	    0 otherwise 
*/
int
xslAddBreakPoint(const xmlChar * url, long lineNumber,
                 const xmlChar * templateName, int type)
{

    int result = 0;
    xslBreakPointPtr breakpoint;

    if (!url || (lineNumber == -1)) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Invalid url or line number to xslAddBreakPoint\n");
#endif
        return result;
    }

    if (xslFindBreakPointByLineNo(url, lineNumber))
        return result;

    if (templateName && xslFindBreakPointByName(templateName))
        return result;


    if (breakIndex < MAX_BREAKPOINTS) {
        if (!xslIsBreakPoint(url, lineNumber)) {
	  breakpoint = &breakList[breakIndex];
            breakpoint->url = strdup(url);
            breakpoint->lineNo = lineNumber;
            if (templateName)
                breakpoint->templateName = strdup(templateName);
            else
                breakpoint->templateName = NULL;
            breakpoint->enabled = 1;
            breakpoint->id = ++breakpointCounter;
            breakpoint->type = type;
            breakIndex++;
            result = breakIndex;
        }
    } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Too many break points already: xslAddBreakPoint\n");
#endif
    }
    return result;
}


/**
 * xslDeleteBreakPoint:
 * @breakPointNumber : 0 < breakPointNumber <= xslBreakPointCount()
 *
 * Delete the break point specified
 * Returns 1 if successfull,
 *	    0 otherwise
*/
int
xslDeleteBreakPoint(int breakPointNumber)
{
    int index, result = 0;
    xslBreakPointPtr cur, next;
    if ((breakPointNumber < 1)
        || (breakPointNumber > xslBreakPointCount())) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Break point number %d not found : xslDeleteBreakPoint\n",
                         breakPointNumber);
#endif
        return 0;
    }
    breakPointNumber--;
    cur = &breakList[breakPointNumber];
    if (cur->url) {
        free(cur->url);
    }
    if (cur->templateName) {
        free(cur->templateName);
    }
    /* we don't need to free cur because we are currently using a 
       static array of breakpoints
    */

    /* move all other elements down */
    for (index = breakPointNumber + 1; index < breakIndex; index++) {
      cur = &breakList[index - 1];
      next = &breakList[index];
        cur->url = next->url;
        cur->lineNo = next->lineNo;
        cur->templateName = next->templateName;
        cur->type = next->type;
        cur->id = next->id;
    }

    /* set the last item in array to a "NULL" entry*/
    cur = &breakList[breakIndex - 1];
    cur->url = NULL;
    cur->templateName = NULL;
    cur->id = -1;
    breakIndex--;
    result = 1;

    return result;
}


/**
 * xslEnableBreakPoint:
 * @breakPointNumber : 0 < breakPointNumber <= xslBreakPointCount()
 * @enable : enable break point if 1, disable if 0, toggle if -1
 *
 * Enable or disable a break point
 * Returns: 1 if successfull,
 *	    0 otherwise
*/
int
xslEnableBreakPoint(int breakPointNumber, int enable)
{
    int result = 0;
    xslBreakPointPtr breakpoint = xslGetBreakPoint(breakPointNumber);
    if (breakpoint){
      if (enable != XSL_TOGGLE_BREAKPOINT)
        breakpoint->enabled = enable;
      else {
        if (breakpoint->enabled)
	  breakpoint->enabled = 0;
        else
	  breakpoint->enabled = 1;
      }
    result++;
    }
    return result;
}


/**
 * xslIsBreakPointEnabled:
 * @breakPointNumber : 0 < breakPointNumber <= xslBreakPointCount()
 *
 * Is the breakpoint at breakPointNumber specified enabled
 * Returns:  -1 if breakPointNumber is invalid
 *           0 if break point is disabled 
 *           1 if break point is enabled      
*/
int
xslIsBreakPointEnabled(int breakPointNumber)
{
    int result = -1;
    xslBreakPointPtr breakpoint = xslGetBreakPoint(breakPointNumber);
    if (breakpoint){
      result = breakpoint->enabled;
    }
    return result;
}


/**
 * xslBreakPointCount:
 *
 * Return the number of breakpoints present
 */
int
xslBreakPointCount()
{
    return breakIndex;
}


/**
 * xslGetBreakPoint:
 * @breakPointNumber : 0 < breakPointNumber <= xslBreakPointCount()
 *
 * Lookup the value of break point at breakPointNumber specified
 * Returns break point if breakPointNumber is valid,  
 *          NULL otherwise
*/
xslBreakPointPtr
xslGetBreakPoint(int breakPointNumber)
{
    xslBreakPointPtr breakPoint = NULL;

    if ((breakPointNumber < 1)
        || (breakPointNumber > xslBreakPointCount())) {
      /*
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Break point number %d not found : xslGetBreakPoint\n",
                         breakPointNumber);
#endif
      */
        return breakPoint;
    }
    breakPointNumber--;
    return &breakList[breakPointNumber];
}


/**
 * xslPrintBreakPoint:
 * @file : file != NULL
 * @breakPointNumber : 0 < breakPointNumber <= xslBreakPointCount()
 *
 * Print the details of break point to file specified
 *
 * Returns: 1 if successfull,
 *	    0 otherwise
 */
int
xslPrintBreakPoint(FILE * file, int breakPointNumber)
{
    int result = 0;

    if (!file || (breakPointNumber < 1)
        || (breakPointNumber > xslBreakPointCount())) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Break point number %d not found : xslPrintBreakPoint\n",
                         breakPointNumber);
#endif
        return result;
    }

    breakPointNumber--;
    fprintf(file, "Breakpoint %d ", breakList[breakPointNumber].id);
    if (breakList[breakPointNumber].enabled)
        fprintf(file, "enabled ");
    else
        fprintf(file, "disabled ");

    if (breakList[breakPointNumber].templateName) {
        fprintf(file, "for template :\"%s\" ",
                breakList[breakPointNumber].templateName);
    }

    if (breakList[breakPointNumber].url) {
        fprintf(file, "in file %s : line %ld",
                breakList[breakPointNumber].url,
                breakList[breakPointNumber].lineNo);
    } else {
        fprintf(file, "in file <n/a>, line %ld",
                breakList[breakPointNumber].lineNo);
    }
    return ++result;
}

/**
 * xslIsBreakPoint:
 * @url : url non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber : number >= 0 and is available in url specified
 *
 * Determine if there is a break point at file and line number specified
 * Returns 1  if successfull,
 *	0 otherwise
*/
int
xslIsBreakPoint(const xmlChar * url, long lineNumber)
{
    int index, result = 0;

    if (!url || (lineNumber == -1))
        return result;

    result = xslFindBreakPointByLineNo(url, lineNumber);

    return result;
}


/**
 * xslIsBreakPointNode:
 * @node : node != NULL
 *
 * Determine if a node is a break point
 * Returns : 1 on sucess, 0 otherwise
 */
int
xslIsBreakPointNode(xmlNodePtr node)
{
    int index, result = 0;

    if (!node || !node->doc)
        return result;

    if (xmlGetLineNo(node) == -1)
        return result;

    if (node->doc->URL) {
        result = xslIsBreakPoint(node->doc->URL, xmlGetLineNo(node));
    }

    return result;
}

#endif
