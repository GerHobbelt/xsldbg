
/***************************************************************************
                          file_cmds.cpp  - user commands to work with files
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


#include <stdio.h>
#include <libxml/tree.h>
#include <libxml/catalog.h>

#include "xsldbg.h"
#include "debugXSL.h"
#include "files.h"
#include "options.h"
#include "utils.h"
#include "xsldbgthread.h"

static char buffer[500];

xmlChar *fixResolveFilePath(xmlChar * name) {
    if (name && (xmlStrstr(name, (const xmlChar*)"file:/") != NULL) && (xmlStrstr(name, (const xmlChar*)"file:///") == NULL)) {
        xmlChar *fixedName = (xmlChar *)xmlMalloc((xmlStrlen(name) + 10) * sizeof(xmlChar));
        xmlStrCpy(fixedName, (const xmlChar*)"file:///");
        int index = 6;
        while (name[index] == '/') {
            index++;
        }
        xmlStrcat(fixedName, &name[index]);
        xmlFree(name);
        name = fixedName;
    }

    return name;
}

static QString fixResolveFilePath(const QString & name) {
    QString result = name;
    if ((name.left(6) == "file:/") && (name.indexOf("file:///") == -1)) {
        result = "file:///";
        int index = 6;
        while (name[index] == '/') {
            index++;
        }
        result += name.mid(index);
    }

    return result;
}

int xslDbgEntities(const xmlChar *arg)
{
    int result = 0;
    bool resolveURIOption = (xmlStrcmp(arg, (const xmlChar*)"-r") == 0);
    if (!resolveURIOption && xmlStrLen(arg)) {
        QString providedArgs(xsldbgText(arg));
        xsldbgGenericErrorFunc(QObject::tr("Unknown argument to entities command: \"%1\"\n").arg(providedArgs));
        return result;
    }

    if (filesEntityList()) {
        int entityIndex;
        entityInfoPtr entInfo;

        if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
            /* notify that we are starting new list of entity names */
            notifyListStart(XSLDBG_MSG_ENTITIY_CHANGED);
            for (entityIndex = 0;
                 entityIndex < arrayListCount(filesEntityList());
                 entityIndex++) {
                entInfo = (entityInfoPtr) arrayListGet(filesEntityList(),
                                                       entityIndex);
                if (entInfo){
                    if (entInfo->ResolvedURI && xmlStrlen(entInfo->ResolvedURI) == 0) {
                      xmlFree(entInfo->ResolvedURI); // no entity resolved, clear value and try again
                      entInfo->ResolvedURI = NULL;
                    }
                    if (entInfo->SystemID)
                        entInfo->SystemID = fixResolveFilePath(entInfo->SystemID);
                    if (!entInfo->ResolvedURI) { // attempt to resolve file entities with their URI
                        if (entInfo->PublicID && xmlStrlen(entInfo->PublicID)
                                && entInfo->SystemID && xmlStrlen(entInfo->SystemID)) {
                            entInfo->ResolvedURI = fixResolveFilePath(xmlCatalogResolvePublic(entInfo->PublicID));
                        } else if (entInfo->SystemID && xmlStrlen(entInfo->SystemID)) {
                            if (!xmlStrnCmp(entInfo->SystemID, "file:/", 6) || !xmlStrnCmp(entInfo->SystemID, "ftp:/", 5) || !xmlStrnCmp(entInfo->SystemID, "http:/", 6)) {
                                entInfo->ResolvedURI = fixResolveFilePath(xmlStrdup(entInfo->SystemID));
                            } else {
                                entInfo->ResolvedURI = fixResolveFilePath(xmlCatalogResolveSystem(entInfo->SystemID));
                            }
                        }
                    }
                    notifyListQueue(entInfo);
                }

            }
            notifyListSend();
            result = 1;
        } else {
            for (entityIndex = 0;
                 entityIndex < arrayListCount(filesEntityList());
                 entityIndex++) {
                entInfo = (entityInfoPtr) arrayListGet(filesEntityList(),
                                                       entityIndex);
                if (entInfo) {
            /* display identifier of an XML entity */
                    QString publicID, systemID;
                    if (entInfo->PublicID)
                        publicID = xsldbgText(entInfo->PublicID);
                    if (entInfo->SystemID)
                        systemID = fixResolveFilePath(xsldbgText(entInfo->SystemID));

                    if (!resolveURIOption) {
                        if (!publicID.isEmpty() && !systemID.isEmpty()) {
                            xsldbgGenericErrorFunc(QObject::tr("Entity PublicID:\"%1\" SystemID:\"%2\"\n").arg(publicID).arg(systemID));
                        } else {
                            xsldbgGenericErrorFunc(QObject::tr("Entity SystemID:\"%1\"\n").arg(systemID));
                        }
                    } else {
                        QString resolveURI;
                        if (entInfo->ResolvedURI && xmlStrlen(entInfo->ResolvedURI) == 0) {
                          xmlFree(entInfo->ResolvedURI); // no entity resolved, clear value and try again
                          entInfo->ResolvedURI = NULL;
                        }
                        if (!entInfo->ResolvedURI) { // attempt to resolve file entities with their URI
                            if (entInfo->PublicID && xmlStrlen(entInfo->PublicID)
                                    && entInfo->SystemID && xmlStrlen(entInfo->SystemID)) {
                                entInfo->ResolvedURI = fixResolveFilePath(xmlCatalogResolvePublic(entInfo->PublicID));
                            } else if (entInfo->SystemID && xmlStrlen(entInfo->SystemID)) {
                                if (!xmlStrnCmp(entInfo->SystemID, "file:/", 6) || !xmlStrnCmp(entInfo->SystemID, "ftp:/", 5) || !xmlStrnCmp(entInfo->SystemID, "http:/", 6)) {
                                    entInfo->ResolvedURI = fixResolveFilePath(xmlStrdup(entInfo->SystemID));
                                } else {
                                    entInfo->ResolvedURI = fixResolveFilePath(xmlCatalogResolveSystem(entInfo->SystemID));
                                }
                            }
                        }
                        if (entInfo->ResolvedURI && xmlStrlen(entInfo->ResolvedURI))
                            resolveURI = xsldbgText(entInfo->ResolvedURI);
                        if (!publicID.isEmpty() && !systemID.isEmpty()) {
                            xsldbgGenericErrorFunc(QObject::tr("Entity PublicID:\"%1\" SystemID:\"%2\" URI:\"%3\"\n").arg(publicID).arg(systemID).arg(resolveURI));
                        } else {
                            xsldbgGenericErrorFunc(QObject::tr("Entity SystemID:\"%1\" URI:\"%3\"\n").arg(systemID).arg(resolveURI));
                        }
                    }
                }
            }
            if (arrayListCount(filesEntityList()) == 0) {
                xsldbgGenericErrorFunc(QObject::tr("No external General Parsed entities present.\n"));
            } else {
        xsldbgGenericErrorFunc(QObject::tr("\tTotal of %1 entity found.").arg(arrayListCount(filesEntityList())) + QString("\n"));
            }

            result = 1;
        }
    }
    return result;
}

