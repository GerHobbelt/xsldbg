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

#include "files.h"
#include "options.h"

/* top xml document */
xmlDocPtr top_doc;

/* temporary xml document */
xmlDocPtr temp_doc;

/* top stylsheet */
xsltStylesheetPtr top_style;


/**
 * changeDir:
 * @path : path to adopt as new working directory
 *
 * Change working directory to path 
 */
int changeDir(const char *path){
  int result = 0;
  if (chdir(path) == 0){
    result++;
  }
  if (!result)
    printf("Unable to change to directory %s\n", path); 
  else
    printf("Change to directory %s\n", path); 
}

/**
 * loadXmlFile:
 * @path : xml file to load
 * @type : a valid File_Type
 *
 * Returns 1 on success,
 *         0 otherwise 
 */   
int loadXmlFile(const char *path, enum File_type file_type){
  int result = 0; 
  int type = file_type; /* shouldn't be needed but just to be safe */

  if (!freeXmlFile(file_type))
    return result;

  switch(file_type){
  case FILES_XMLFILE_TYPE:
    if (path && strlen(path)){
      printf("Setting xml data file name to %s\n", path);
      setStringOption(OPTIONS_DATA_FILE_NAME, path);
    }
    top_doc = loadXmlData();
    if (top_doc)
      result++;
    break;

  case FILES_SOURCEFILE_TYPE:
    if (path && strlen(path)){
      printf("Setting stylesheet file name to %s\n", path);
      setStringOption(OPTIONS_SOURCE_FILE_NAME, path);
    }
    top_style = loadStyleSheet();
    if (top_style)
      result++;
    break;

  case FILES_TEMPORARYFILE_TYPE:
    if (!path || !strlen(path)){
      printf("Missing file name\n");
      break;
    }
    top_doc = loadXmlTemporay(path);
    if (temp_doc)
      result++;
    break;
  }
  return result;
}


/**
 * freeXmlFile:
 * @type : a valid File_Type
 * 
 * Free memory associated with the xml file 
 * Returns 1 on success,
 *         0 otherwise
 */
int freeXmlFile(enum File_type file_type){
  int result = 0, type = file_type;
  switch(type){
  case FILES_XMLFILE_TYPE:
    if (top_doc)
      xmlFreeDoc(top_doc);
    top_doc = NULL;
    result++;
    break;

  case FILES_SOURCEFILE_TYPE:
    if (top_style)
      xsltFreeStylesheet(top_style);
    top_style = NULL;
    result++;
    break;

  case FILES_TEMPORARYFILE_TYPE:
    if (temp_doc)
      xmlFreeDoc(temp_doc);
    temp_doc = NULL;
    result++;
    break;
  }
  return result;
}




/**
 * getStylesheet:
 *
 * Return the topmost stylesheet 
 * Returns non-null on success,
 *         NULL otherwise
 */
xsltStylesheetPtr getStylesheet(){
  return top_style;
}

/**
 * getTemporaryDoc:
 *
 * Returns the current "temporary" document
 */
xmlDocPtr getTemporayDoc(){
  return temp_doc;
}

/**
 * getMainDoc:
 *
 * Returns the main docment
 */
xmlDocPtr getMainDoc(){
  return top_doc;
}


/**
 *
 *
 * Returns 1 if stylesheet or its xml data file has been reloaded ,
 *         0 otherwise
 */
int filesReloaded(int reloaded){
  int result = 0;
  return result;
}



/**
 * filesInit:
 *
 * Initialize the file related structures
 * Returns 1 on success,
 *         0 otherwise
 */
int filesInit(){
  int result = 0;

  top_doc = NULL;
  temp_doc = NULL;
  top_style = NULL;
  result = 1; /* nothing else  to do for the moment*/

  return result;
}

/**
 * filesFree:
 *
 * Free memory used by file related structures
 */
void filesFree(){
  int result;
  result  = freeXmlFile(FILES_SOURCEFILE_TYPE);
  if (result)
    result  = freeXmlFile(FILES_XMLFILE_TYPE);  
  if (result)
    result  = freeXmlFile(FILES_TEMPORARYFILE_TYPE);   
  if (!result)
    printf("Unable to free memory used by xml/xsl files\n");
}
