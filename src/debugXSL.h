/***************************************************************************
                          debugXSL.h  -  description
                             -------------------
    begin                : Sun Sep 16 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
 * Orinal file : debugXML.h : This is a set of routines used for debugging the tree
 *              produced by the XML parser.
 * New file : debugXSL.c : Debug support version
 *
 * See Copyright for the status of this software.
 *
 * Daniel Veillard <daniel@veillard.com>
 *
 * Permission abtained to modify the LGPL'd code and extend to include breakpoints, inspections of
 * stylesheet source, xml data, stylesheet variables
 *        Keith Isdale <k_isdale@tpg.com.au>
 */

#ifndef __DEBUG_XSL__
#define __DEBUG_XSL__
#include <stdio.h>
#include <libxml/tree.h>
#include <libxml/debugXML.h>

#ifdef LIBXML_DEBUG_ENABLED
#include <libxslt/xsltInternals.h>
#include <libxml/xpath.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "xsldbg.h"


/*Indicate what type of varibale to print out. 
  Is used by print_variable function */
enum {
  DEBUG_PRINT_GLOBAL_VAR = 1,
  DEBUG_PRINT_LOCAL_VAR
};


/****************************************************************
 *								*
 *	 The XSL shell related structures and functions		*
 *								*
 ****************************************************************/


/*
 * The Shell interface.
 */
void	xslDbgShell	(xmlNodePtr source, xmlNodePtr doc,
			 char *filename,
			 xmlShellReadlineFunc input,
			 FILE *output,
			 xsltTransformContextPtr styleCtxt);


/* print out the list of template names for 
  */
void xslDbgPrintTemplateNames(xsltTransformContextPtr styleCtxt, xmlShellCtxtPtr ctxt, \
			      xmlChar *arg, int verbose, int allFiles);
			 
#ifdef __cplusplus
}
#endif


int
xslDbgShellExecute(xmlChar *name, int verbose);

#endif /* LIBXML_DEBUG_ENABLED */
#endif /* __DEBUG_XSL__ */
