
/***************************************************************************
                          files_win32.c  -  file functions win32 platform
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

static const char *tempNames[] = {
    "__xsldbg_tmp_file1_txt",
    "__xsldbg_tmp_file2_txt"
};

  /**
   * filesPlatformInit:
   *
   * Intialize the platform specific files module
   *
   *  This is a platform specific interface
   *
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
    xmlChar *result = NULL;
    const char pathSepString[2] = { PATHCHAR, '\0' };

    if (fileName) {
        if ((fileName[0] == '~') && getenv("HOME")) {
            result =
                (xmlChar *) xmlMalloc(strlen(fileName) +
                                      strlen(getenv("HOME")));
            if (result) {
                xmlStrCpy(result, getenv("HOME"));
                xmlStrCat(result, pathSepString);
                xmlStrCat(result, &fileName[1]);
            } else {
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: Out of memory\n");
            }
        } else {
            /* don't make any changes, return a copy only */
            result = xmlStrdup(fileName);
        }
    }
    return result;
}
