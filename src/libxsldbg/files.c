
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

#include <stdio.h>
#include <libxml/entities.h>
#include <libxml/tree.h>
#include <libxml/catalog.h>
#include <libxml/parserInternals.h>
#include <libxml/encoding.h>    /* needed by filesTranslate, filesEncoding functions */

#include "xsldbg.h"
#include "debugXSL.h"
#include "files.h"
#include "options.h"
#include "xsldbgthread.h"



/* top xml document */
static xmlDocPtr topDocument;

/* temporary xml document */
static xmlDocPtr tempDocument;

/* used as a scratch pad for temporary results*/
static xmlChar buffer[DEBUG_BUFFER_SIZE];

/* top stylsheet */
static xsltStylesheetPtr topStylesheet;

/* what is the base path for top stylesheet */
static xmlChar *stylePathName = NULL;

/* what is the path for current working directory*/
static xmlChar *workingDirPath = NULL;

static ArrayListPtr entityNameList = NULL;

/* Current encoding to use for standard output*/
static xmlCharEncodingHandlerPtr stdoutEncoding = NULL;

/* input and output buffers for encoding*/
static xmlBufferPtr encodeInBuff = NULL;
static xmlBufferPtr encodeOutBuff = NULL;

/* -----------------------------------------
   Private function declarations for files.c
 -------------------------------------------*/

/**
 * guessStylesheetHelper:
 * @payload: valid xsltStylesheetPtr
 * @data: valid searchInfoPtr of type SEARCH_NODE
 * @name: not used
 *
 * Try to guess what the complete file/URI is. If successful the search
 *   info will be set to found and the search data will contain the
 *   file name found. We are given our payload via walkStylesheets
 */
static void guessStylesheetHelper(void *payload, void *data,
                                  xmlChar * name ATTRIBUTE_UNUSED);


/**
 * guessStylesheetHelper2:
 * @payload: valid xmlNodePtr of the included stylesheet 
 * @data: valid searchInfoPtr of type SEARCH_NODE
 * @name: not used
 *
 * Try to guess what the complete file/URI is. If successful the search
 *   info will be set to found and the search data will contain the
 *   file name found. We are given our payload via walkIncludes
 */
static void guessStylesheetHelper2(void *payload, void *data,
                                   xmlChar * name ATTRIBUTE_UNUSED);


entityInfoPtr filesNewEntityInfo(const xmlChar * SystemID,
                                 const xmlChar * PublicID);

void filesFreeEntityInfo(entityInfoPtr info);

void filesAddEntityName(const xmlChar * SystemID,
                        const xmlChar * PublicID);


/* ------------------------------------- 
    End private functions
---------------------------------------*/


FILE *terminalIO;

/* No longer needed
   static FILE *oldStdin, *oldStdout, *oldStderr;*/

static char *ttyName;           /* what is the name of the default terminal */
static char *termName = NULL;   /* what is the name of terminal we are redirected to */


/**
 * redirectToTerminal:
 * @device: terminal to redirect i/o to , will not work under win32
 *
 * Open communications to the terminal device @device
 *
 * Returns 1 if sucessful
 *         0 otherwise
 */
