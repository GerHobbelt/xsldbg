
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

#ifdef VERSION
#undef VERSION
#endif

#include "config.h"
#include "xsldbg.h"
#include "debugXSL.h"
#include "files.h"
#include "options.h"
#include <stdio.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>  
#endif




/* private data for search for file names*/
typedef  struct _fileSearch FileSearch;
typedef FileSearch *FileSearchPtr;
struct _fileSearch{
  xmlChar *nameInput;
  xmlChar *absoluteNameMatch;
  xmlChar *guessedNameMatch;  
};

/* top xml document */
xmlDocPtr top_doc;

/* temporary xml document */
xmlDocPtr temp_doc;

/* top stylsheet */
xsltStylesheetPtr top_style;

/* what is the base path for top stylesheet */
xmlChar *stylePathName = NULL;

/* what is the path for current working directory*/
xmlChar *workingDirPath = NULL;

/* used as a scratch pad for temporary results*/
xmlChar buffer[DEBUG_BUFFER_SIZE];

FILE *terminalIn = NULL, *terminalOut = NULL;

/**
 * redirectToTerminal:
 * @device: terminal to redirect i/o to , will not work under win32
 */
int 
redirectToTerminal(xmlChar *device)
{
  int result =0;
#ifndef WIN32 /* fix me for WinNT!! */
  if (terminalIn != NULL)
    fclose(terminalIn);  
  if (terminalOut != NULL)
    fclose(terminalOut); 
  
  terminalOut = fopen(device, "w+");  
  if (terminalOut != NULL){
    /* make sure that stdout and stderr are mapped to our terminalOut*/
    dup2(fileno(terminalOut), fileno(stdout));
    dup2(fileno(terminalOut), fileno(stderr));    
    /*    if (stdout)
      fclose(stdout);
    stdout =  terminalOut;
    if (stderr)
      fclose(stderr);
    stderr =  terminalOut;
    */
  }
  terminalIn = fopen(device, "a+"); 
  if (terminalIn != NULL){
    /* make sure that stdin is mapped to termimalIn /
    dup2(fileno(terminalIn), fileno(stdin));
    /*
    char *c ;
    if (stdin)
      fclose(stdin); 
    stdin =  terminalIn;
    */
  }
  if (terminalIn && terminalOut){
    char c = 0;
    fprintf(stdout,"Hi there\n");
    while (c != -1){
      c = getc(stdin);
      fprintf(stdout, "%c", c);
    }
  }else
    printf("Can't open terminal %s\n" , device);  
  result++;
  
#else
    printf ("Terminals are no supported by this operating system\n");
#endif
    return result;    
}


void guessStyleSheetHelper(void *payload ATTRIBUTE_UNUSED,
			   void *data ATTRIBUTE_UNUSED, xmlChar * name)
{
  xsltStylesheetPtr style = (xsltStylesheetPtr)payload;
  FileSearchPtr searchData = (FileSearchPtr)data;

  if (style && style->doc && searchData && searchData->nameInput &&
      (searchData->absoluteNameMatch == NULL)){
    /* at this point we know that we have not made an absolute match 
       but we may have made a relative match */
    if (xmlStrCmp(style->doc->URL, searchData->nameInput) == 0){
      /* absolute path match great!*/
      searchData->absoluteNameMatch = (xmlChar*)xmlMemStrdup(style->doc->URL);
      return;
    }


    /* try to guess we assume that the files are unique */
    xmlStrCpy(buffer,"__#!__");
    /* try relative to top stylesheet directory */
    if (stylePath()){
	xmlStrCpy(buffer, stylePath());
	xmlStrCat(buffer, searchData->nameInput);
    }
    if (xmlStrCmp(style->doc->URL, buffer) == 0){
      /* guessed right!*/
      searchData->guessedNameMatch = (xmlChar*)xmlMemStrdup(buffer);
      return;
    }

    if (workingPath()){
	  /* try relative to working directory */
	  xmlStrCpy(buffer, workingPath());
	  xmlStrCat(buffer, searchData->nameInput);
    }
    if ( xmlStrCmp(style->doc->URL, buffer) == 0){
      /* guessed right!*/
      searchData->guessedNameMatch = (xmlChar*)xmlMemStrdup(buffer);
      return;
    }

    if (xmlStrChr(searchData->nameInput, PATHCHAR) == NULL){
      /* Last try, nameInput contains only a file name, and no path specifiers
	 Strip of the file name at end of the stylesheet doc URL */
      char *lastSlash = xmlStrrChr(style->doc->URL, PATHCHAR);
	  if (lastSlash){
	    lastSlash++; /* skip the slash */
	    if (xmlStrCmp(lastSlash, searchData->nameInput) == 0){
	      /* guessed right!*/
	      searchData->guessedNameMatch = (xmlChar*)xmlMemStrdup(style->doc->URL);
	    }
	  }
    }
  }
}  

/**
 * guessStyleSheetName:
 *
 * Try to find a matching stylesheet name
 * Returns non-NULL if found,
 *          NULL otherwise
 */
xmlChar *guessStyleSheetName(xmlChar* name)
{
  xmlChar *result = NULL;
  FileSearch searchData = {name, NULL, NULL};
  if (name){
    walkStylesheets((xmlHashScanner)guessStyleSheetHelper, &searchData,
		     getStylesheet());    
    if (searchData.absoluteNameMatch)
      result = searchData.absoluteNameMatch;
    else
      result =  searchData.guessedNameMatch;
  }
  return result;
}


