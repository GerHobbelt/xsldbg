
/***************************************************************************
                          dbgcallstack.c  -  description
                             -------------------
    begin                : Fri Nov 2 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

#include "config.h"

#ifdef WITH_DEBUGGER

/*
------------------------------------------------------
                  Xsl call stack related
-----------------------------------------------------
*/

#include <libxslt/xsltutils.h>
#include <breakpoint/breakpoint.h>
#include <string.h>

xslCallPointPtr callStackBot, callStackTop;

xslCallPointInfoPtr callInfo;

/* What frame depth are we to stop at */
int stopDepth = -1;


void callStackInit(){
    callInfo =
        (xslCallPointInfoPtr) xmlMemMalloc(sizeof(xslCallPointInfo));
    callInfo->next = NULL;
    callInfo->templateName = NULL;
    callInfo->url = NULL;
    callStackBot = (xslCallPointPtr) xmlMemMalloc(sizeof(xslCallPoint));
    callStackBot->next = NULL;
    callStackBot->info = NULL;
    callStackBot->lineNo = -1;
    callStackTop = callStackBot;
}

void callStackFree(){

    xslCallPointInfoPtr curInfo = callInfo, nextInfo;
    xslCallPointPtr curCall = callStackBot, nextCall;

    /* remove all call info's */
    while (curInfo) {
        nextInfo = curInfo->next;
        if (curInfo->templateName)
            xmlMemFree(curInfo->templateName);
        if (curInfo->url)
            xmlMemFree(curInfo->url);
        curInfo = nextInfo;
    }

    /* remove all call stack items left. There should be none !! */
    while (curCall) {
        nextCall = curCall->next;
        xmlMemFree(curCall);
        curCall = nextCall;
    }
}


/**
 * xslAddCallInfo:
 * @templateName : template name to add
 * @url : url for templateName
 *
 * Returns a reference to the added info if sucessfull, otherwise NULL  
 */
xslCallPointInfoPtr
xslAddCallInfo(const xmlChar * templateName, const xmlChar * url)
{
    xslCallPointInfoPtr result = NULL, cur = callInfo;

    if (!templateName || !url) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error : Null template name or url:"
                         " xslAddCallInfo\n");
        if (templateName)
            xsltGenericError(xsltGenericErrorContext, "template :'%s'\n",
                             templateName);
        if (url)
            xsltGenericError(xsltGenericErrorContext, "url :'%s'\n", url);

#endif
        return result;
    }

    while (cur->next) {
        if (cur->templateName && !strcmp(cur->templateName, templateName)
            && !strcmp(cur->url, url)) {
            result = cur;
            break;
        }
        cur = cur->next;
    }

    if (!result) {
        result =
            (xslCallPointInfoPtr) xmlMemMalloc(sizeof(xslCallPointInfo));
        if (result) {
            cur->next = result;
            result->templateName =
                (xmlChar *) xmlMemoryStrdup(templateName);
            result->url = (xmlChar *) xmlMemoryStrdup(url);
            result->next = NULL;
        } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
            xsltGenericError(xsltGenericErrorContext,
                             "Error : Unable to create xslCallPointInfo from :"
                             " xslAddCallInfo\n");
#endif
        }
    }
    return result;
}

/**
 * xslAddCall:
 * @templ : current template being applied
 * @source : the source node being processed
 *
 * Add template "call" to call stack
 * Returns : 1 on sucess, 0 otherwise
 */
int
xslAddCall(xsltTemplatePtr templ, xmlNodePtr source)
{
    int result = 0;
    const xmlChar *name = "Default template";
    xslCallPointInfoPtr info;
    if (!templ || !source)
      return result;

    if (!source->doc || !source->doc->URL) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error : Invalid document url in call from : xslAddCall\n");
#endif
        return result;
    }

    /* are at a "frame" break point ie "step down"*/
    if ((xslDebugStatus == DEBUG_STEPDOWN) && (stopDepth == xslCallDepth())) {
        xslDebugStatus = DEBUG_STOP;
        stopDepth = 0;
    }

    /* this need not be an error just we've got a text in source */
    if (xmlGetLineNo(source) == -1) {
        return result;
    }

    if (templ) {
        if (templ->name)
            name = templ->name;
        else {
            if (templ->match)
                name = templ->match;
        }
    }

    if (!name) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error : Invalid template name : xslAddCall\n");
