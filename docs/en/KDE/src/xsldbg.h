
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






/**
 * Provide provide application level services and useful bits and pieces
 *
 * @short application functions and useful bits and pieces
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */


/* end of WITH_XSL_DEBUG */

/**
 * ATTRIBUTE_UNUSED:
 *
 * This macro is used to flag unused function parameters to GCC
 */







/**
 * LIBXSLT_PUBLIC:
 *
 * This macro is used to declare PUBLIC variables for MSC on Windows
 */




/* We want skip most of these includes when building documentation */



/* 
  Make things simpler when working between char* and xmlChar*  .
   By definition a char is the same size as an xmlChar(unsigned char). 
*/








/* JRF: Note - I need to think about where these are used - mostly
        I would prefer to use URIs unless a native filename is explicitly
        required */

/* what char is use to separate directories in an URI*/


    /* Handle the differnces in path and quote character between
     * win32 and *nix systems */











/**
 * Load the stylesheet and return it 
 *
 * @returns The stylesheet after reloading it if successful
 *         NULL otherwise
 */


    xsltStylesheetPtr loadStylesheet(void);




/**
 * Load the xml data file and return it  
 *
 * @returns The stylesheet after reloading it if successful
 *         NULL otherwise
 */


    xmlDocPtr loadXmlData(void);




/**
 * Load the temporary data file and return it 
 *
 * @returns The temporary file after reloading it if successful,
 *         NULL otherwise
 * @param path The name of temporary file to loa
 */


    xmlDocPtr loadXmlTemporary(const xmlChar * path);