/**
 * stylePath:
 *
 * Return the base path for the top stylesheet ie
 *        ie URL minus the actual file name
 */
xmlChar *
stylePath()
{
  return stylePathName;
}

/**
 * workingPath:
 *
 * Return the working directory as set by changeDir function
 */
xmlChar*
workingPath()
{
  return workingDirPath;
}

/**
 * changeDir:
 * @path : path to adopt as new working directory
 *
 * Change working directory to path 
 */
int
changeDir(const xmlChar * path)
{
    int result = 0;
    const xmlChar endString[2] = {PATHCHAR,'\0'};
    if (path && chdir((char *) path) == 0) {
      if (workingDirPath)
	xmlFree(workingDirPath);
       /* must have path char at end of path name*/
      xmlStrCpy(buffer, path);
      xmlStrCat(buffer, endString);
      workingDirPath = (xmlChar*)xmlMemStrdup(buffer);
      result++;
    }
    if (!result)
        printf("Unable to change to directory %s\n", path);
    else
        printf("Change to directory %s\n", path);
    return result;
}

/**
 * loadXmlFile:
 * @path : xml file to load
 * @type : a valid File_Type
 *
 * Returns 1 on success,
 *         0 otherwise 
 */
int
loadXmlFile(const xmlChar * path, enum File_type file_type)
{
    int result = 0;

    if (!freeXmlFile(file_type))
        return result;

    switch (file_type) {
        case FILES_XMLFILE_TYPE:
            if (path && xmlStrLen(path)) {
	      if (isOptionEnabled(OPTIONS_SHELL)) { 
                xsltGenericError(xsltGenericErrorContext,
				 "Setting xml data file name to %s\n", path);
	      }
	      setStringOption(OPTIONS_DATA_FILE_NAME, path);
            }
            top_doc = loadXmlData();
            if (top_doc)
                result++;
            break;

        case FILES_SOURCEFILE_TYPE:
            if (path && xmlStrLen(path)) {
	      if (isOptionEnabled(OPTIONS_SHELL)) { 
		xsltGenericError(xsltGenericErrorContext,
				 "Setting stylesheet file name to %s\n", path);
	      }
	      setStringOption(OPTIONS_SOURCE_FILE_NAME, path);
	    }
            top_style = loadStylesheet();
            if (top_style && top_style->doc){
	      /* look for last slash (or baskslash) of URL*/
	       char *lastSlash =   xmlStrrChr(top_style->doc->URL, PATHCHAR);
	       const char *docUrl = top_style->doc->URL;
                result++;		
		if (docUrl && lastSlash){
		    stylePathName = xmlMemStrdup(docUrl);
		    stylePathName[lastSlash - docUrl + 1] = '\0';
		    if (isOptionEnabled(OPTIONS_SHELL)) {   
		    	xsltGenericError(xsltGenericErrorContext,
					 "Setting stylesheet base path to %s\n", 
					 stylePathName);
		    }
		}
	    }
            break;

        case FILES_TEMPORARYFILE_TYPE:
            if (!path || !xmlStrLen(path)) {
                xsltGenericError(xsltGenericErrorContext,"Missing file name\n");
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
int
freeXmlFile(enum File_type file_type)
{
    int result = 0, type = file_type;

    switch (type) {
        case FILES_XMLFILE_TYPE:
            if (top_doc)
                xmlFreeDoc(top_doc);
            top_doc = NULL;
            result++;
            break;

        case FILES_SOURCEFILE_TYPE:
            if (top_style)
                xsltFreeStylesheet(top_style);
	    if (stylePathName)
	      xmlFree(stylePathName);
	    stylePathName = NULL;
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
xsltStylesheetPtr
getStylesheet(void)
{
    return top_style;
}

/**
 * getTemporaryDoc:
 *
 * Returns the current "temporary" document
 */
xmlDocPtr
getTemporayDoc(void)
{
    return temp_doc;
}

/**
 * getMainDoc:
 *
 * Returns the main docment
 */
xmlDocPtr
getMainDoc(void)
{
    return top_doc;
}


/**
 * filesReloaded:
 * @reloaded : if = -1 then ignore @reloaded
 *             otherwise change the status of files to value of @reloaded   
 *
 * Returns 1 if stylesheet or its xml data file has been "flaged" as reloaded,
 *         0 otherwise
 */
int
filesReloaded(int reloaded)
{
    static int changed = 0;

    if (reloaded >= 0) {
        changed = reloaded;
    }

    return changed;
}



/**
 * filesInit:
 *
 * Initialize the file related structures
 * Returns 1 on success,
 *         0 otherwise
 */
int
filesInit(void)
{
    int result = 0;

    top_doc = NULL;
    temp_doc = NULL;
    top_style = NULL;
    result = 1;                 /* nothing else  to do for the moment */

    return result;
}

/**
 * filesFree:
 *
 * Free memory used by file related structures
 */
void
filesFree(void)
{
    int result;
  if (terminalIn != NULL)
    fclose(terminalIn);  
  if (terminalOut != NULL)
    fclose(terminalOut);


    result = freeXmlFile(FILES_SOURCEFILE_TYPE);
    if (result)
        result = freeXmlFile(FILES_XMLFILE_TYPE);
    if (result)
        result = freeXmlFile(FILES_TEMPORARYFILE_TYPE);
    if (!result)
        xsltGenericError(xsltGenericErrorContext,
			 "Unable to free memory used by xml/xsl files\n");
    if (workingDirPath)
	xmlFree(workingDirPath);
}