int
openTerminal(xmlChar * device)
{
    int result = 0;

    if (!device)                /* Failed; there's no device */
        return result;
#ifdef __riscos
    /*
     * On RISC OS, you get one terminal - the screen.
     * we assume that the parameter is meant to be an output device as
     * per normal - we can use vdu:, rawvdu: or :tt, or a filename for
     * normal VDU output, VDU output without newline expansion,
     * C terminal output with control code escaping, or a raw file
     * respectively.
     * The name passed is expected to be in native file format - no
     * URI escaping here.
     * One assumes that you might use a socket or a pipe here.
     */

    if (terminalIO)
        fclose(terminalIO);

    if (termName) {
        xmlFree(termName);
        termName = NULL;
    }


    if (device[0] == '\0') {
        /* look like we are supposed to close the terminal */
        selectNormalIO();       /* shouldn't be needed but just in case */
    } else {
        terminalIO = fopen((char *) device, "w");
        if (terminalIO != NULL) {
            termName = xmlMemStrdup((char *) device);
            result++;
        } else {
            xsltGenericError(xsltGenericErrorContext,
                             "Unable to open terminal %s\n", device);
        }
    }
#else

#ifdef HAVE_UNISTD_H            /* fix me for WinNT */

    if ((device[0] >= '0') && (device[0] <= '9')) {
        /*set the tty level  */
        switch (device[0]) {

            case '1':
                /* redirect only some output to terminal */
                if (!terminalIO && termName) {
                    terminalIO = fopen((char *) device, "w");
                    if (terminalIO) {
                        result++;
                    } else {
                        xsltGenericError(xsltGenericErrorContext,
                                         "Unable to open terminal %s",
                                         device);
                        termName = NULL;
                    }
                }
                break;


            case '2':
                /* redirect everything to the terminal */
                if (termName && terminalIO) {
                    /* we have previously sucessfully opened the terminal so just
                     * go ahead a redirect I/O */
                    result = freopen(termName, "r", stdin) != NULL;
                    result = result
                        && (freopen(termName, "w", stdout) != NULL);
                    result = result
                        && (freopen(termName, "w", stderr) != NULL);
                    if (!result) {
                        xsltGenericError(xsltGenericErrorContext,
                                         "Unable to redirect to terminal %s\n",
                                         termName);
                    }
                }
                break;

            default:
                /* look like we are supposed to close the terminal */
                if ((terminalIO != NULL) && (ttyName != NULL)) {
                    fclose(terminalIO);
                    freopen(ttyName, "r", stdin);
                    freopen(ttyName, "w", stdout);
                    freopen(ttyName, "w", stderr);
                    terminalIO = NULL;
                    result++;
                }
                break;
        }

    } else {

        if (terminalIO != NULL)
            fclose(terminalIO);

        if (termName) {
            xmlFree(termName);
            termName = NULL;
        }

        /* just open the terminal the user will need to provide a
         * tty level by invoking tty command again with a value of 0 - 9
         */
        terminalIO = fopen((char *) device, "w");
        if (terminalIO != NULL) {
            termName = xmlMemStrdup((char *) device);
            /*
             * dup2(fileno(terminalIO), fileno(stdin));
             * dup2(fileno(terminalIO), fileno(stderr));
             * dup2(fileno(terminalIO), fileno(stdout));
             */
            result++;
        } else {
            xsltGenericError(xsltGenericErrorContext,
                             "Unable to open terminal %s", device);
        }
    }

#else
    xsltGenericError(xsltGenericErrorContext,
                     "Terminals are no supported by this operating system\n");
#endif

#endif
    return result;
}


/**
 * selectTerminalIO:
 *
 * Returns 1 if able to use prevously opened terminal 
 *         0 otherwise
*/
int
selectTerminalIO(void)
{
    /* No longer used but must remain for the moment
     * int result = 0;
     * 
     * if (termName) {
     * freopen(termName, "w", stdout);
     * freopen(termName, "w", stderr);
     * freopen(termName, "r", stdin);
     * result++;
     * } else
     * result++;
     * 
     */
    return 1;
}


/** 
 * selectNormalIO:
 * 
 * Returns 1 if able to select orginal stdin, stdout, stderr
 *         0 otherwise
*/
int
selectNormalIO(void)
{
    /* No longer used but must remain for the moment
     * int result = 0;
     * 
     * #ifdef UNISTD_H
     * if (ttyName) {
     * freopen(ttyName, "w", stdout);
     * freopen(ttyName, "w", stderr);
     * freopen(ttyName, "r", stdin);
     * }
     * #endif
     * result++;
     */
    return 1;
}


/**
 * guessStylesheetHelper:
 * @payload: valid xsltStylesheetPtr
 * @data: valid searchInfoPtr of type SEARCH_NODE
 * @name: not used
 *
 * Try to guess what the complete file/URI is. If successful the search
 *   info will be set to found and the search data will contain the
 *   file name found. We are given our payload via walkStylesheets
 */
