
/***************************************************************************
              utils.cpp  - various utility functions
                             -------------------
    begin                : Mon May 2 2016
    copyright            : (C) 2016 by Keith Isdale
    email                : keithisdale@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "utils.h"
#include "files.h"

#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <QDebug>
int trimString(xmlChar * text)
{
    int result = 0;
    xmlChar *start, *end;

    if (text && xmlStrlen(text)) {
        start = text;
        end = text + xmlStrLen(text) - 1;
        while ((*start == ' ') && (start <= end))
            start++;

        while ((*end == ' ') && (end >= start))
            end--;

        /* copy  to @text */
        while (start <= end) {
            *text = *start;
            text++;
            start++;
        }

        *text = '\0';
        result = 1;
    }
    return result;
}


int splitString(xmlChar * textIn, int maxStrings, xmlChar ** out)
{
    int wordCount = 0;
    int foundQuote = 0;

    if (!textIn || !out)
        return wordCount;


    while ((*textIn != '\0') && (wordCount < maxStrings)) {
        /*skip the first spaces ? */
        while (*textIn == ' ')
            textIn++;

        if (*textIn == '\"') {
            textIn++;
            foundQuote = 1;
        }
        out[wordCount] = textIn;

        /* look for end of word */
        if (foundQuote == 0) {
            while ((*textIn != ' ') && (*textIn != '\0'))
                textIn++;

            if (*textIn != '\0') {
                *textIn = '\0';
                textIn++;
            }

            if (xmlStrLen(out[wordCount]) > 0) {
                wordCount++;
            }
        } else {
            /* look for ending quotation mark */
            while ((*textIn != '\0') && (*textIn != '\"'))
                textIn++;
            if (*textIn == '\0') {
                xsldbgGenericErrorFunc(i18n("Error: Unmatched quotes in input.\n"));
                wordCount = 0;
                break;
            }
            *textIn = '\0';
            textIn++;           /* skip the '"' which is now a '\0' */
            foundQuote = 0;
            wordCount++;
        }

    }

    if (*textIn != '\0')
        wordCount = 0;          /* We have not processed all the text givent to us */
    return wordCount;
}



int lookupName(QString name, const char ** matchList)
{
    int result = -1, nameIndex;

    if (name.isEmpty() || !matchList)
        return result;

    for (nameIndex = 0; matchList[nameIndex]; nameIndex++) {
        if (name == matchList[nameIndex]) {
            result = nameIndex;
            break;
        }
    }

    return result;
}


xmlChar * fullQName(const xmlChar* nameURI, const xmlChar * name)
{
  xmlChar *result = NULL;
  if (!nameURI && !name)
    result =  xmlStrdup((xmlChar*)"");
  else{
    if (nameURI == NULL){
      result = xmlStrdup(name);
    }else{
      result = (xmlChar*) xmlMalloc(sizeof(char) * (
              xmlStrLen(name) +
              xmlStrLen(nameURI) + 3));
      if (result)
    sprintf((char*)result, "%s:%s",  (char*)nameURI, (char*)name);
    }
  }
  return result;
}

QString filesExpandName(const QString fileName, bool addFilePrefix)
{
    QString result;

    if (!fileName.isEmpty()) {
        if ((fileName[0] == '~') && getenv("HOME")) {
            if (addFilePrefix)
                result = "file://";
            result += getenv("HOME");
            result += fileName.mid(1);
        } else if (addFilePrefix  && fileName[0] == '/'){
            /* ensure that URI has three leading slashes */
            int sourceIndex = 0;
            result = "file:///";
            while ((sourceIndex < fileName.count()) && fileName[sourceIndex] == '/')
                sourceIndex++;
            result += fileName.mid(sourceIndex);
        }else if (addFilePrefix) {
            result = fixResolveFilePath(fileName);
        }else{
            /* return a copy only */
            result = fileName;
        }
    }

    return result;
}


QString fixLocalPaths(const QString & file)
{
    QString result = file;

    if (file.left(6) == "file:/")
        result = filesExpandName(file, true);

    return result;
}
