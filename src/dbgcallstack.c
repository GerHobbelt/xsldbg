
/***************************************************************************
                          dbgcallstack.c  -  description
                             -------------------
    begin                : Fri Nov 2 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

#include "xsldbg.h"
#include "breakpointInternals.h"


/*
------------------------------------------------------
                  Xsl call stack related 
-----------------------------------------------------
*/

/* keep track of the top and bottom of call stack*/

/* This is the major structure and contains a stack of call points */
xslCallPointPtr callStackBot, callStackTop;

/* save memory by keep only one copy of data used for several 
 items on call stack */
xslCallPointInfoPtr callInfo;

/* What frame depth are we to stop at */
int stopDepth = -1;


/**
 * callStackInit:
 *
 * Returns 1 if callStack has been initialized properly and all
 *               memory required has been obtained,
 *         0 otherwise
*/
int
callStackInit(void)
{

    callInfo = (xslCallPointInfoPtr) xmlMalloc(sizeof(xslCallPointInfo));
    if (callInfo) {
        callInfo->next = NULL;
        callInfo->templateName = NULL;
        callInfo->url = NULL;
    }
    callStackBot = (xslCallPointPtr) xmlMalloc(sizeof(xslCallPoint));
    if (callStackBot) {
        callStackBot->next = NULL;
        callStackBot->info = NULL;
        callStackBot->lineNo = -1;
        callStackTop = callStackBot;
    }
    return (callInfo != NULL) && (callStackBot != NULL);
}



/**
 * callStackFree:
 *
 *
 * Free all memory used by callStack
 */
void
callStackFree(void)
{

    xslCallPointInfoPtr curInfo = callInfo, nextInfo;
    xslCallPointPtr curCall = callStackBot, nextCall;

    /* remove all call info's */
    while (curInfo) {
        nextInfo = curInfo->next;
        if (curInfo->templateName)
            xmlFree(curInfo->templateName);
        if (curInfo->url)
            xmlFree(curInfo->url);
        curInfo = nextInfo;
    }
    curInfo = NULL;

    /* remove all call stack items left. There should be none !! */
    while (curCall) {
        nextCall = curCall->next;
        xmlFree(curCall);
        curCall = nextCall;
    }

    callStackBot = NULL;
    callStackTop = NULL;
    callInfo = NULL;
}


/**
 * addCallInfo:
 * @templatename: Template name to add
 * @url: The url for the template
 *
 * Add template "call" to call stack
 *
 * Returns A reference to the added info if successful, 
 *          NULL otherwise
 */
xslCallPointInfoPtr
addCallInfo(const xmlChar * templateName, const xmlChar * url)
{
    xslCallPointInfoPtr result = NULL, cur = callInfo;

    if (!templateName || !url) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error : Null template name or url:"
                         " addCallInfo\n");
        if (templateName)
            xsltGenericError(xsltGenericErrorContext, "template :'%s'\n",
                             templateName);
        if (url)
            xsltGenericError(xsltGenericErrorContext, "url :'%s'\n", url);

#endif
        return result;
    }

    while (cur->next) {
        if (cur->templateName
            && !xmlStrCmp(cur->templateName, templateName)
            && !xmlStrCmp(cur->url, url)) {
            result = cur;
            break;
        }
        cur = cur->next;
    }

    if (!result) {
        result = (xslCallPointInfoPtr) xmlMalloc(sizeof(xslCallPointInfo));
        if (result) {
            cur->next = result;
            result->templateName =
                (xmlChar *) xmlMemStrdup((char *) templateName);
            result->url = (xmlChar *) xmlMemStrdup((char *) url);
            result->next = NULL;
        } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
            xsltGenericError(xsltGenericErrorContext,
                             "Error : Unable to create xslCallPointInfo from :"
                             " addCallInfo\n");
#endif
        }
    }
    return result;
}


/**
 * addCall:
 * @templ: The current template being applied
 * @source: The source node being processed
 *
 * Add template "call" to call stack
 *
 * Returns 1 on success, 
 *         0 otherwise
 */