void
guessStylesheetHelper(void *payload, void *data,
                      xmlChar * name ATTRIBUTE_UNUSED)
{
    xsltStylesheetPtr style = (xsltStylesheetPtr) payload;
    searchInfoPtr searchCriteria = (searchInfoPtr) data;
    nodeSearchDataPtr searchData = NULL;

    if (!style || !style->doc || !searchCriteria || !searchCriteria->data
        || (searchCriteria->type != SEARCH_NODE))
        return;

    searchData = (nodeSearchDataPtr) searchCriteria->data;
    if (searchData->nameInput && (searchData->absoluteNameMatch == NULL)) {
        /* at this point we know that we have not made an absolute match 
         * but we may have made a relative match */
        if (xmlStrCmp(style->doc->URL, searchData->nameInput) == 0) {
            /* absolute path match great! */
            searchData->absoluteNameMatch =
                (xmlChar *) xmlMemStrdup((char *) style->doc->URL);
            searchData->node = (xmlNodePtr) style->doc;
            searchCriteria->found = 1;
            return;
        }


        /* try to guess we assume that the files are unique */
        xmlStrCpy(buffer, "__#!__");
        /* try relative to top stylesheet directory */
        if (stylePath()) {
            xmlStrCpy(buffer, stylePath());
            xmlStrCat(buffer, searchData->nameInput);
        }
        if (xmlStrCmp(style->doc->URL, buffer) == 0) {
            /* guessed right! */
            searchData->guessedNameMatch =
                (xmlChar *) xmlMemStrdup((char *) buffer);
            searchData->node = (xmlNodePtr) style->doc;
            searchCriteria->found = 1;
            return;
        }

        if (workingPath()) {
            /* try relative to working directory */
            xmlStrCpy(buffer, workingPath());
            xmlStrCat(buffer, searchData->nameInput);
        }
        if (xmlStrCmp(style->doc->URL, buffer) == 0) {
            /* guessed right! */
            searchData->guessedNameMatch =
                (xmlChar *) xmlMemStrdup((char *) buffer);
            searchData->node = (xmlNodePtr) style->doc;
            searchCriteria->found = 1;
            return;
        }

        if (xmlStrChr(searchData->nameInput, PATHCHAR) == NULL) {
            /* Last try, nameInput contains only a file name, and no path specifiers
             * Strip of the file name at end of the stylesheet doc URL */

            /* what is used to separate directories or the URL */
            char separatorChar;

            /* where did the directory separator occur */
            char *lastSlash;

            /* if the stylesheets URL seems to be a URI then use the 
             * URI separator character. Otherwise use the default directory
             * separator character for the operating system */
            if (xmlStrChr(style->doc->URL, URISEPARATORCHAR))
                separatorChar = URISEPARATORCHAR;
            else
                separatorChar = PATHCHAR;

            lastSlash = xmlStrrChr(style->doc->URL, separatorChar);

            if (lastSlash) {
                lastSlash++;    /* skip the slash */
                if (xmlStrCmp(lastSlash, searchData->nameInput) == 0) {
                    /* guessed right! */
                    searchData->guessedNameMatch =
                        (xmlChar *) xmlMemStrdup((char *) style->doc->URL);
                    searchData->node = (xmlNodePtr) style->doc;
                    searchCriteria->found = 1;
                }
            }
        }
    }
}


/**
 * guessStylesheetHelper2:
 * @payload: valid xmlNodePtr of the included stylesheet 
 * @data: valid searchInfoPtr of type SEARCH_NODE
 * @name: not used
 *
 * Try to guess what the complete file/URI is. If successful the search
 *   info will be set to found and the search data will contain the
 *   file name found. We are given our payload via walkIncludes
 */