int xslDbgSystem(const xmlChar * arg)
{
    int result = 0;
    xmlChar *name;

    if (!arg || (xmlStrlen(arg) == 0)) {
#ifdef WITH_XSLDBG_DEBUG_PROCESS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
#endif
        return result;
    }

    name = fixResolveFilePath(xmlCatalogResolveSystem(arg));
    if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
        if (name) {
            notifyXsldbgApp(XSLDBG_MSG_RESOLVE_CHANGE, name);
            result = 1;
            xmlFree(name);
        } else {
            notifyXsldbgApp(XSLDBG_MSG_RESOLVE_CHANGE, "");
            xsldbgGenericErrorFunc(QObject::tr("SystemID \"%1\" was not found in current catalog.\n").arg(xsldbgText(arg)));
        }
    } else {
        if (name) {
            xsldbgGenericErrorFunc(QObject::tr("SystemID \"%1\" maps to: \"%2\"\n").arg(xsldbgText(arg)).arg(xsldbgText(name)));
            xmlFree(name);
            result = 1;
        } else {
            xsldbgGenericErrorFunc(QObject::tr("SystemID \"%1\" was not found in current catalog.\n").arg(xsldbgText(arg)));
        }
    }

    return result;
}


int xslDbgPublic(const xmlChar * arg)
{
    int result = 0;
    xmlChar *name;

    if (!arg || (xmlStrlen(arg) == 0)) {
#ifdef WITH_XSLDBG_DEBUG_PROCESS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
#endif
        return result;
    }

    name = fixResolveFilePath(xmlCatalogResolvePublic(arg));
    if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
        if (name) {
            notifyXsldbgApp(XSLDBG_MSG_RESOLVE_CHANGE, name);
            result = 1;
            xmlFree(name);
        } else {
            notifyXsldbgApp(XSLDBG_MSG_RESOLVE_CHANGE, "");
        xsldbgGenericErrorFunc(QObject::tr("PublicID \"%1\" was not found in current catalog.\n").arg(xsldbgText(arg)));
        }
    } else {
        if (name) {
            xsldbgGenericErrorFunc(QObject::tr("PublicID \"%1\" maps to: \"%2\"\n").arg(xsldbgText(arg)).arg(xsldbgText(name)));
            xmlFree(name);
            result = 1;
        } else {
        xsldbgGenericErrorFunc(QObject::tr("PublicID \"%1\" was not found in current catalog.\n").arg(xsldbgText(arg)));
        }
        xsltGenericError(xsltGenericErrorContext, "%s", buffer);
    }
    return result;
}


