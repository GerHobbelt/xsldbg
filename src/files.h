
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

#ifdef HAVE_UNISTD_H
#include <unistd.h>             /* need chdir function */
#endif

/* used by loadXmlFile function */
enum File_type {
    FILES_XMLFILE_TYPE = 100,
    FILES_SOURCEFILE_TYPE,
    FILES_TEMPORARYFILE_TYPE
};

/**
 * redirectToTerminal:
 * @device: terminal to redirect i/o to , will not work under win32
 */
int openTerminal(xmlChar *device);


/**
 * selectTerminalIO:
 * Return 1 if able to use prevously opened terminal 
 *        0 otherwise
*/
int selectTerminalIO(void);


/** 
 * selectNormalIO:
 * 
 * Returns 1 if able to select orginal stdin, stdout, stderr
 *         0 otherwise
*/
int 
selectNormalIO(void);

/**
 * guessStyleSheetName:
 *
 * Try to find a matching stylesheet name
 * Returns non-NULL if found,
 *          NULL otherwise
 */
xmlChar *guessStyleSheetName(xmlChar* name);


/**
 * stylePath:
 *
 * Return the base path for the top stylesheet ie
 *        ie URL minus the actual file name
 */
xmlChar *stylePath(void);


/**
 * workingPath:
 *
 * Return the working directory as set by changeDir function
 */
xmlChar*
workingPath(void);


/**
 * changeDir:
 * @path : path to adopt as new working directory
 *
 * Change working directory to path 
 */
int changeDir(const xmlChar * path);

/**
 * loadXmlFile:
 * @path : xml file to load
 * @type : a valid File_Type
 *
 * Returns 1 on success,
 *         0 otherwise 
 */
int loadXmlFile(const xmlChar * path, enum File_type file_type);


/**
 * freeXmlFile:
 * @type : a valid File_Type
 * 
 * Free memory associated with the xml file 
 * Returns 1 on success,
 *         0 otherwise
 */
int freeXmlFile(enum File_type file_type);


/**
 * getStylesheet:
 *
 * Return the topmost stylesheet 
 * Returns non-null on success,
 *         NULL otherwise
 */
xsltStylesheetPtr getStylesheet(void);

/**
 * getTemporaryDoc:
 *
 * Returns the current "temporary" document
 */
xmlDocPtr getTemporayDoc(void);


/**
 * getMainDoc:
 *
 * Returns the main docment
 */
xmlDocPtr getMainDoc(void);


/**
 * filesReloaded:
 * @reloaded : if = -1 then ignore @reloaded
 *             otherwise change the status of files to value of @reloaded   
 *
 * Returns 1 if stylesheet or its xml data file has been "flaged" as reloaded,
 *         0 otherwise
 */
int filesReloaded(int reloaded);


/**
 * filesInit:
 *
 * Initialize the file related structures
 * Returns 1 on success,
 *         0 otherwise
 */
int filesInit(void);

/**
 * filesFree:
 *
 * Free memory used by file related structures
 */
void filesFree(void);




#endif
