
/***************************************************************************
                          file_cmds.c  -  define file command related functions
                             -------------------
    begin                : Sat Jan 19 2002
    copyright            : (C) 2002 by Keith Isdale
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

#include <stdio.h>
#include <libxml/tree.h>
#include <libxml/catalog.h>

#include "xsldbg.h"
#include "debugXSL.h"
#include "files.h"
#include "options.h"
#include "xsldbgthread.h"

static char buffer[500];

/**
 * xslDbgEntities:
 * 
 * Print list of entites found 
 *
 * Returns 1 on sucess,
 *         0 otherwise
 */
int
xslDbgEntities(void)
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
            result++;
        } else {
            for (entityIndex = 0;
                 entityIndex < arrayListCount(filesEntityList());
                 entityIndex++) {
                entInfo = (entityInfoPtr) arrayListGet(filesEntityList(),
                                                       entityIndex);
                if (entInfo) {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Entity %s ", entInfo->SystemID);
                    if (entInfo->PublicID)
                        xsltGenericError(xsltGenericErrorContext,
                                         "%s", entInfo->PublicID);

                    xsltGenericError(xsltGenericErrorContext, "\n");
                }
            }
            if (arrayListCount(filesEntityList()) == 0) {
                xsltGenericError(xsltGenericErrorContext,
                                 "No external General Parsed entities"
                                 " present\n\n");
            } else {
                xsltGenericError(xsltGenericErrorContext,
                                 "\n\tTotal of %d entities found\n",
                                 arrayListCount(filesEntityList()));
            }

            result++;
        }
    }
    return result;
}


/**
 * xslDbgSystem:
 * @arg : Is valid
 * 
 * Print what a system file @arg maps to via the current xml catalog
 *
 * Returns 1 on sucess,
 *         0 otherwise
 */
int
xslDbgSystem(const xmlChar * arg)
{
    int result = 0;
    xmlChar *name;

    if (!arg)
        return result;

    name = xmlCatalogResolveSystem(arg);
    if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
        if (name) {
            notifyXsldbgApp(XSLDBG_MSG_RESOLVE_CHANGE, name);
            result++;
            xmlFree(name);
        } else {
            snprintf(buffer, sizeof(buffer),
                     "SystemID %s was not found "
                     " in xml catalog\n\n", arg);
            notifyXsldbgApp(XSLDBG_MSG_RESOLVE_CHANGE, "");
            xsltGenericError(xsltGenericErrorContext, buffer);
        }
    } else {
        if (name) {
            snprintf(buffer, sizeof(buffer),
                     "SystemID %s maps to : %s\n\n", arg, name);
            xmlFree(name);
            result++;
        } else {
            snprintf(buffer, sizeof(buffer),
                     "SystemID %s was not found "
                     " in xml catalog\n\n", arg);
        }
        xsltGenericError(xsltGenericErrorContext, buffer);
    }

    return result;
}


/**
 * xslDbgPublic:
 * @arg : Is valid
 * 
 * Print what a public ID @arg maps to via the current xml catalog
 *
 * Returns 1 on sucess,
 *         0 otherwise
 */
int
xslDbgPublic(const xmlChar * arg)
{
    int result = 0;
    xmlChar *name;

    if (!arg)
        return result;

    name = xmlCatalogResolvePublic(arg);
    if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
        if (name) {
            notifyXsldbgApp(XSLDBG_MSG_RESOLVE_CHANGE, name);
            result++;
            xmlFree(name);
        } else {
            snprintf(buffer, sizeof(buffer),
                     "PublicID %s was not found "
                     " in xml catalog\n\n", arg);
            notifyXsldbgApp(XSLDBG_MSG_RESOLVE_CHANGE, "");
            xsltGenericError(xsltGenericErrorContext, buffer);
        }
    } else {
        if (name) {
            snprintf(buffer, sizeof(buffer),
                     "PublicID  %s maps to : %s\n\n", arg, name);
            xmlFree(name);
            result++;
        } else {
            snprintf(buffer, sizeof(buffer),
                     "PublicID %s was not found "
                     " in xml catalog\n\n", arg);
        }
        xsltGenericError(xsltGenericErrorContext, buffer);
    }
    return result;
}


/**
 * xslDbgEncoding:
 * @arg: Is valid encoding supported by libxml2
 *
 * Set current encoding to use for output to standard output
 *
 * Returns 1 on sucess,
 *         0 otherwise
 */
int
xslDbgEncoding(xmlChar * arg)
{
    int result = 0;
    xmlChar *opts[2];

    if (splitString(arg, 1, opts) == 1) {
        if (filesSetEncoding((char*)opts[0])) {
            setStringOption(OPTIONS_ENCODING, opts[0]);
            result++;
        }
    } else
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Expected one argument to encoding command\n");
    return result;
}
