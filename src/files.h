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

#include "xsldbg.h"

/* used by loadXmlFile function */
enum File_type{
   FILES_XMLFILE_TYPE = 100,
  FILES_SOURCEFILE_TYPE,
  FILES_TEMPORARYFILE_TYPE};

/**
 * changeDir:
 * @path : path to adopt as new working directory
 *
 * Change working directory to path 
 */
int changeDir(const char *path);

/**
 * loadXmlFile:
 * @path : xml file to load
 * @type : a valid File_Type
 *
 * Returns 1 on success,
 *         0 otherwise 
 */   
int loadXmlFile(const char *path, enum File_type file_type);


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
xsltStylesheetPtr getStylesheet();

/**
 * getTemporaryDoc:
 *
 * Returns the current "temporary" document
 */
xmlDocPtr getTemporayDoc();


/**
 * getMainDoc:
 *
 * Returns the main docment
 */
xmlDocPtr getMainDoc();



/**
 *
 *
 * Returns 1 if stylesheet or its xml data file has been reloaded ,
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
int filesInit();

/**
 * filesFree:
 *
 * Free memory used by file related structures
 */
void filesFree();




#endif 
