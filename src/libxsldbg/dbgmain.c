
/***************************************************************************
                          dbgmain.c  -  main function for debugger use
                             -------------------
    begin                : Fri Nov 2 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

#include "xsldbg.h"
#include "xslbreakpoint.h"
#include "xslcallpoint.h"
#include "files.h"

#include <libxslt/xsltutils.h>  /* need for breakpoint callback support */

/* setup debugger callbacks */
struct DebuggerCallbacks {
    xsltHandleDebuggerCallback debuggercallback;
    xsltAddCallCallback addcallback;
    xsltDropCallCallback dropcallback;
} debuggerDriver;

/* -----------------------------------------
   Private function declarations for dbgmain.c
 -------------------------------------------*/

/**
 * handleDebugger:
 * @cur : source node being executed
 * @node : data node being processed
 * @templ : temlate that applies to node
 * @ctxt : the xslt transform context 
 * 
 * If either cur or node are a breakpoint, or xslDebugStatus in state 
 *   where debugging must occcur at this time, then transfer control
 *   to the debugBreak function
 */
void handleDebugger(xmlNodePtr cur, xmlNodePtr node,
                    xsltTemplatePtr templ, xsltTransformContextPtr ctxt);

/* ------------------------------------- 
    End private functions
---------------------------------------*/

/*
-----------------------------------------------------------
       Main debugger functions
-----------------------------------------------------------
*/

extern char *xslShellReadline(char *prompt);


/**
 * debugInit :
 *
 * Initialize debugger
 * Returns 1 on success,
 *         0 otherwise
 */
int
debugInit(void)
{
    int result;

    xslDebugStatus = DEBUG_NONE;
    result = breakPointInit();
    result = result && callStackInit();

    /* setup debugger callbacks */
    debuggerDriver.debuggercallback = handleDebugger;
    debuggerDriver.addcallback = addCall;
    debuggerDriver.dropcallback = dropCall;
    xsltSetDebuggerCallbacks(3, &debuggerDriver);
    return result;
}


/**
 * debugFree :
 *
 * Free up any memory taken by debugging
 */
void
debugFree(void)
{
    breakPointFree();
    callStackFree();
}


/** 
 * debugGotControl:
 * @reached: 1 if debugger has received control, -1 to read its value,
               0 to clear the flag
 *
 * Set flag that debuger has received control to value of @reached
 *
 * Returns 1 if any break point was reached previously,
 *         0 otherwise
 */
int
debugGotControl(int reached)
{
    static int hasReached;
    int result = hasReached;

    if (reached != -1)
        hasReached = reached;
    return result;
}


/**
 * handleDebugger:
 * @cur : source node being executed
 * @node : data node being processed
 * @templ : temlate that applies to node
 * @ctxt : the xslt transform context 
 * 
 * If either cur or node are a breakpoint, or xslDebugStatus in state 
 *   where debugging must occcur at this time then transfer control
 *   to the debugBreak function
 */
void
handleDebugger(xmlNodePtr cur, xmlNodePtr node,
               xsltTemplatePtr templ, xsltTransformContextPtr ctxt)
{
    setActiveBreakPoint(NULL);
    if (!cur && !node) {
        xsltGenericError(xsltGenericErrorContext,
                         "Soure and doc are NULL can't enter debugger\n");
    } else {
        switch (xslDebugStatus) {

                /* A temparary stopping point */
            case DEBUG_WALK:
            case DEBUG_TRACE:
                /* only allow breakpoints at xml elements */
                if (xmlGetLineNo(cur) != -1)
                    debugBreak(cur, node, templ, ctxt);
                break;

            case DEBUG_STOP:
                xslDebugStatus = DEBUG_CONT;
                /* only allow breakpoints at xml elements */
                if (xmlGetLineNo(cur) != -1)
                    debugBreak(cur, node, templ, ctxt);
                break;

            case DEBUG_STEP:
                /* only allow breakpoints at xml elements */
                if (xmlGetLineNo(cur) != -1)
                    debugBreak(cur, node, templ, ctxt);
                break;

            case DEBUG_CONT:
                {
                    xslBreakPointPtr breakPoint = NULL;
                    xmlChar *baseUri = NULL;

                    if (cur) {
                        breakPoint =
                            getBreakPoint(cur->doc->URL,
                                          xmlGetLineNo(cur));

                        if (breakPoint) {
                            if (breakPoint->enabled) {
                                setActiveBreakPoint(breakPoint);
                                debugBreak(cur, node, templ, ctxt);
                                return;
                            }
                        }
                    }
                    if (node) {
                        baseUri = filesGetBaseUri(node);
                        if (baseUri != NULL) {
                            breakPoint =
                                getBreakPoint(baseUri, xmlGetLineNo(node));
                        } else {
                            breakPoint =
                                getBreakPoint(node->doc->URL,
                                              xmlGetLineNo(node));
                        }
                        if (breakPoint) {
                            if (breakPoint->enabled) {
                                setActiveBreakPoint(breakPoint);
                                debugBreak(cur, node, templ, ctxt);
                            }
                        }

                        if (baseUri)
                            xmlFree(baseUri);
                    }
                }
                break;
        }
    }
}
