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

#include <libxslt/xslt.h>
#include <libexslt/exslt.h>
#include <libxslt/xsltutils.h>
#include <breakpoint/breakpoint.h>
#include "config.h"

xmlDocPtr loadXmlData();
xmlDocPtr loadXmlTemporay(const char *path);
xsltStylesheetPtr loadStyleSheet();

#endif