int xslDbgEncoding(xmlChar * arg)
{
    int result = 0;
    xmlChar *opts[2];

    if (!arg) {
#ifdef WITH_XSLDBG_DEBUG_PROCESS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
#endif
        return result;
    }

    if (splitString(arg, 1, opts) == 1) {
        if (filesSetEncoding((char *) opts[0])) {
            optionsSetStringOption(OPTIONS_ENCODING, xsldbgText(opts[0]));
            result = 1;
        }
    } else
        xsldbgGenericErrorFunc(QObject::tr("Error: Missing arguments for the command %1.\n").arg(QString("encoding")));
    return result;
}


int xslDbgShellOutput(const xmlChar *arg)
{
  int result = 0;
  if (arg && (xmlStrLen(arg) > 0)){
     if (!xmlStrnCmp(arg, "file:/", 6)){
      /* convert URI to local file name */
      xmlChar *outputFileName = filesURItoFileName(arg);
      if (outputFileName){
    optionsSetStringOption(OPTIONS_OUTPUT_FILE_NAME, xsldbgText(outputFileName));
    notifyXsldbgApp(XSLDBG_MSG_FILE_CHANGED, 0L);
    xmlFree(outputFileName);
    result = 1;
      }
    } else if (xmlStrEqual(arg, (xmlChar*)"-")) {
      optionsSetStringOption(OPTIONS_OUTPUT_FILE_NAME,
                 NULL);
      notifyXsldbgApp(XSLDBG_MSG_FILE_CHANGED, 0L);
      result = 1;
    } else if (!xmlStrnCmp(arg, "ftp://", 6) || !xmlStrnCmp(arg, "http://", 7)){
    xsldbgGenericErrorFunc(QObject::tr("Error: Invalid arguments for the command %1.\n").arg(QString("output")));
    return 0;
    } else {
      /* assume that we were provided a local file name
       * that may need expanding
       */
      QString expandedName = filesExpandName(xsldbgText(arg));

      // The output file must not be the same as our SOURCE or DATA file
      if (!expandedName.isEmpty() &&
    (optionsGetStringOption(OPTIONS_SOURCE_FILE_NAME) !=  expandedName) &&
    (optionsGetStringOption(OPTIONS_DATA_FILE_NAME) != expandedName) ){
       optionsSetStringOption(OPTIONS_OUTPUT_FILE_NAME, expandedName);
       notifyXsldbgApp(XSLDBG_MSG_FILE_CHANGED, 0L);
       result = 1;
      }else{
       xsldbgGenericErrorFunc(QObject::tr("Error: Invalid arguments for the command %1.\n").arg(QString("output")));
      }
    }
   } else {
    xsldbgGenericErrorFunc(QObject::tr("Error: Missing arguments for the command %1.\n").arg(QString("output")));
  }

  return result;
}