void
guessStylesheetHelper2(void *payload, void *data,
                       xmlChar * name ATTRIBUTE_UNUSED)
{
    xmlNodePtr node = (xmlNodePtr) payload;
    searchInfoPtr searchCriteria = (searchInfoPtr) data;
    nodeSearchDataPtr searchData = NULL;

    if (!node || !node->doc || !searchCriteria || !searchCriteria->data ||
        (searchCriteria->type != SEARCH_NODE))
        return;

    searchData = (nodeSearchDataPtr) searchCriteria->data;
    if (searchData->nameInput && (searchData->absoluteNameMatch == NULL)) {
        /* at this point we know that we have not made an absolute match 
         * but we may have made a relative match */
        if (xmlStrCmp(node->doc->URL, searchData->nameInput) == 0) {
            /* absolute path match great! */
            searchData->absoluteNameMatch =
                (xmlChar *) xmlMemStrdup((char *) node->doc->URL);
            searchData->node = node;
            searchCriteria->found = 1;
            return;
        }


        /* try to guess we assume that the files are unique */
        xmlStrCpy(buffer, "__#!__");
        /* try relative to top stylesheet directory */
        if (stylePath()) {
            xmlStrCpy(buffer, stylePath());
            xmlStrCat(buffer, searchData->nameInput);
        }
        if (xmlStrCmp(node->doc->URL, buffer) == 0) {
            /* guessed right! */
            searchData->guessedNameMatch =
                (xmlChar *) xmlMemStrdup((char *) buffer);
            searchData->node = node;
            searchCriteria->found = 1;
            return;
        }

        if (workingPath()) {
            /* try relative to working directory */
            xmlStrCpy(buffer, workingPath());
            xmlStrCat(buffer, searchData->nameInput);
        }
        if (xmlStrCmp(node->doc->URL, buffer) == 0) {
            /* guessed right! */
            searchData->guessedNameMatch =
                (xmlChar *) xmlMemStrdup((char *) buffer);
            searchData->node = node;
            searchCriteria->found = 1;
            return;
        }

        if (xmlStrChr(searchData->nameInput, PATHCHAR) == NULL) {
            /* Last try, nameInput contains only a file name, and no path specifiers
             * Strip of the file name at end of the stylesheet doc URL */

            /* what is used to separate directories or the URL */
            char separatorChar;

            /* where did the directory separator occur */
            char *lastSlash;

            /* if the stylesheets URL seems to be a URI then use the 
             * URI separator character. Otherwise use the default directory
             * separator character for the operating system */
            if (xmlStrChr(node->doc->URL, URISEPARATORCHAR))
                separatorChar = URISEPARATORCHAR;
            else
                separatorChar = PATHCHAR;

            lastSlash = xmlStrrChr(node->doc->URL, separatorChar);

            if (lastSlash) {
                lastSlash++;    /* skip the slash */
                if (xmlStrCmp(lastSlash, searchData->nameInput) == 0) {
                    /* guessed right! */
                    searchData->guessedNameMatch =
                        (xmlChar *) xmlMemStrdup((char *) node->doc->URL);
                    searchData->node = node;
                    searchCriteria->found = 1;
                }
            }
        }
    }
}

/**
 * guessStylesheetName:
 * @searchInf: Is valid
 *
 * Try to find a matching stylesheet name
 * Sets the values in @searchinf depending on outcome of search
 */
void
guessStylesheetName(searchInfoPtr searchCriteria)
{
    nodeSearchDataPtr searchData;

    if (!searchCriteria)
        return;

    searchData = (nodeSearchDataPtr) searchCriteria->data;
    if (searchData->nameInput == NULL)
        return;                 /* must supply name of file to look for */

    walkStylesheets((xmlHashScanner) guessStylesheetHelper,
                    searchCriteria, getStylesheet());
    if (!searchCriteria->found) {
        /* try looking in the included stylesheets */
        walkIncludes((xmlHashScanner) guessStylesheetHelper2,
                     searchCriteria, getStylesheet());
    }
}


/**
 * stylePath:
 *
 * Return The base path for the top stylesheet ie
 *        ie URL minus the actual file name
 *
 * Returns The base path for the top stylesheet ie
 *        ie URL minus the actual file name
 */
xmlChar *
stylePath(void)
{
    return stylePathName;
}


/**
 * workingPath:
 * 
 * Return the working directory as set by changeDir function
 *
 * Returns The working directory as set by changeDir function
 */
xmlChar *
workingPath(void)
{
    return workingDirPath;
}


/**
 * changeDir:
 * @path: The path to adopt as new working directory
 *
 * Change working directory to path 
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
changeDir(const xmlChar * path)
{
    int result = 0;
    const xmlChar endString[2] = { PATHCHAR, '\0' };


    if (path) {

#ifndef __riscos                /* RISC OS has no concept of 'home' directory */
        /* replace ~ with home path */
        if ((*path == '~') && getenv("HOME")) {
            xmlStrCpy(buffer, getenv("HOME"));
            xmlStrCat(buffer, path + 1);
            /* must have path char at end of path name */
            xmlStrCat(buffer, endString);
        } else
            xmlStrCpy(buffer, path);
#else
        xmlStrCpy(buffer, path);
#endif
        /* must have path char at end of path name */
        xmlStrCat(buffer, endString);

        if (chdir((char *) buffer) == 0) {
            if (workingDirPath)
                xmlFree(workingDirPath);
            workingDirPath = (xmlChar *) xmlMemStrdup((char *) buffer);
            result++;
        }
        if (!result)
            xsltGenericError(xsltGenericErrorContext,
                             "Unable to change to directory %s\n", path);
        else
            xsltGenericError(xsltGenericErrorContext,
                             "Change to directory %s\n", path);
    } else
        xsltGenericError(xsltGenericErrorContext,
                         "Null Input to changeDir %s %d\n", __FILE__,
                         __LINE__);
    return result;
}