#endif
        return result;
    }

    info = xslAddCallInfo(name, source->doc->URL);

    if (info) {
        xslCallPointPtr cur;

        cur = (xslCallPointPtr) xmlMemMalloc(sizeof(xslCallPoint));
        if (cur) {
            callStackTop->next = cur;
            callStackTop = cur;
            cur->info = info;
            cur->lineNo = xmlGetLineNo(source);
            cur->next = NULL;
            result++;
        } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
            xsltGenericError(xsltGenericErrorContext,
                             "Error : Unable to create call point : xslAddCall\n");
#endif
        }
    } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error : Unable to create call info : xslAddCall\n");
#endif
    }

    return result;
}

/**
 * xslDropCall :
 *
 * Drop the topmost item off the call stack
 */
void
xslDropCall()
{

    if (!callStackBot) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "xslDropCall failed invalid call stack:"
                         " breakpoint.c");
#endif
        return;
    }

    /* are at a "frame" break point ie "step up"*/
    if ((xslDebugStatus == DEBUG_STEPUP) && (stopDepth == -1 * xslCallDepth())) {
        xslDebugStatus = DEBUG_STOP;
        stopDepth = 0;
    }

    if (callStackBot->next) {
        xslCallPointPtr cur = callStackBot;

        while (cur->next && cur->next->next) {
            cur = cur->next;
        }
        if (cur->next)
            xmlMemFree(cur->next);
        cur->next = NULL;
        callStackTop = cur;
    } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "xslDropCall failed "
                         "no items on call stack : breakpoint.c");
#endif
    }
}

/** 
 * xslStepupToDepth :
 * @depth :the frame depth to step up to  
 *
 * Set the frame depth to step up to
 * Returns 1 on sucess , 0 otherwise
 */
int
xslStepupToDepth(int depth)
{
  int result = 0;
  if ((depth > 0) && (depth <= xslCallDepth())){
    stopDepth = -1 * depth;
    xslDebugStatus = DEBUG_STEPUP;
    result++;
  }else{
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "xslStepupToDepth failed invalid depth %d: "
                         " breakpoint.c", depth);
#endif
  }
  return result;
}

/** 
 * xslStepdownToDepth :
 * @depth : the frame depth to step down to 
 *
 * Set the frame depth to step down to
 * Returns 1 on sucess , 0 otherwise
 */
int
xslStepdownToDepth(int depth)
{
  int result = 0;
  if ((depth > 0) && (depth >= xslCallDepth())){ 
    stopDepth = depth;
    xslDebugStatus = DEBUG_STEPDOWN;
    result++;
  }else{
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "xslStepdownToDepth failed invalid depth %d: "
                         " breakpoint.c", depth);
#endif
  }
  return result;
}

/**
 * xslGetCall :
 * @depth : 0 < depth <= xslCallDepth()  
 *
 * Retrieve the call point at specified call depth 

 * Return  non-null a if depth is valid
 *         NULL otherwise 
 */
xslCallPointPtr
xslGetCall(int depth)
{
    xslCallPointPtr result = NULL, cur = callStackBot;

    if (!callStackBot) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "xslGetCall failed invalid call stack:"
                         " breakpoint.c");
#endif
        return result;
    }
    if ((depth < 1) && (depth > xslCallDepth())) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "xslGetCall failed invalid call depth:"
                         " breakpoint.c");
#endif
        return result;
    }

    while (depth > 0 && cur->next) {
        cur = cur->next;
        depth--;
    }

    if (depth == 0)
        result = cur;
    else
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "xslGetCall failed invalid call depth:"
                         " breakpoint.c");
#endif

    return result;
}


/** 
 * xslGetCallStackTop :
 *
 * Returns the top of the call stack
 */
xslCallPointPtr
xslGetCallStackTop()
{
    return callStackTop;
}


/** 
 * xslCallDepth :
 *
 * Return the depth of call stack
 */
int
xslCallDepth()
{
    xslCallPointPtr cur = callStackBot;
    int result = 0;

    while (cur->next) {
        result++;
        cur = cur->next;
    }
    return result;
}

#endif
