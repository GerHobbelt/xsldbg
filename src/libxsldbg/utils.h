
/**
 *
 *  This file is part of the kdewebdev package
 *  Copyright (c) 2002 Keith Isdale <keith@kdewebdev.org>
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


#ifndef XSLDBG_UTILS_H
#define XSLDBG_UTILS_H

#include <stdio.h>
#include <string.h>
#include <libxml/tree.h>
#include <libxml/debugXML.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/xsltutils.h>
#include <libxml/xpath.h>

#include "xsldbg.h"


/*
   Make things simpler when working between char* and xmlChar*
   By definition a char is the same size as an xmlChar(unsigned char)
 */

#define xmlStrLen(text) strlen((char*)(text))
#define xmlStrCat(a, b) strcat((char*)(a), (char*)(b))
#define xmlStrCmp(a, b) strcmp((char*)(a), (char*)(b))
#define xmlStrnCmp(a, b, c) strncmp((char*)(a), (char*)(b), c)
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



/**
 * Remove leading and trailing spaces off @p text
 *         stores result back into @p text
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param text A valid string with leading or trailing spaces
 */
int trimString(xmlChar * text);


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
int splitString(xmlChar * textIn, int maxStrings, xmlChar ** out);


/**
 * Lookup and name in a list
 *
 *
 * @returns The id of name found in @p matchList
 *         0 otherwise
 *
 * @param name Is valid
 * @param matchList A NULL terminated list of names to use as lookup table
 *
 */
int lookupName(QString name, const char ** matchList);


/**
 * Join nameURI to name
 *
 * @returns a copy of "nameURI:name"

 * fullQName:
 * @param nameURI : QName part of name
 * @param name : Local part of name 
 *
 *
 */
xmlChar * fullQName(const xmlChar* nameURI, const xmlChar * name);


/**
 * Converts a fileName to an absolute path
 *          If operating system supports it a leading "~" in the fileName
 *          will be converted to the user's home path. Otherwise
 *          the same name will be returned
 *
 * Returns A copy of the converted @p fileName or a copy of 
 *           the @p fileName as supplied. May return NULL
 *
 * @param fileName A valid fileName
 */
QString filesExpandName(const QString fileName, bool addFilePrefix=true);


/**
 * Ensure that paths with a "file:/" have the correct number of leading slashes,ie file:///
 *
 * @returns the corrected version of @p file
 */
QString fixLocalPaths(QString & file);

#endif