/**
 * loadXmlFile:
 * @path: xml file to load
 * @fileType: A valid FileTypeEnum 
 * 
 * Load specified file type, freeing any memory previously used 
 *
 * Returns 1 on success,
 *         0 otherwise 
 */
int
loadXmlFile(const xmlChar * path, FileTypeEnum fileType)
{
    int result = 0;

    if (!freeXmlFile(fileType))
        return result;

    switch (fileType) {
        case FILES_XMLFILE_TYPE:
            if (path && xmlStrLen(path)) {
                if (isOptionEnabled(OPTIONS_SHELL)) {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Setting xml data file name to %s\n",
                                     path);
                }
                setStringOption(OPTIONS_DATA_FILE_NAME, path);
            }
            topDocument = loadXmlData();
            if (topDocument)
                result++;
            break;

        case FILES_SOURCEFILE_TYPE:
            if (path && xmlStrLen(path)) {
                if (isOptionEnabled(OPTIONS_SHELL)) {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Setting stylesheet file name to %s\n",
                                     path);
                }
                setStringOption(OPTIONS_SOURCE_FILE_NAME, path);
            }
            topStylesheet = loadStylesheet();
            if (topStylesheet && topStylesheet->doc) {
                /* look for last slash (or baskslash) of URL */
                char *lastSlash = xmlStrrChr(topStylesheet->doc->URL,
                                             PATHCHAR);
                const char *docUrl =
                    (const char *) topStylesheet->doc->URL;

                result++;
                if (docUrl && lastSlash) {
                    stylePathName = (xmlChar *) xmlMemStrdup(docUrl);
                    stylePathName[lastSlash - docUrl + 1] = '\0';
                    if (isOptionEnabled(OPTIONS_SHELL)) {
                        xsltGenericError(xsltGenericErrorContext,
                                         "Setting stylesheet base path to %s\n",
                                         stylePathName);
                    }
                } else {
                    const char cwd[4] = { '.', PATHCHAR, '\0' };

                    /* ie for *nix this becomes "./" */
                    stylePathName = xmlStrdup(BAD_CAST cwd);
                }

                /* try to find encoding for this stylesheet */
                if (isOptionEnabled(OPTIONS_AUTOENCODE))
                    filesSetEncoding((char*)topStylesheet->encoding);
            }
            break;

        case FILES_TEMPORARYFILE_TYPE:
            if (!path || !xmlStrLen(path)) {
                xsltGenericError(xsltGenericErrorContext,
                                 "Missing file name\n");
                break;
            }
            topDocument = loadXmlTemporary(path);
            if (tempDocument)
                result++;
            break;
    }
    return result;
}


/**
 * freeXmlFile:
 * @fileType: A valid FileTypeEnum 
 * 
 * Free memory associated with the xml file 
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
freeXmlFile(FileTypeEnum fileType)
{
    int result = 0, type = fileType;

    switch (type) {
        case FILES_XMLFILE_TYPE:
            if (topDocument)
                xmlFreeDoc(topDocument);
            topDocument = NULL;
            result++;
            break;

        case FILES_SOURCEFILE_TYPE:
            if (topStylesheet)
                xsltFreeStylesheet(topStylesheet);
            if (stylePathName)
                xmlFree(stylePathName);
            stylePathName = NULL;
            topStylesheet = NULL;
            result++;
            break;

        case FILES_TEMPORARYFILE_TYPE:
            if (tempDocument)
                xmlFreeDoc(tempDocument);
            tempDocument = NULL;
            result++;
            break;
    }
    return result;
}


/**
 * getStylesheet:
 *
 * Return The topmost stylesheet non-null on success,
 *         NULL otherwise
 *
 * Returns The topmost stylesheet non-null on success,
 *         NULL otherwise
 */
xsltStylesheetPtr
getStylesheet(void)
{
    return topStylesheet;
}


/**
 * getTemporaryDoc:
 *
 * Return The current "temporary" document
 *
 * Returns The current "temporary" document
 */
xmlDocPtr
getTemporaryDoc(void)
{
    return tempDocument;
}


/**
 * getMainDoc:
 *
 * Return The main docment
 *
 * Returns The main docment
 */
xmlDocPtr
getMainDoc(void)
{
    return topDocument;
}


