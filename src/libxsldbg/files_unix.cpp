
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


#include "xsldbg.h"
#include "files.h"
#include "utils.h"
#include "options.h"

static char *tempNames[2] = { NULL, NULL };

int filesPlatformInit(void)
{
    const char *namePrefix = "/tmp/";
    int nameIndex;
    int result = 1;

    /* The "base" names for files files to use */
    const char *names[] = {
        "_xsldbg_tmp1.txt",
        "_xsldbg_tmp2.txt"
    };

    if (getenv("USER")) {
        for (nameIndex = 0; nameIndex < 2; nameIndex++) {
            tempNames[nameIndex] = (char*)
                xmlMalloc(strlen(namePrefix) + strlen(getenv("USER")) +
                          strlen(names[nameIndex]) + 1);
            if (tempNames[nameIndex]) {
                xmlStrCpy(tempNames[nameIndex], namePrefix);
                xmlStrCat(tempNames[nameIndex], getenv("USER"));
                xmlStrCat(tempNames[nameIndex], names[nameIndex]);
            } else {
                xsldbgGenericErrorFunc(i18n("Error: Out of memory.\n"));
                break;
                result = 0;
            }
        }
    } else {
	 xsldbgGenericErrorFunc(i18n("Error: USER environment variable is not set.\n"));
    }
    return result;
}


void filesPlatformFree(void)
{
    int nameIndex;

    for (nameIndex = 0; nameIndex < 2; nameIndex++) {
        if (tempNames[nameIndex])
            xmlFree(tempNames[nameIndex]);
    }
}

const char * filesTempFileName(int fileNumber)
{

    const char *result = NULL;

    if ((fileNumber < 0) || (fileNumber >= int(sizeof(tempNames)/sizeof(*tempNames)))){
#ifdef WITH_XSLDBG_DEBUG_PROCESS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Unable to allocate temporary file %d for xsldbg\n",
                         fileNumber);
#endif
    }else{
        result = tempNames[fileNumber];
    }

    return result;
}


QString filesSearchFileName(FilesSearchFileNameEnum fileType)
{
    QString result;
    int type = fileType;
    int preferHtml = optionsGetIntOption(OPTIONS_PREFER_HTML);
    QString baseDir;
    QString name;
    static const char *searchNames[] = {
        /* Note: File names here are in native format, to be appended to the
         *  help directory name or search results path
         */
        /* First list names when prefer html is false */
        "searchresult.xml",     /* input  */
        "search.xsl",           /* stylesheet to use */
        "searchresult.txt",     /* where to put the result */
        /*Now for the names to use when prefer html is true */
        "searchresult.xml",     /* input  */
        "searchhtml.xsl",       /* stylesheet to use */
        "searchresult.html"     /* where to put the result */
    };

    if (optionsGetStringOption(OPTIONS_DOCS_PATH).isEmpty()
        || filesSearchResultsPath().isEmpty()) {
        xsldbgGenericErrorFunc(i18n("Error: The value of the option docspath or searchresultspath is empty. See help on setoption or options command for more information.\n"));
        return result;
    }


    name = searchNames[(preferHtml * 3) + type];
    switch (type) {
        case FILES_SEARCHINPUT:
            baseDir = filesSearchResultsPath();
            break;

        case FILES_SEARCHXSL:
            baseDir = optionsGetStringOption(OPTIONS_DOCS_PATH);
            break;

        case FILES_SEARCHRESULT:
            baseDir = filesSearchResultsPath();
            break;
    }

    result = baseDir + name;

    return result;
}
