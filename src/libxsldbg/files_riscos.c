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

static const char *tempNames[] = {
    "__xsldbg_tmp_file1_txt",
    "__xsldbg_tmp_file2_txt"
};


  /**
   * filesTempFileName:
   * @ fileNumber : Nnumber of temp file required
   *
   * Return the name of tempfile. For each call to this function
   *     with the same @fileNumber the same file name will be returned
   *     File number : 0 is used by cat command
   *     File number : 1 is used by profiling output  
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