/**
 * filesReloaded:
 * @reloaded: if = -1 then ignore @reloaded
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

    terminalIO = NULL;
#ifdef __riscos
    ttyName = ":tt";            /* Default tty */
#endif
#ifdef HAVE_UNISTD
    ttyName = ttyname(fileno(stdin));
    /* save out io for when/if we send debugging to a terminal */
    oldStdin = stdin;
    oldStdout = stdout;
    oldStderr = stderr;
#endif
    topDocument = NULL;
    tempDocument = NULL;
    topStylesheet = NULL;
    entityNameList = arrayListNew(4, (freeItemFunc) filesFreeEntityInfo);
#ifdef  HAVE_INCLUDE_FIX
    xmlSetEntityReferenceFunc(filesEntityRef);
#endif

    /* setup the encoding */
    encodeInBuff = xmlBufferCreate();
    encodeOutBuff = xmlBufferCreate();
    if (entityNameList && encodeInBuff && encodeOutBuff)
        result++;
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

    if (terminalIO) {
        fclose(terminalIO);
        terminalIO = NULL;
    }
    if (termName) {
        xmlFree(termName);
        termName = NULL;
    }

    result = freeXmlFile(FILES_SOURCEFILE_TYPE);
    if (result)
        result = freeXmlFile(FILES_XMLFILE_TYPE);
    if (result)
        result = freeXmlFile(FILES_TEMPORARYFILE_TYPE);
    if (!result)
        xsltGenericError(xsltGenericErrorContext,
                         "Unable to free memory used by xml/xsl files\n");
    if (stylePathName) {
        xmlFree(stylePathName);
        stylePathName = NULL;
    }

    if (workingDirPath) {
        xmlFree(workingDirPath);
        workingDirPath = NULL;
    }

    if (entityNameList) {
        arrayListFree(entityNameList);
        entityNameList = NULL;
    }

    /* Free memory used by encoding related structures */
    if (encodeInBuff)
        xmlBufferFree(encodeInBuff);

    if (encodeOutBuff)
        xmlBufferFree(encodeOutBuff);

    /* close current encoding */
    filesSetEncoding(NULL);
}


/**
 * isSourceFile:
 * @fileName : is valid
 * 
 * Returns true if @name has the ".xsl" externsion
 */
int
isSourceFile(xmlChar * fileName)
{
    return strstr((char *) fileName, ".xsl") ||
        strstr((char *) fileName, ".Xsl") ||
        strstr((char *) fileName, ".XSL");
}



entityInfoPtr
filesNewEntityInfo(const xmlChar * SystemID, const xmlChar * PublicID)
{

    entityInfoPtr result = (entityInfoPtr) xmlMalloc(sizeof(entityInfo));

    if (result) {
        if (SystemID)
            result->SystemID = xmlStrdup(SystemID);
        else
            result->SystemID = xmlStrdup(BAD_CAST "");

        if (PublicID)
            result->PublicID = xmlStrdup(PublicID);
        else
            result->PublicID = xmlStrdup(BAD_CAST "");
    }
    return result;
}

void
filesFreeEntityInfo(entityInfoPtr info)
{
    if (!info)
        return;

    if (info->SystemID)
        xmlFree(info->SystemID);

    if (info->PublicID)
        xmlFree(info->PublicID);
}

/**
 * filesAddEntityName:
 * @name : is valid
 *
 * Add name to entity name list of know external entities if 
 *  it doesn't already exist in list
 */
void
filesAddEntityName(const xmlChar * SystemID, const xmlChar * PublicID)
{
    int entityIndex = 0;
    entityInfoPtr tempItem;

    if (!SystemID || !filesEntityList())
        return;

    for (entityIndex = 0;
         entityIndex < arrayListCount(filesEntityList()); entityIndex++) {
        tempItem = (entityInfoPtr) arrayListGet(filesEntityList(),
                                                entityIndex);
        if (tempItem && xmlStrEqual(SystemID, tempItem->SystemID)) {
            /* name aready exits so don't add it */
            return;
        }

    }

    tempItem = filesNewEntityInfo(SystemID, PublicID);
    arrayListAdd(filesEntityList(), tempItem);
}

/**
 * filesEntityRef :
 * @uri : Is valid
 * @firstNode : Is valid
 * @lastNode : Is Valid
 *
 * Fixes the nodes from firstNode to lastNode so that debugging can occur
   */
