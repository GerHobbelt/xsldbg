
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

#include <libxslt/xsltInternals.h>
#include <libxml/xpath.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_BUFFER_SIZE 500   /*used by xslDbgShell */




/****************************************************************
 *								*
 *	 The XSL shell related structures and functions		*
 *								*
 ****************************************************************/

/**
 * IS_BLANK:
 * @c:  an UNICODE value (int)
 *
 * Macro to check the following production in the XML spec
 *
 * [3] S ::= (#x20 | #x9 | #xD | #xA)+
 */
#define IS_BLANK(c) (((c) == 0x20) || ((c) == 0x09) || ((c) == 0xA) ||	\
                     ((c) == 0x0D))

/**
 * trimString:
 * @text : valid string with leading or trailing spaces
 *
 * Remove leading and trailing spaces off @text
 *         stores result back into @text
 */
    int trimString(xmlChar * text);

/*
 * splitString:
 * @textIn : the string to split
 * @maxStrings : max number of strings to put into @out
 * @out: 
 * Spit string by white space and put into @out
 * 
 * Return 1 on success,
 *        0 otherwise
 */
    int splitString(xmlChar * textIn, int maxStrings, xmlChar ** out);


/*
 * The Shell interface.
 */
    void xslDbgShell(xmlNodePtr source, xmlNodePtr doc,
                     xmlChar * filename,
                     xmlShellReadlineFunc input,
                     FILE * output, xsltTransformContextPtr styleCtxt);


  /** 
   * xslDbgPrintTemplateNames:
   * @styleCtxt : is valid 
   * @ctxt : not used
   * @arg : not used
   * @verbose : if 1 then print extra messages about templates found,
   *            otherwise print normal messages only 
   * @allFiles : if 1 then look for all templates in stylsheets found in of @styleCtxt
   *             otherwise look in the stylesheet found by xslDebugBreak function
   *
   * print out the list of template names found that match critieria   
   */
    void xslDbgPrintTemplateNames(xsltTransformContextPtr styleCtxt,
                                  xmlShellCtxtPtr ctxt, xmlChar * arg,
                                  int verbose, int allFiles);



/* -----------------------------------------

   BreakPoint related commands

  ------------------------------------------- */

/**
 * xslFrameBreak:
 *@arg : non-null 
 *@stepup : if != 1 then we step up, otherwise step down
 *
 * Set a "frame" breakpoint either up or down from here
 * Return 1 on success,
 *        0 otherwise
 */
int xslDbgShellFrameBreak(xmlChar * arg, int stepup);


/**
 * xslDbgShellBreak:
 * @arg : non-null
 * 
 * Add break point specified by arg
 * Return 1 on success,
 *        0 otherwise
 */
int xslDbgShellBreak(xmlChar * arg, xsltStylesheetPtr style);


/**
 * xslDbgShellDelete:
 * @arg : non-null
 * 
 * Delete break point specified by arg
 * Return 1 on success,
 *        0 otherwise
 */
int xslDbgShellDelete(xmlChar * arg);


/**
 * xslDbgEnableBreakPoint:
 * @payload : valid xslBreakPointPtr
 * @data : enable type, a pointer to an integer 
 *         for a value of 
 *                 1 enable breakpoint
 *                 0 disable breakpoint
 *                 -1 toggle enabling of breakpoint 
 *
 * Enable/disable break points via use of scan of breakPoints
*/
void xslDbgEnableBreakPoint(void *payload, void *data,
                       xmlChar * name ATTRIBUTE_UNUSED);

/**
 * xslDbgPrintBreakPoint:
 * @payload : the breakpoint to print
 * @data : not used
 * @name : not used
 *
 * Print data given by scan of breakPoints 
*/
void xslDbgPrintBreakPoint(void *payload, void *data ATTRIBUTE_UNUSED,
                      xmlChar * name ATTRIBUTE_UNUSED);


/* -----------------------------------------

   Template related commands

  ------------------------------------------- */

/** 
 * xslDbgPrintTemplateNames:
 * @styleCtxt : is valid 
 * @ctxt : not used
 * @arg : not used
 * @verbose : if 1 then print extra messages about templates found,
 *            otherwise print normal messages only 
 * @allFiles : if 1 then look for all templates in stylsheets found in of @styleCtxt
 *             otherwise look in the stylesheet found by xslDebugBreak function
 *
 * print out the list of template names found that match critieria   
 */
