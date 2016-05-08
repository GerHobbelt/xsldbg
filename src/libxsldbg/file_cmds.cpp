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

int xslDbgEntities(void)
{
    int result = 0;

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
                if (entInfo)
                    notifyListQueue(entInfo);

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
                    xsldbgGenericErrorFunc(QObject::tr("Entity %1 ").arg(xsldbgText(entInfo->SystemID)));
                    if (entInfo->PublicID)
			xsldbgGenericErrorFunc(xsldbgText(entInfo->PublicID));
		    xsldbgGenericErrorFunc("\n");
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

    name = xmlCatalogResolveSystem(arg);
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

    name = xmlCatalogResolvePublic(arg);
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