void
filesEntityRef(xmlEntityPtr ent, xmlNodePtr firstNode, xmlNodePtr lastNode)
{

    if (firstNode && firstNode->next &&
        (ent->etype == XML_EXTERNAL_GENERAL_PARSED_ENTITY)) {

        if (!firstNode)
            return;

        /* find the first XML_ELEMENT_NODE */
        while (firstNode->next && (firstNode->type != XML_ELEMENT_NODE))
            firstNode = firstNode->next;

        if (lastNode == NULL) {
            if (ent->SystemID) {
                if (ent->ExternalID)
                    filesAddEntityName(ent->SystemID, ent->ExternalID);
                else
                    filesAddEntityName(ent->URI, BAD_CAST "");
                filesSetBaseUri(firstNode, ent->URI);
            }
        } else {
            if (ent->SystemID) {
                xmlNodePtr node = firstNode;

                if (ent->ExternalID)
                    filesAddEntityName(ent->SystemID, ent->ExternalID);
                else
                    filesAddEntityName(ent->URI, BAD_CAST "");
                while (node) {
                    filesSetBaseUri(node, ent->URI);
                    node = node->next;
                }
            }
        }
    }

}



 /**
   * filesSetBaseUri:
   * @node : Is valid and has a doc parent
   * @uri : Is Valid
   * 
   * Set the base uri for this node. Function is used when xml file
   *    has external entities in its DTD
   * 
   * Returns 1 if successful,
   *        0 otherwise
   */

int
filesSetBaseUri(xmlNodePtr node, const xmlChar * uri)
{
    int result = 0;

    if (!node || !node->doc || !uri)
        return result;
    else {
        /*
         * xmlNsPtr xsldbgNs  = xmlSearchNs(node->doc, node, BAD_CAST "xsldbg");
         * if (!xsldbgNs)
         * xsldbgNs = xmlNewNs(node, XSLDBG_XML_NAMESPACE, BAD_CAST "xsldbg");      
         * if (xsldbgNs){   
         * xmlSetNsProp(node, xsldbgNs, BAD_CAST "uri", uri);  
         * result++;
         * }
         */
        xmlNewProp(node, BAD_CAST "xsldbg:uri", uri);
    }
    return result;
}


  /**
   * filesGetBaseUri:
   * @node : Is valid and has a doc parent
   * 
   * Get a copy of the base uri for this node. Function is most usefull 
   *  used when xml file  has external entities in its DTD
   * 
   * Returns the a copy of the base uri for this node,
   *         NULL otherwise
   */
xmlChar *
filesGetBaseUri(xmlNodePtr node)
{
    xmlChar *result = NULL;

    if (!node || !node->doc)
        return result;

    while (node && node->parent) {
        /*
         * result =  xmlGetNsProp(node, BAD_CAST "uri", XSLDBG_XML_NAMESPACE);
         */
        if (node->type == XML_ELEMENT_NODE) {
            result = xmlGetProp(node, BAD_CAST "xsldbg:uri");
            if (result)
                break;
        }
        node = node->parent;
    }

    if (!result && node->doc && node->doc->URL)
        result = xmlStrdup(node->doc->URL);

    return result;
}




/**
 * filesEntityList:
 *
 * Return the list entity names used for documents loaded
 *
 * Returns the list entity names used for documents loaded
 */
ArrayListPtr
filesEntityList(void)
{
    return entityNameList;
}


extern int intVolitileOptions[OPTIONS_VERBOSE - OPTIONS_XINCLUDE + 1];

/**
 * filesLoadCatalogs:
 *
 * Load the catalogs specifed by OPTIONS_CATALOG_NAMES if 
 *      OPTIONS_CATALOGS is enabled
 * Returns 1 if sucessful
 *         0 otherwise   
 */
