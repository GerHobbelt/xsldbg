
/**************************************************************************
                          debugXSL.h  -  describe the core function of xsldbg
                             -------------------
    begin                : Sun Sep 16 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 **************************************************************************/

/**************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 **************************************************************************/

/*
 * Orinal file : debugXML.h : This is a set of routines used for
 *    debugging the tree   produced by the XML parser.
 *
 * New file : debugXSL.h : Debug support version
 *
 * See Copyright for the status of this software.
 *
 * Daniel Veillard <daniel@veillard.com>
 *
 * Permission abtained to modify the LGPL'd code and extend to include 
 *   break points, inspections of stylesheet source, xml data, stylesheet
 *    variables   Keith Isdale <k_isdale@tpg.com.au>
 */

#ifndef __DEBUG_XSL__
#define __DEBUG_XSL__


/* We want skip most of these includes when building documentation*/
#ifndef BUILD_DOCS
#include <stdio.h>
#include <libxml/tree.h>
#include <libxml/debugXML.h>
#include <libxslt/xsltInternals.h>
#include <libxml/xpath.h>
#include "xslbreakpoint.h"
#endif

#include "xsldbgio.h" /* define readline function */

#ifdef __cplusplus
extern "C" {
#endif


/* how may items have been printed */
extern int printCount;

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


#ifdef USE_GNOME_DOCS
/**
 * trimString:
 * @text : A valid string with leading or trailing spaces
 *
 * Remove leading and trailing spaces off @text
 *         stores result back into @text
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Remove leading and trailing spaces off @p text
 *         stores result back into @p text
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param text A valid string with leading or trailing spaces
 */
#endif
#endif
    int trimString(xmlChar * text);



#ifdef USE_GNOME_DOCS
/**
 * splitString:
 * @textIn: The string to split
 * @maxStrings: The max number of strings to put into @out
 * @out: Is valid and at least the size of @maxStrings
 *
 * Split string by white space and put into @out
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Spit string by white space and put into @p out
 * 
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param textIn The string to split
 * @param maxStrings The max number of strings to put into @p out
 * @param out Is valid and at least the size of @p maxStrings
 */
#endif
#endif
    int splitString(xmlChar * textIn, int maxStrings, xmlChar ** out);



#ifdef USE_GNOME_DOCS
/**
 *  xslDbgShell:
 * @source: The current stylesheet instruction being executed
 * @doc: The current document node being processed
 * @filename: Not used
 * @input: The function to call to when reading commands from stdio
 * @output: Where to put the results
 * @styleCtxt: Is valid 
 *
 * Present to the user the xsldbg shell
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Present the xsldbg shell to user and process entered commands
 *
 * @param source Current stylesheet instruction being executed
 * @param doc Current document node being processed
 * @param filename Not used
 * @param input The function to call to when reading commands from stdio
 * @param output Where to put the results
 * @param styleCtxt Is valid 
 */
#endif
#endif
    void xslDbgShell(xmlNodePtr source, xmlNodePtr doc,
                     xmlChar * filename,
                     xmlShellReadlineFunc input,
                     FILE * output, xsltTransformContextPtr styleCtxt);



#ifdef USE_GNOME_DOCS
/**
 * xslDbgPrintStyleSheets:
 * @arg: The stylesheets of interests, is NULL for all stylsheets
 *
 * Print stylesheets that can be found in loaded stylsheet
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Print stylesheets that can be found in loaded stylsheet
 *
 * @param arg The stylesheets of interests, is NULL for all stylsheets
 *
 */
#endif
#endif
    void xslDbgPrintStyleSheets(xmlChar * arg);



/* -----------------------------------------

   Break Point related commands

  ------------------------------------------- */


#ifdef USE_GNOME_DOCS
/**
 * xslDbgShellFrameBreak:
 * @arg: Is valid 
 * @stepup: If != 1 then we step up, otherwise step down
 *
 * Set a "frame" break point either up or down from here
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Set a "frame" break point either up or down from here
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param arg Is valid 
 * @param stepup If != 1 then we step up, otherwise step down
 */
#endif
#endif
    int xslDbgShellFrameBreak(xmlChar * arg, int stepup);



#ifdef USE_GNOME_DOCS
/**
 * xslDbgShellBreak:
 * @arg: Is valid
 * @style: Is valid
 * @ctxt: Is valid
 * 
 * Add break point specified by arg
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Add break point specified by arg
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param arg Is valid
 * @param style Is valid
 * @param ctxt Is valid
 */
#endif
#endif
    int xslDbgShellBreak(xmlChar * arg, xsltStylesheetPtr style,
                         xsltTransformContextPtr ctxt);



#ifdef USE_GNOME_DOCS
/**
 * xslDbgShellDelete:
 * @arg: Is valid
 * 
 * Delete break point specified by arg
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Delete break point specified by arg
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param arg Is valid
 */
#endif
#endif
    int xslDbgShellDelete(xmlChar * arg);


#ifdef USE_GNOME_DOCS
/**
 * xslDbgEnableBreakPoint:
 * @payload: A valid xslBreakPointPtr
 * @data: Enable type, a pointer to an integer 
 *         for a value of 
 *                 1 enable break point
 *                 0 disable break point
 *                 -1 toggle enabling of break point 
 * @name: Not used
 *
 * Enable/disable break points via use of scan of break points
*/
#else
#ifdef USE_KDE_DOCS
/**
 * Enable/disable break points via use of scan of break points
 *
 * @param payload Is valid xslBreakPointPtr
 * @param data Enable type, a pointer to an integer 
 *         for a value of 
 *              @li   1 enable break point
 *              @li   0 disable break point
 *              @li   -1 toggle enabling of break point 
 * @param name Not used
*/
#endif
#endif
    void xslDbgEnableBreakPoint(void *payload, void *data, xmlChar * name);

#ifdef USE_GNOME_DOCS
/**
 * xslDbgShellEnable:
 * @arg : is valid
 * @enableType : enable break point if 1, disable if 0, toggle if -1
 *
 * Enable/disable break point specified by arg using enable 
 *      type of @enableType
 * Returns 1 if successful,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
  /**
   * Enable/disable break point specified by arg using enable
   *
   * @param arg: is valid enable "commmand text"
   * @param enableType : enable break point if 1, disable if 0, toggle if -1
   *
   * @returns 1 if successful,
   *          0 otherwise
   */
#endif 
#endif
  int xslDbgShellEnable(xmlChar * arg, int enableType);


#ifdef USE_GNOME_DOCS
/**
 * xslDbgPrintBreakPoint:
 * @payload: A valid xslBreakPointPtr
 * @data: Not used
 * @name: Not used
 *
 * Print data given by scan of break points 
*/
#else
#ifdef USE_KDE_DOCS
/**
 * Print data given by scan of break points 
 *
 * @param payload Is valid xslBreakPointPtr
 * @param data Not used
 * @param name Not used
*/
#endif
#endif
    void xslDbgPrintBreakPoint(void *payload, void *data, xmlChar * name);




/* -----------------------------------------

   Template related commands

  ------------------------------------------- */


#ifdef USE_GNOME_DOCS
/** 
 * xslDbgPrintTemplateNames:
 * @styleCtxt: Is valid 
 * @ctxt: Not used
 * @arg: Not used
 * @verbose: If 1 then print extra messages about templates found,
 *            otherwise print normal messages only 
 * @allFiles: If 1 then look for all templates in stylsheets found in 
 *                 @styleCtxt
 *             otherwise look in the stylesheet found by 
 *                 debugBreak function
 *
 * Print out the list of template names found that match critieria   
 */
#else
#ifdef USE_KDE_DOCS
/** 
 * Print out the list of template names found that match critieria   
 *
 * @param styleCtxt Is valid 
 * @param ctxt Not used
 * @param arg Not used
 * @param verbose If 1 then print extra messages about templates found,
 *            otherwise print normal messages only 
 * @param allFiles If 1 then look for all templates in stylsheets found in 
 *                 @p styleCtxt
 *             otherwise look in the stylesheet found by 
 *                 debugBreak function
 */
#endif
#endif
    void xslDbgPrintTemplateNames(xsltTransformContextPtr styleCtxt,
                                  xmlShellCtxtPtr ctxt,
                                  xmlChar * arg, int verbose,
                                  int allFiles);


#ifdef USE_GNOME_DOCS
/**
 * xslDbgPrintTemplateHelper:
 * @templ: Is valid
 * @verbose: Either 1 or 0
 * @templateCount: Is valid
 * @printCount: Is valid
 * @templateName: template name to print, may be NULL
 *
 * This display the templates in the same order as they are in the 
 *   stylesheet. If verbose is 1 then print more information
 *   For each template found @templateCount is increased
 *   For each printed template @printCount is increased
 */
#else
#ifdef USE_KDE_DOCS
/**
 * This displays the templates in the same order as they are in the 
 *   stylesheet. If verbose is 1 then print more information
 *   For each template found @p templateCount is increased
 *   For each printed template @p printCount is increased
 *
 * @param templ Is valid
 * @param verbose Either 1 or 0
 * @param templateCount Is valid
 * @param printCount Is valid
 * @param templateName The template name to print, may be NULL
 */
#endif
#endif
    void xslDbgPrintTemplateHelper(xsltTemplatePtr templ, int verbose,
                                   int *templateCount, int *printCount,
                                   xmlChar * templateName);


#ifdef USE_GNOME_DOCS
/**
 * getTemplate:
 * 
 * Get the last template node found, if any
 *
 * Returns The last template node found, if any
 */
#else
#ifdef USE_KDE_DOCS
/** 
 * Get the last template node found, if any
 *
 * @returns the last template node found, if any
 */
#endif
#endif
    xsltTemplatePtr getTemplate(void);


/* -----------------------------------------

   Node viewing related commands

  ------------------------------------------- */


#ifdef USE_GNOME_DOCS
/**
 * xslDbgShellPrintList: 
 * @ctxt: The current shell context
 * @arg: What xpath to display
 * @dir: If 1 print in dir mode?, 
 *        otherwise ls mode
 *
 * Print list of nodes in either ls or dir format
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Print list of nodes in either ls or dir format
 *
 * @param ctxt The current shell context
 * @param arg What xpath to display
 * @param dir If 1 print in dir mode, 
 *        otherwise ls mode
 */
#endif
#endif
    void xslDbgShellPrintList(xmlShellCtxtPtr ctxt, xmlChar * arg,
                              int dir);



#ifdef USE_GNOME_DOCS
/** 
 * xslDbgShellCat:
 * @styleCtxt: the current stylesheet context
 * @ctxt: The current shell context
 * @arg: The xpath to print
 *
 * Print the result of an xpath expression. This can include variables
 *        if styleCtxt is not NULL
 */
#else
#ifdef USE_KDE_DOCS
/** 
 * Print the result of an xpath expression. This can include variables
 *        if styleCtxt is not NULL
 *
 * @param styleCtxt Current stylesheet context
 * @param ctxt Current shell context
 * @param arg The xpath to print
 */
#endif
#endif
    void xslDbgShellCat(xsltTransformContextPtr styleCtxt,
                        xmlShellCtxtPtr ctxt, xmlChar * arg);



#ifdef USE_GNOME_DOCS
/**
 * xslDbgShellPrintVariable:
 * @styleCtxt: The current stylesheet context 
 * @arg: The name of variable to look for '$' prefix is optional
 * @type: A valid VariableTypeEnum
 *
 *  Print the value variable specified by args.
 */
#else
#ifdef USE_KDE_DOCS
/**
 *  Print the value variable specified by args.
 *
 * @param styleCtxt The current stylesheet context 
 * @param arg The name of variable to look for '$' prefix is optional
 * @param type Is valid VariableTypeEnum
 */
#endif
#endif
    void xslDbgShellPrintVariable(xsltTransformContextPtr styleCtxt,
                                  xmlChar * arg, VariableTypeEnum type);




/* -----------------------------------------

   Operating system related commands

  ------------------------------------------- */


#ifdef USE_GNOME_DOCS
/**
 * xslDbgShellChangeWd:
 * @path: The path to change to
 *
 * Change the current working directory of the operating system
 *
 * Returns 1 if able to change xsldbg's working directory to @path
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * @returns 1 if able to change xsldbg working direcorty to @p path
 *          0 otherwise
 *
 * @param path Operating system path(directory) to change to
 */
#endif
#endif
    int xslDbgShellChangeWd(const xmlChar * path);



#ifdef USE_GNOME_DOCS
/**
 * xslDbgShellExecute:
 * @name: The name of command string to be executed by operating system shell
 * @verbose: If 1 then print extra debugging messages,
 *            normal messages otherwise
 * 
 * Execute an operating system command
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * @returns 1 if able to execute command @p name,
 *          0 otherwise
 *
 * @param name The name of command string to be executed 
 *            by operating system shell
 * @param verbose If 1 then print extra debugging messages,
 *            normal messages otherwise
 */
#endif
#endif
    int xslDbgShellExecute(xmlChar * name, int verbose);




/* -----------------------------------------

   libxslt parameter related commands

  ------------------------------------------- */


#ifdef USE_GNOME_DOCS
/** 
 * xslDbgShellAddParam:
 * @arg: A string comprised of two words separated by
 *          one or more spaces. 
 *
 * Add a libxslt parameter to be sent to libxslt later on
 *
 * Returns 1 on success,
 *         0 otherwise
*/
#else
#ifdef USE_KDE_DOCS
/* 
 * Add a parameter to be sent to libxslt later on
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param arg A string comprised of two words separated by
 *          one or more spaces. 
 */
#endif
#endif
    int xslDbgShellAddParam(xmlChar * arg);



#ifdef USE_GNOME_DOCS
/**
 * xslDbgShellDelParam:
 * @arg: A single white space trimmed parameter number to look for
 * 
 * Delet a libxslt parameter to be sent to libxslt later on
 *
 * Returns 1 if able to delete parameter @name,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 *
 * @returns 1 if able to delete parameter @p name,
 *          0 otherwise
 *
 * @param arg A single white space trimmed libxslt parameter number to look for
 */
#endif
#endif
    int xslDbgShellDelParam(xmlChar * arg);



#ifdef USE_GNOME_DOCS
/**
 * xslDbgShellShowParam:
 * @arg: Not used
 *
 * Print list of current paramters
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Print list of current paramters
 *
 * @returns 1 on success,
 *         0 otherwise
 *
 * @param arg Not used
 */
#endif
#endif
    int xslDbgShellShowParam(xmlChar * arg);


/* -----------------------------------------

   Tracing related commands

  ------------------------------------------- */


#ifdef USE_GNOME_DOCS
/**
 * xslDbgShellTrace:
 * @arg: Not used
 *
 * Start the tracing of the stylesheet. First need to restart it.
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Start the tracing of the stylesheet. First need to restart it.
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param arg Not used
 */
#endif
#endif
    int xslDbgShellTrace(xmlChar * arg);


#ifdef USE_GNOME_DOCS
/**
 * xslDbgShellWalk:
 * @arg: An interger between 0 and 9 indicate the speed of walk
 *
 * Start walking through the stylesheet.
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Start walking through the stylesheet.
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param arg An interger between 0 and 9 indicate the speed of walk
 */
#endif
#endif
    int xslDbgShellWalk(xmlChar * arg);



/* -----------------------------------------
   
   Seach related commands
   
  ------------------------------------------- */


#ifdef USE_GNOME_DOCS
/**
 * xslDbgShellSearch:
 * @styleCtxt: Is valid
 * @style: Is valid
 * @arg: The xpath query to use for searching dataBase
 * 
 * Displays the result of performing a query on the search dataBase
 *
 * Returns 1 if able to run query with @arg, 
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * @returns 1 if able to run query with @p arg, 
 *          0 otherwise
 *
 * @param styleCtxt Is valid
 * @param style Is valid
 * @param arg The xpath query to use for searching dataBase
 */
#endif
#endif
    int xslDbgShellSearch(xsltTransformContextPtr styleCtxt,
                          xsltStylesheetPtr style, xmlChar * arg);


#ifdef __cplusplus
}
#endif
#endif
 /* __DEBUG_XSL__ */
