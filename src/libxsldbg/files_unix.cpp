
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
#include <QDir>

#define XSLDBG_NUM_TEMP_FILES 2
static QByteArray tempNames[XSLDBG_NUM_TEMP_FILES];

int filesPlatformInit(void)
{
	int result = 1;

	for (int index=0; index < XSLDBG_NUM_TEMP_FILES; index++)
		tempNames[index] = QFile::encodeName(QString("%1%2_xsldbg_tmp%3.txt").arg(QDir::tempPath()).arg(PATHCHAR).arg(index));
        
    return result;
}


void filesPlatformFree(void)
{
    // Empty
}

QByteArray filesTempFileName(int fileNumber)
{

    QByteArray result;

    if ((fileNumber < 0) || (fileNumber >= XSLDBG_NUM_TEMP_FILES)){
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