int
addCall(xsltTemplatePtr templ, xmlNodePtr source)
{
    int result = 0;
    const char *name = "Default template";
    xslCallPointInfoPtr info;

    if (!templ || !source)
        return result;

    if (!source->doc || !source->doc->URL) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error : Invalid document url in call from : addCall\n");
#endif
        return result;
    }

    /* are at a "frame" break point ie "step down" */
    if ((xslDebugStatus == DEBUG_STEPDOWN)
        && (stopDepth == callDepth())) {
        xslDebugStatus = DEBUG_STOP;
        stopDepth = 0;
    }

    /* this need not be an error just we've got a text in source */
    if (xmlGetLineNo(source) == -1) {
        return result;
    }

    if (templ) {
        if (templ->name)
            name = (char *) templ->name;
        else {
            if (templ->match)
                name = (char *) templ->match;
        }
    }

    if (!name) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error : Invalid template name : addCall\n");
#endif
        return result;
    }

    info = addCallInfo((xmlChar *) name, source->doc->URL);

    if (info) {
        xslCallPointPtr cur;

        cur = (xslCallPointPtr) xmlMalloc(sizeof(xslCallPoint));
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
                             "Error : Unable to create call point : addCall\n");
#endif
        }
    } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error : Unable to create call info : addCall\n");
#endif
    }

    return result;
}


/**
 * dropCall:
 *
 * Drop the topmost item off the call stack
 */
void
dropCall(void)
{

    if (!callStackBot) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "dropCall failed invalid call stack:"
                         " breakpoint.c");
#endif
        return;
    }

    /* are at a "frame" break point ie "step up" */
    if ((xslDebugStatus == DEBUG_STEPUP)
        && (stopDepth == -1 * callDepth())) {
        xslDebugStatus = DEBUG_STOP;
        stopDepth = 0;
    }

    if (callStackBot->next) {
        xslCallPointPtr cur = callStackBot;

        while (cur->next && cur->next->next) {
            cur = cur->next;
        }
        if (cur->next)
            xmlFree(cur->next);
        cur->next = NULL;
        callStackTop = cur;
    } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "dropCall failed "
                         "no items on call stack : breakpoint.c");
#endif
    }
}


/** 
 * stepupToDepth:
 * @depth:The frame depth to step up to  
 *
 * Set the frame depth to step up to
 *
 * Returns 1 on success, 
 *         0 otherwise
 */
int
stepupToDepth(int depth)
{
    int result = 0;

    if ((depth > 0) && (depth <= callDepth())) {
        stopDepth = -1 * depth;
        xslDebugStatus = DEBUG_STEPUP;
        result++;
    } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "stepupToDepth failed invalid depth %d: "
                         " breakpoint.c", depth);
#endif
    }
    return result;
}


/** 
 * stepdownToDepth:
 * @depth: The frame depth to step down to 
 *
 * Set the frame depth to step down to
 *
 * Returns 1 on success, 
 *         0 otherwise
 */
int
stepdownToDepth(int depth)
{
    int result = 0;

    if ((depth > 0) && (depth >= callDepth())) {
        stopDepth = depth;
        xslDebugStatus = DEBUG_STEPDOWN;
        result++;
    } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "stepdownToDepth failed invalid depth %d: "
                         " breakpoint.c", depth);
#endif
    }
    return result;
}


/**
 * getCall:
 * @depth: 0 < @depth <= callDepth()  
 *
 * Retrieve the call point at specified call depth 

 * Returns non-null if depth is valid,
 *         NULL otherwise 
 */
xslCallPointPtr
getCall(int depth)
{
    xslCallPointPtr result = NULL, cur = callStackBot;

    if (!callStackBot) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "getCall failed invalid call stack:"
                         " breakpoint.c");
#endif
        return result;
    }
    if ((depth < 1) && (depth > callDepth())) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "getCall failed invalid call depth:"
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
    else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "getCall failed invalid call depth:"
                         " breakpoint.c");
#endif
    }
    return result;
}


/** 
 * getCallStackTop:
 *
 * Get the top item in the call stack
 *
 * Returns The top of the call stack
 */
xslCallPointPtr
getCallStackTop(void)
{
    return callStackTop;
}


/** 
 * callDepth:
 * 
 * Return the depth of call stack
 *
 * Returns The depth of call stack
 */
int
callDepth(void)
{
    xslCallPointPtr cur = callStackBot;
    int result = 0;

    while (cur->next) {
        result++;
        cur = cur->next;
    }
    return result;
}
