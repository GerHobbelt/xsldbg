
/***************************************************************************
                          dbgmain.c  -  description
                             -------------------
    begin                : Fri Nov 2 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/


#include "config.h"

#ifdef WITH_DEBUGGER

/*
-----------------------------------------------------------
       Main debugger functions
-----------------------------------------------------------
*/

#include <libxslt/xsltutils.h>
#include <breakpoint/breakpoint.h>
#include <string.h>

/* Private functions never to be exported in breakpoint.h */
void breakpointInit();
void breakpointFree();
void callStackInit();
void callStackFree();



/**
 * xslDebugInit :
 *
 * Initialize debugger
 */
void
xslDebugInit()
{
    xslDebugStatus = DEBUG_CONT;

     breakpointInit();
     callStackInit();
}


/**
 * xslDebugFree :
 *
 * Free up any memory taken by debugging
 */
void
xslDebugFree()
{
  breakpointFree();
  callStackFree();
}


extern char *xslShellReadline(char *prompt);


/**
 * @templ : The source node being executed
 * @node : The data node being processed
 * @root : The template being applide to "node"
 * @ctxt : stylesheet being processed
 *
 * A break point has been found so pass control to user
 */
void
xslDebugBreak(xmlNodePtr templ, xmlNodePtr node, xsltTemplatePtr root,
              xsltTransformContextPtr ctxt);
/* This is implemented in debugXSL.c
 */




/** 
 * xslDebugerReached :
 * @reached : true if debugger has received control
 *
 * Set flag that debuger has received control to value of @reached
 * returns true if any breakpoint was reached previously
 */
int xslDebugerReached(int reached){
  static int hasReached;
  int result = hasReached;
  hasReached = reached;
  return result;
}

#endif