void
xslDbgPrintTemplateNames(xsltTransformContextPtr styleCtxt,
                         xmlShellCtxtPtr ctxt ATTRIBUTE_UNUSED,
                         xmlChar * arg ATTRIBUTE_UNUSED, int verbose,
                         int allFiles);

/* -----------------------------------------

   Node viewing related commands

  ------------------------------------------- */

/**
 * xslDbgShellPrintList: 
 * @ctxt : the current shell context
 * @arg : what xpath to display
 * @dir : if 1 print in dir mode?, 
 *        otherwise ls mode
 *
 * Print list of nodes in either ls or dir format
 */
void xslDbgShellPrintList(xmlShellCtxtPtr ctxt, xmlChar * arg, int dir);


/** 
 * xslDbgShellCat :
 * @styleCtxt : current stylesheet context
 * @ctxt : current shell context
 * @arg : xpath to print
 *
 * Print the result of an xpath expression. This can include variables
 *        if styleCtxt is not NULL
 */
void xslDbgShellCat(xsltTransformContextPtr styleCtxt, xmlShellCtxtPtr ctxt,
	       xmlChar * arg);

/**
 * xslDbgShellPrintVariable:
 * @styleCtxt : the current stylesheet context 
 * @arg : the name of variable to look for '$' prefix is optional
 * @type : either DEBUG_PRINT_GLOBAL_VAR, 
 *         or DEBUG_PRINT_LOCAL_VAR
 *
 *  Print the value variable specified by args.
 */
void xslDbgShellPrintVariable(xsltTransformContextPtr styleCtxt, xmlChar * arg,
			 int type);


/* -----------------------------------------

   Operating system related commands

  ------------------------------------------- */

/**
 * xslDbgShellChangeWd
 * @path : path to change to
 *
 * Returns 1 if able to change xsldbg working direcorty to @path
 *         0 otherwise
 */ 
int xslDbgShellChangeWd(const xmlChar * path);


/**
 * xslDbgShellExecute:
 * @name: name of command string to be executed by operating system shell
 * @verbose : if 1 then print extra debugging messages,
 *            normal messages otherwise
 *
 * Return 1 on success,
 *        0 otherwise
 */
int xslDbgShellExecute(xmlChar * name, int verbose);


/* -----------------------------------------

   libxslt parameter related commands

  ------------------------------------------- */

/* 
 * xslDbgShellAddParam:
 * @arg : a string comprised of two words separated by
 *          one or more spaces. 
 *
 * Add a parameter to be sent to libxslt later on
 * Return 1 on success,
 *        0 otherwise
*/
int xslDbgShellAddParam(xmlChar * arg);


/**
 * xslDbgShellDelParam:
 * @arg : single white space trimmed parameter name to look for
 * 
 * Return 1 if able to delete parameter @name,
 *        0 otherwise
 */
int xslDbgShellDelParam(xmlChar * arg);


/**
 * xslDbgShellShowParam:
 * @arg : not used
 *
 * Print list of current paramters
 * Return 1 on success,
 *        0 otherwise
 */
int xslDbgShellShowParam(xmlChar * arg ATTRIBUTE_UNUSED);


/* -----------------------------------------

   Tracing related commands

  ------------------------------------------- */

/**
 * xslDbgShellTrace:
 * @arg : not used
 *
 * Start the tracing of the stylesheet. First need to restart it.
 * Return 1 on success,
 *        0 otherwise
 */
int xslDbgShellTrace(xmlChar * arg ATTRIBUTE_UNUSED);

/**
 * xslDbgShellWalk:
 * @arg : an interger between 0 and 9 indicate the speed of walk
 *
 * Start walking through the stylesheet.
 * Return 1 on success,
 *        0 otherwise
 */
int xslDbgShellWalk(xmlChar * arg);



/* -----------------------------------------
   
   Seach related commands
   
  ------------------------------------------- */

/**
 * xslDbgShellSearch:
 * @styleCtxt :
 * @style :
 * @arg : xpath query to use for searching dataBase
 * 
 * Return 1 if able to run query with @arg, 
 *        0 otherwise
 */
int
xslDbgShellSearch(xsltTransformContextPtr styleCtxt, xsltStylesheetPtr style,
		  xmlChar * arg);


#ifdef __cplusplus
}
#endif
#endif                         
 /* __DEBUG_XSL__ */