int
filesLoadCatalogs(void)
{
    int result = 0;
    int catalogOptId = OPTIONS_CATALOGS - OPTIONS_XINCLUDE;
    const char *catalogs;

    /* only reload catalogs if something has changed */
    if (intVolitileOptions[catalogOptId] !=
        isOptionEnabled(OPTIONS_CATALOGS)) {
        xmlCatalogCleanup();
        if (intVolitileOptions[catalogOptId] != 0) {
            if (getStringOption(OPTIONS_CATALOG_NAMES) == NULL) {
#ifdef __riscos
                catalogs = getenv("SGML$CatalogFiles");
#else
                catalogs = getenv("SGML_CATALOG_FILES");
#endif
                if (catalogs == NULL) {
#ifdef __riscos
                    xsltGenericError(xsltGenericErrorContext,
                                     "Variable SGML$CatalogFiles not set\n");
#else
                    xsltGenericError(xsltGenericErrorContext,
                                     "Variable $SGML_CATALOG_FILES not set\n");
#endif
                    return result;
                } else
                    setStringOption(OPTIONS_CATALOG_NAMES,
                                    (xmlChar *) catalogs);
            } else
                catalogs = (char *) getStringOption(OPTIONS_CATALOG_NAMES);
            xmlLoadCatalogs(catalogs);
        }
        result++;
    }
    return result;
}




  /**
   * filesEncode:
   * @text: Is valid, text to translate from UTF-8, 
   *
   * Return  A  new string of converted @text
   *
   * Returns  A  new string of converted @text, may be NULL
   */
xmlChar *
filesEncode(const xmlChar * text)
{
    xmlChar *result = NULL;

    if (!stdoutEncoding || !encodeInBuff || !encodeOutBuff)
        return xmlStrdup(text); /* no encoding active return as UTF-8 */

    xmlBufferEmpty(encodeInBuff);
    xmlBufferEmpty(encodeOutBuff);
    xmlBufferCat(encodeInBuff, text);

    if (xmlCharEncOutFunc(stdoutEncoding, encodeOutBuff, encodeInBuff) >=
        0) {
        result = xmlStrdup(xmlBufferContent(encodeOutBuff));
    } else {
        xsltGenericError(xsltGenericErrorContext,
                         "Encoding of text failed\n");
        return xmlStrdup(text); /*  panic,  return as UTF-8 */
    }
    return result;
}



  /**
   * filesDeccode:
   * @text: Is valid, text to translate from current encoding to UTF-8, 
   *
   * Return  A  string of converted @text
   *
   * Returns  A  string of converted @text, may be NULL
   */
xmlChar *
filesDecode(const xmlChar * text)
{
    xmlChar *result = NULL;

    if (!stdoutEncoding || !encodeInBuff || !encodeOutBuff)
        return xmlStrdup(text); /* no encoding active return as UTF-8 */

    xmlBufferEmpty(encodeInBuff);
    xmlBufferEmpty(encodeOutBuff);
    xmlBufferCat(encodeInBuff, text);

    if (xmlCharEncInFunc(stdoutEncoding, encodeOutBuff, encodeInBuff) >= 0) {
        result = xmlStrdup(xmlBufferContent(encodeOutBuff));
    } else {
        xsltGenericError(xsltGenericErrorContext,
                         "Encoding of text failed\n");
        return xmlStrdup(text); /*  panic,  return @text unchanged */
    }
    return result;
}


  /*
   * filesSetEncoding:
   * @encoding : Is a valid encoding supported by the iconv library or NULL
   *
   * Opens encoding for all standard output to @encoding. If  @encoding 
   *        is NULL then close current encoding and use UTF-8 as output encoding
   *
   * Returns 1 if successful in setting the encoding of all standard output
   *           to @encoding
   *         0 otherwise
   */
int
filesSetEncoding(const char *encoding)
{
    int result = 0;

    if (encoding) {
        /* don't switch encoding unless we've found a valid encoding */
        xmlCharEncodingHandlerPtr tempEncoding =
            xmlFindCharEncodingHandler(encoding);
        if (tempEncoding) {
            filesSetEncoding(NULL);     /* re-use code to close encoding */
            stdoutEncoding = tempEncoding;
            result =
                (xmlCharEncOutFunc(stdoutEncoding, encodeOutBuff, NULL) >=
                 0);
            if (!result) {
                xmlCharEncCloseFunc(stdoutEncoding);
                stdoutEncoding = NULL;
                xsltGenericError(xsltGenericErrorContext,
                                 "Unable to initialize encoding %s",
                                 encoding);
            } else
                setStringOption(OPTIONS_ENCODING, (xmlChar*) encoding);
        } else {
            xsltGenericError(xsltGenericErrorContext,
                             "Invalid encoding %s\n", encoding);
        }
    } else {
        /* close encoding and use UTF-8 */
        if (stdoutEncoding)
            result = (xmlCharEncCloseFunc(stdoutEncoding) >= 0);
        else
            result++;
        stdoutEncoding = NULL;
    }
    return result;
}
