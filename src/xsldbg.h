
/***************************************************************************
                          xsldbg.h  - descibe the application level functions
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


#ifndef XSLDEBUGGER_H
#define XSLDEBUGGER_H

#ifdef USE_KDE_DOCS
/**
 * Provide provide application level services and useful bits and pieces
 *
 * @short application functions and useful bits and pieces
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */
#endif

#ifdef  WITH_XSLT_DEBUG

#ifndef WITH_XSLT_DEBUG_SHELL
#define WITH_XSLT_DEBUG_SHELL
#endif

#ifndef WITH_XSLT_DEBUG_EXTRA
#define WITH_XSLT_DEBUG_EXTRA
#endif

#ifndef WITH_XSLT_DEBUG_PROCESS
#define WITH_XSLT_DEBUG_PROCESS
#endif

#ifndef  WITH_XSL_DEBUG_HELP
#define WITH_XSL_DEBUG_HELP
#endif

#endif /* end of WITH_XSL_DEBUG */

/**
 * ATTRIBUTE_UNUSED:
 *
 * This macro is used to flag unused function parameters to GCC
 */
#ifdef __GNUC__
#ifdef HAVE_ANSIDECL_H
#include <ansidecl.h>
#endif
#ifndef ATTRIBUTE_UNUSED
#define ATTRIBUTE_UNUSED
#endif
#else
#define ATTRIBUTE_UNUSED
#endif

/**
 * LIBXSLT_PUBLIC:
 *
 * This macro is used to declare PUBLIC variables for MSC on Windows
 */
#if !defined(WIN32) || defined(__CYGWIN__)
#define LIBXSLT_PUBLIC
#endif

/* We want skip most of these includes when building documentation */
#ifndef BUILD_DOCS
#include <signal.h>
#include <libxslt/xslt.h>
#include <libexslt/exslt.h>
#include <libxslt/xsltutils.h>
#include "breakpointInternals.h"

#ifndef __riscos
#ifndef WIN32
#include "config.h"
#else
#include "xsldbgwin32conf.h"	
#endif
#else
#include "config_riscos.h"
#endif

#endif


/* 
  Make things simpler when working between char* and xmlChar*  .
   By definition a char is the same size as an xmlChar(unsigned char). 
*/
#define xmlStrLen(text) strlen((char*)(text))
#define xmlStrCat(a, b) strcat((char*)(a), (char*)(b))
#define xmlStrCmp(a, b) strcmp((char*)(a), (char*)(b))
#define xmlStrCpy(a, b) strcpy((char*)(a), (char*)(b))
#define xmlStrnCpy(a, b, c) strncpy((char*)(a),(char*)(b), c)
#define xmlStrChr(a, b) strchr((char*)(a), b)
#define xmlStrrChr(a, b) strrchr((char*)(a), b)

/* what char is use to separate directories in an URI*/
#define URISEPARATORCHAR '/'

    /* Handle the differences in path and quote character between
     * win32 and *nix systems */
#ifdef WIN32
#define  QUOTECHAR ' '
#define  PATHCHAR '\\'
#else
#define  QUOTECHAR '\"'
#define  PATHCHAR  '/'
#endif
/* JRF: Although RISC OS native paths use . as a separator, the arguments
        to xsldbg are passed in unix or URI form, and thus the above
        specification is correct. */
#ifdef __cplusplus
extern "C" {
#endif



#ifdef USE_GNOME_DOCS
/**
 * loadStylesheet:
 *
 * Load the stylesheet and return it 
 *
 * Returns the stylesheet after reloading it if successful
 *         NULL otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Load the stylesheet and return it 
 *
 * @returns The stylesheet after reloading it if successful
 *         NULL otherwise
 */
#endif
#endif
    xsltStylesheetPtr loadStylesheet(void);


#ifdef USE_GNOME_DOCS
/**
 * loadXmlData:
 *
 * Load the xml data file and return it  
 *
 * Returns the data file after reloading it if successful
 *         NULL otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Load the xml data file and return it  
 *
 * @returns The stylesheet after reloading it if successful
 *         NULL otherwise
 */
#endif
#endif
    xmlDocPtr loadXmlData(void);


#ifdef USE_GNOME_DOCS
/**
 * loadXmlTemporary:
 * @path: The name of temporary file to load 
 *
 * Load the temporary data file and return it 
 *
 * Returns The temporary file after reloading it if successful,
 *         NULL otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Load the temporary data file and return it 
 *
 * @returns The temporary file after reloading it if successful,
 *         NULL otherwise
 * @param path The name of temporary file to loa
 */
#endif
#endif
    xmlDocPtr loadXmlTemporary(const xmlChar * path);


#ifdef __cplusplus
}
#endif
#endif