int xslDbgList(xmlChar *arg)
{
    int result = 0;
    int lineStart = xsldbgLineNo();
    int lineEnd = lineStart;
    int defaultLineCount = 10; // how many lines to print by default
    bool autoList = false;
    xmlChar *currentURI = xsldbgUrl();
    static const char *errorPrompt = I18N_NOOP("Failed to list file.");

    if (!currentURI) {
        xsltGenericError(xsltGenericErrorContext, "Error: No XSL/XML file currently loaded\n");
        return result;
    }

    if (arg[0] == '+') {
        autoList = true;
        defaultLineCount = 1;
    }

    if ((xmlStrstr(currentURI, (const xmlChar*)":/") != NULL) && (xmlStrstr(currentURI, (const xmlChar*)"file:/") == NULL)) {
        if (!autoList) {
            xsltGenericError(xsltGenericErrorContext, "Error: Can only list local files, saw uri: %s\n", currentURI);
        }
        return result;
    }

    if (!autoList) {
        xmlChar *parameters = arg;
        xmlChar *opts[2];
        if (arg[0] == '-' && arg[1] == 'l') {
            parameters = &arg[2];
        }

        if (splitString(parameters, 2, opts) == 2) {
            if ((xmlStrlen(opts[0]) == 0) || !sscanf((char *) opts[0], "%d", &lineStart)) {
                xsldbgGenericErrorFunc(QObject::tr("Error: Unable to parse %1 as a line number.\n").arg((char*)opts[0]));
                xsldbgGenericErrorFunc(QString("Error: %1\n").arg(QObject::tr(errorPrompt)));
                return result;
            }
            if ((xmlStrlen(opts[1]) == 0) || !sscanf((char *) opts[1], "%d", &lineEnd)) {
                xsldbgGenericErrorFunc(QObject::tr("Error: Unable to parse %1 as a line number.\n").arg((char*)opts[1]));
                xsldbgGenericErrorFunc(QString("Error: %1\n").arg(QObject::tr(errorPrompt)));
                return result;
            }
        } else if(splitString(parameters, 1, opts) == 1) {
            if ((xmlStrlen(opts[0]) == 0) || !sscanf((char *) opts[0], "%d", &lineStart)) {
                xsldbgGenericErrorFunc(QObject::tr("Error: Unable to parse %1 as a line number.\n").arg((char*)opts[0]));
                xsldbgGenericErrorFunc(QString("Error: %1\n").arg(QObject::tr(errorPrompt)));
                return result;
            }
            lineEnd = -1;
        } else {
            lineEnd = -1;
        }
    }
    if (lineStart < 0) { // if documents is not load properly show first line
        lineStart = 0;
    }


    QStringList lineText = filesDataReadFile(xsldbgText(currentURI));
    if (lineEnd < 0) {
        lineEnd = qMin(lineText.count() - 1, lineStart + defaultLineCount);
    }

    if ((lineStart < 0) || (lineStart > lineText.count())) {
        xsltGenericError(xsltGenericErrorContext, "Error: list command 'lineStart' parameter %d not within range of lines read:%d in %s\n", lineStart, lineText.count(), currentURI);
        return result;
    }

    if (lineEnd > lineText.count()) {
        xsltGenericError(xsltGenericErrorContext, "Error: list command 'lineEnd' parameter %d greater than lines read:%d in %s\n", lineEnd, lineText.count(), currentURI);
        return result;
    }



    int lineNo = lineStart;
    while (lineNo <= lineEnd) {
        xsldbgGenericErrorFunc(QString("%1 %2\n").arg(lineNo).arg(lineText[lineNo]));
        lineNo++;
    }
    result = true;

    xmlFree(currentURI);
    return result;
}
