
/***************************************************************************
                          dbgmain.c  -  main function for debugger use
                             -------------------
    begin                : Fri Nov 2 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/


#include "config.h"

#include <breakpoint/breakpointInternals.h>

/*
-----------------------------------------------------------
       Main debugger functions
-----------------------------------------------------------
*/

extern char *xslShellReadline (char *prompt);

/**
 * xslDebugInit :
 *
 * Initialize debugger
 * Returns 1 on success,
 *         0 otherwise
 */
int
xslDebugInit (void)
{

  int result;

  xslDebugStatus = DEBUG_CONT;
  result = breakPointInit ();
  result = result && callStackInit ();

  return result;
}


/**
 * xslDebugFree :
 *
 * Free up any memory taken by debugging
 */
void
xslDebugFree (void)
{
  breakPointFree ();
  callStackFree ();
}


/**
 * xslDebugBreak:
 * @templ : The source node being executed
 * @node : The data node being processed
 * @root : The template being applide to "node"
 * @ctxt : transform context for stylesheet being processed
 *
 * A break point has been found so pass control to user
 */
void
xslDebugBreak (xmlNodePtr templ ATTRIBUTE_UNUSED,
	       xmlNodePtr node ATTRIBUTE_UNUSED,
	       xsltTemplatePtr root ATTRIBUTE_UNUSED,
	       xsltTransformContextPtr ctxt ATTRIBUTE_UNUSED)
{
  xsltGenericError (xsltGenericErrorContext,
		    "xslDebugBreak function not overloaded!\n");
}

/** 
 * xslDebugGotControl :
 * @reached : 1 if debugger has received control, 0 otherwise
 *
 * Set flag that debuger has received control to value of @reached
 * Returns 1 if any breakpoint was reached previously,
 *         0 otherwise
 */
int
xslDebugGotControl (int reached)
{
  static int hasReached;
  int result = hasReached;

  hasReached = reached;
  return result;
}
