
/***************************************************************************
                          files_unix.c  -  file functions *nix platform
                                                specific
                             -------------------
    begin                : Tue Jan 29 2002
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

#include "xsldbg.h"
#include "files.h"
#include "utils.h"
#include "options.h"

/* Note: These are native filenames; they can be accessed directly by fopen,
         etc. But they cannot be operated on to extract components from */

static const char *tempNames[] = {
    "<Wimp$ScrapDir>.XSLD_0",
    "<Wimp$ScrapDir>.XSLD_1"
};

  /**
   * filesPlatformInit:
   *
   * Intialize the platform specific files module
   *
   *  This is a platform specific interface
   *
   * Returns 1 if sucessful
   *         0 otherwise  
   */
int
filesPlatformInit(void)
{
    return 1;
}


  /**
   * filesPlatformFree:
   *
   * Free memory used by the platform specific files module
   *
   *  This is a platform specific interface
   *
   */
void
filesPlatformFree(void)
{
    /* empty */
}

  /**
   * filesTempFileName:
   * @fileNumber : Number of temp file required
   *
   * Return the name of tempfile. For each call to this function
   *     with the same @fileNumber the same file name will be returned
   *     File number : 0 is used by cat command
   *     File number : 1 is used by profiling output  
   *
   *  This is a platform specific interface
   *
   *
   * Returns The name of temp file to be used for temporary results, 
   *         NULL otherwise
   */
const char *
filesTempFileName(int fileNumber)
{

    const char *result = NULL;

    if ((fileNumber < 0) || ((fileNumber + 1) > (int) sizeof(tempNames)))
        xsltGenericError(xsltGenericErrorContext,
                         "Unable to allocate temporary file %d for xsldbg\n",
                         fileNumber);
    else
        result = tempNames[fileNumber];

    return result;
}


  /**
   * filesExpandName:
   * @fileName : A valid fileName
   *
   * Converts a fileName to an absolute path
   *          If operating system supports it a leading "~" in the fileName
   *          will be converted to the user's home path. Otherwise
   *          the same name will be returned
   *
   * Returns A copy of the converted @fileName or a copy of 
   *           the @fileName as supplied. May return NULL
   */
xmlChar *
filesExpandName(const xmlChar * fileName)
{
    /* risc os does not support exanded file names */
    xmlChar *result = NULL;

    if (fileName) {
        result = xmlStrdup(fileName);
        if (!result) {
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Out of memory\n");
        }
    }
    return result;
}

  /**
   * filesSearchFileName:
   * @fileType : Is valid
   *
   * Return a copy of the file name to use as an argument to searching
   *
   * Returns A copy of the file name to use as an argument to searching
   */
  xmlChar *filesSearchFileName(FilesSearchFileNameEnum fileType){
  xmlChar *result = NULL;
  int type = fileType;
  int preferHtml = optionsGetIntOption(OPTIONS_PREFER_HTML);
  const xmlChar *baseDir = NULL;
  const xmlChar *name = NULL;
  static const char* searchNames[] = {
    /* First list names when prefer html is false*/
    ".searchresult/xml", /* input  */
    ".search/xsl",        /* stylesheet to use*/
    ".searchresult/txt",  /* where to put the result*/
    /*Now for the names to use when prefer html is true */
    ".searchresult/xml", /* input  */   
    ".searchhtml/xsl",    /* stylesheet to use*/
    ".searchresult/html"  /* where to put the result*/
  };

  if (!optionsGetStringOption(OPTIONS_DOCS_PATH) || !stylePath()){
    xsltGenericError(xsltGenericErrorContext,
		     "Error: Null docs dir path or Null stylesheet path\n");
    return result;
  }
    

  name = (xmlChar*)searchNames[(preferHtml * 3)  + type];
    switch(type){
    case FILES_SEARCHINPUT:
      baseDir = stylePath();
      break;

    case FILES_SEARCHXSL:
      baseDir = optionsGetStringOption(OPTIONS_DOCS_PATH);
      break;

    case FILES_SEARCHRESULT:
      baseDir = stylePath();
      break;    
    }
    
    result = xmlMalloc(xmlStrLen(baseDir) + xmlStrLen(name) + 1);
    if (result){
      xmlChar* temp = result;
      xmlStrCpy(result, baseDir);
      xmlStrCat(result, name);
      /* We're going to pass a native filename to a command that takes URIs,
       * so we need to convert it */
      result = xmlStrdup((xmlChar*) unixfilename(temp));
      xmlFree(temp);
    }
  return result;
}
