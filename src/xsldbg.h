
/***************************************************************************
                          xsldbg.h  -  description
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

#include <signal.h>
#include <libxslt/xslt.h>
#include <libexslt/exslt.h>
#include <libxslt/xsltutils.h>
#include <breakpoint/breakpoint.h>

#ifndef __riscos
  #include "config.h"
#else
  #include "config_riscos.h"
#endif

#define xmlStrLen(text) strlen((char*)(text))
#define xmlStrCat(a, b) strcat((char*)(a), (char*)(b))
#define xmlStrCmp(a, b) strcmp((char*)(a), (char*)(b))
#define xmlStrCpy(a, b) strcpy((char*)(a), (char*)(b))
#define xmlStrnCpy(a, b, c) strncpy((char*)(a),(char*)(b), c)
#define xmlStrChr(a, b) strchr((char*)(a), b)
#define xmlStrrChr(a, b) strrchr((char*)(a), b)
/* JRF: Note - I need to think about where these are used - mostly
        I would prefer to use URIs unless a native filename is explicitly
        required */
    /* Handle the differnces in path and quote character between
     * win32 and Unix/Linux systems */
#ifdef WIN32
    #define  QUOTECHAR ' '
    #define  PATHCHAR '\\'
#else
    #define  QUOTECHAR '\"'
    #define  PATHCHAR  '/'
#endif

/**
 * loadStylesheet:
 *
 * Return the stylesheet after reloading it if successful
 *         NULL otherwise
 */
xsltStylesheetPtr loadStylesheet(void);

/**
 * loadXmlData:
 *
 * Return the data file after reloading it if successful
 *         NULL otherwise
 */
xmlDocPtr loadXmlData(void);


/**
 * loadXmlTemporary:
 *
 * Return the temporary file after reloading it if successful,
 *         NULL otherwise
 */
xmlDocPtr loadXmlTemporay(const xmlChar * path);

#endif
