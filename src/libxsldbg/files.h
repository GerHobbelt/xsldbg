
/***************************************************************************
                          files.h  -  define file related functions
                             -------------------
    begin                : Sat Nov 10 2001
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

#ifndef FILES_H
#define FILES_H

#include "xslsearch.h"

#ifdef USE_KDE_DOCS
/**
 * Provide a file support
 *
 * @short file support
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>             /* need chdir function */
#endif

extern FILE *terminalIO;

#ifndef USE_KDOC 
  /* used by loadXmlFile, freeXmlFile functions */
    typedef enum {
        FILES_XMLFILE_TYPE = 100,       /* pick a unique starting point */
        FILES_SOURCEFILE_TYPE,
        FILES_TEMPORARYFILE_TYPE
    } FileTypeEnum;
#else
/* used by loadXmlFile, freeXmlFile functions */
    enum FileTypeEnum {
        FILES_XMLFILE_TYPE = 100,       /* pick a unique starting point */
        FILES_SOURCEFILE_TYPE,
        FILES_TEMPORARYFILE_TYPE
    };
#endif

#ifdef USE_GNOME_DOCS
/**
 * redirectToTerminal:
 * @device: terminal to redirect i/o to , will not work under win32
 *
 * Open communications to the terminal device @device
 * Returns 1 if sucessful
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

#endif
#endif
    int openTerminal(xmlChar * device);



#ifdef USE_GNOME_DOCS
/**
 * selectTerminalIO:
 *
 * Returns 1 if able to use prevously opened terminal 
 *         0 otherwise
*/
#else
#ifdef USE_KDE_DOCS
/**
 * @returns 1 if able to use prevously opened terminal 
 *          0 otherwise
*/
#endif
#endif
    int selectTerminalIO(void);


#ifdef USE_GNOME_DOCS
/** 
 * selectNormalIO:
 * 
 * Returns 1 if able to select orginal stdin, stdout, stderr
 *         0 otherwise
*/
#else
#ifdef USE_KDE_DOCS
/** 
 * @returns 1 if able to select orginal stdin, stdout, stderr
 *          0 otherwise
*/
#endif
#endif
    int selectNormalIO(void);


#ifdef USE_GNOME_DOCS
/**
 * guessStyleSheetName:
 * @searchInf : is valid
 *
 * Try to find a matching stylesheet name
 * Sets the values in @searchinf depending on outcome of search
 * 
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Open communications to the terminal device @p device
 *
 * @returns 1 if sucessful
 *          0 otherwise
 *
 * @param device Terminal to redirect i/o to, will not work under win32
 */
#endif
#endif
    void guessStylesheetName(searchInfoPtr searchInf);



#ifdef USE_GNOME_DOCS
/**
 * stylePath:
 *
 * Return the base path for the top stylesheet ie
 *        ie URL minus the actual file name
 */
#else
#ifdef USE_KDE_DOCS
/**
 * @return the base path for the top stylesheet ie
 *        ie URL minus the actual file name
 */
#endif
#endif
    xmlChar *stylePath(void);



#ifdef USE_GNOME_DOCS
/**
 * workingPath:
 *
 * Return the working directory as set by changeDir function
 */
#else
#ifdef USE_KDE_DOCS
/**
 * @return the working directory as set by changeDir function
 */
#endif
#endif
    xmlChar *workingPath(void);



#ifdef USE_GNOME_DOCS
/**
 * changeDir:
 * @path : path to adopt as new working directory
 *
 * Change working directory to path 
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Change working directory to path 
 *
 * @param path The operating system path(directory) to adopt as 
 *         new working directory
 *
 * @returns 1 on success,
 *          0 otherwise
 */
#endif
#endif
    int changeDir(const xmlChar * path);


#ifdef USE_GNOME_DOCS
/**
 * loadXmlFile:
 * @path : xml file to load
 * @fileType : A valid FileTypeEnum
 *
 * Load specified file type, freeing any memory previously used 
 *
 * Returns 1 on success,
 *         0 otherwise 
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Load specified file type, freeing any memory previously used 
 *
 * @returns 1 on success,
 *         0 otherwise 
 *
 * @param path The xml file to load
 * @param fileType A valid FileTypeEnum
 */
#endif
#endif
    int loadXmlFile(const xmlChar * path, FileTypeEnum fileType);


#ifdef USE_GNOME_DOCS
/**
 * freeXmlFile:
 * @fileType : A valid FileTypeEnum
 * 
 * Free memory associated with the xml file 
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Free memory associated with the xml file 
 *
 * @returns 1 on success,
 *         0 otherwise
 *
 * @param fileType : A valid FileTypeEnum
 */
#endif
#endif
    int freeXmlFile(FileTypeEnum fileType);


#ifdef USE_GNOME_DOCS
/**
 * getStylesheet:
 *
 * Return the topmost stylesheet 
 * Returns non-null on success,
 *         NULL otherwise
 */
#else
#ifdef USE_KDE_DOCS

#endif
#endif
    xsltStylesheetPtr getStylesheet(void);


#ifdef USE_GNOME_DOCS
/**
 * getTemporaryDoc:
 *
 * Returns the current "temporary" document
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Return the topmost stylesheet 
 *
 * @returns non-null on success,
 *          NULL otherwise
 */
#endif
#endif
    xmlDocPtr getTemporaryDoc(void);


#ifdef USE_GNOME_DOCS
/**
 * getMainDoc:
 *
 * Returns the main docment
 */
#else
#ifdef USE_KDE_DOCS
/**
 * @returns the main document
 */
#endif
#endif
    xmlDocPtr getMainDoc(void);


#ifdef USE_GNOME_DOCS
/**
 * filesReloaded:
 * @reloaded : if = -1 then ignore @reloaded
 *             otherwise change the status of files to value of @reloaded   
 *
 * Returns 1 if stylesheet or its xml data file has been "flaged" as reloaded,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * @returns 1 if stylesheet or its xml data file has been "flaged" as reloaded,
 *         0 otherwise
 *
 * @param reloaded If = -1 then ignore @p reloaded
 *             otherwise change the status of files to value of @p reloaded
 */
#endif
#endif
    int filesReloaded(int reloaded);


#ifdef USE_GNOME_DOCS
/**
 * filesInit:
 *
 * Initialize the file module
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Initialize the file module
 *
 * @returns 1 on success,
 *          0 otherwise
 */
#endif
#endif
    int filesInit(void);


#ifdef USE_GNOME_DOCS
/**
 * filesFree:
 *
 * Free memory used by file related structures
 */
#else
#ifdef USE_KDE_DOCS
/**
 * Free memory used by file related structures
 */
#endif
#endif
    void filesFree(void);


#ifdef __cplusplus
}
#endif
#endif
