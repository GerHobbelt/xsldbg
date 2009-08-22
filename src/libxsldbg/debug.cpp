
/**
 *
 *  This file is part of the kdewebdev package
 *  Copyright (c) 2001 Keith Isdale <keith@kdewebdev.org>
 *
 *  This library is free software; you can redistribute it and/or 
 *  modify it under the terms of the GNU General Public License as 
 *  published by the Free Software Foundation; either version 2 of 
 *  the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 **/


#include "debug.h"
#include "xsldbg.h"
#include "debugXSL.h"           /* needed for debugXSLBreak function */
#include "breakpoint.h"
#include "callstack.h"
#include "files.h"
#include "options.h"

#include <libxslt/xsltutils.h>  /* need for breakpoint callback support */

/* setup debugger callbacks */
struct DebuggerCallbacks {
    xsltHandleDebuggerCallback debuggercallback;
    xsltAddCallCallback addcallback;
    xsltDropCallCallback dropcallback;
} debuggerDriver;


/* -----------------------------------------
              Private functions
 -------------------------------------------*/

void debugHandleDebugger(xmlNodePtr cur, xmlNodePtr node,
                         xsltTemplatePtr templ,
                         xsltTransformContextPtr ctxt);

/* ------------------------------------- 
    End private functions
---------------------------------------*/


/*-----------------------------------------------------------
       Main debugger functions
-----------------------------------------------------------*/



int debugInit(void)
{
    int result;

    xslDebugStatus = DEBUG_NONE;
    result = breakPointInit();
    result = result && callStackInit();

    /* setup debugger callbacks */
    debuggerDriver.debuggercallback = debugHandleDebugger;
    debuggerDriver.addcallback = callStackAdd;
    debuggerDriver.dropcallback = callStackDrop;
    xsltSetDebuggerCallbacks(3, &debuggerDriver);
    return result;
}


void debugFree(void)
{
    breakPointFree();
    callStackFree();
}


int debugGotControl(int reached)
{
    static int hasReached;
    int result = hasReached;

    if (reached != -1)
        hasReached = reached;
    return result;
}


void debugHandleDebugger(xmlNodePtr cur, xmlNodePtr node,
                    xsltTemplatePtr templ, xsltTransformContextPtr ctxt)
{

    if (!cur && !node) {
        xsldbgGenericErrorFunc(i18n("Error: XSLT source and XML data are empty. Cannot enter the debugger.\n"));
    } else {
	if (optionsGetIntOption(OPTIONS_GDB)){
	    int doValidation = 0;
	    switch(xsldbgValidateBreakpoints){
		case BREAKPOINTS_ARE_VALID:
		    if (!filesGetStylesheet() || !filesGetMainDoc()) {
			xsldbgValidateBreakpoints = BREAKPOINTS_NEED_VALIDATION;
			doValidation = 1;
		    }
			
		break;
	    
		case BREAKPOINTS_NEED_VALIDATION:
		    if (filesGetStylesheet() && filesGetMainDoc() && templ){ 
			xsldbgValidateBreakpoints = BREAKPOINTS_BEING_VALIDATED;
			doValidation = 1;
		    }
		break;
    
		case BREAKPOINTS_BEING_VALIDATED:
		    /*should never be in the state for any length of time */
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
		     xsltGenericError(xsltGenericErrorContext, 
			"Error: Unexpected breakpoint validation state %d", xsldbgValidateBreakpoints);	    
#endif
		break;
	    }
	    if (doValidation){
		    /* breakpoints will either be marked as orphaned or not as needed */
		    xsldbgValidateBreakpoints = BREAKPOINTS_BEING_VALIDATED;
		    walkBreakPoints((xmlHashScanner)
			    xslDbgShellValidateBreakPoint, ctxt);
		    if (filesGetStylesheet() && filesGetMainDoc() && templ){
			xsldbgValidateBreakpoints = BREAKPOINTS_ARE_VALID;
		    }else{
			xsldbgValidateBreakpoints = BREAKPOINTS_NEED_VALIDATION;
		    }
	    }
	}
        switch (xslDebugStatus) {

                /* A temporary stopping point */
            case DEBUG_WALK:
            case DEBUG_TRACE:
                /* only allow breakpoints at xml elements */
                if (xmlGetLineNo(cur) != -1)
                    debugXSLBreak(cur, node, templ, ctxt);
                break;

            case DEBUG_STOP:
                xslDebugStatus = DEBUG_CONT;
                /* only allow breakpoints at xml elements */
                if (xmlGetLineNo(cur) != -1)
                    debugXSLBreak(cur, node, templ, ctxt);
                break;

            case DEBUG_STEP:
                /* only allow breakpoints at xml elements */
                if (xmlGetLineNo(cur) != -1)
                    debugXSLBreak(cur, node, templ, ctxt);
                break;

            case DEBUG_CONT:
                {
                    breakPointPtr breakPtr = NULL;
                    xmlChar *baseUri = NULL;

                    if (cur) {
                        breakPtr =
                            breakPointGet(cur->doc->URL,
                                          xmlGetLineNo(cur));

                        if (breakPtr && (breakPtr->flags & BREAKPOINT_ENABLED) ){
			    debugXSLBreak(cur, node, templ, ctxt);
			    return;
			}
                    }
		    if (node) {
			baseUri = filesGetBaseUri(node);
                        if (baseUri != NULL) {
                            breakPtr =
                                breakPointGet(baseUri, xmlGetLineNo(node));
                        } else {
                            breakPtr =
                                breakPointGet(node->doc->URL,
                                              xmlGetLineNo(node));
                        }
                        if (breakPtr) {
                            if (breakPtr->flags & BREAKPOINT_ENABLED) {
                                debugXSLBreak(cur, node, templ, ctxt);
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
