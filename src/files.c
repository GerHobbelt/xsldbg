
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

#include "xsldbg.h"
#include "debugXSL.h"
#include "files.h"
#include "options.h"
#include <stdio.h>


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


/* -----------------------------------------
   Private function declarations for files.c
 -------------------------------------------*/

/**
 * guessStylesheetHelper:
 * @payload : valid xsltStylesheetPtr
 * @data : valid searchInfoPtr of type SEARCH_NODE
 * @name : not used
 *
 * Try to guess what the complete file/URI is. If successful the search
 *   info will be set to found and the search data will contain the
 *   file name found. We are given our payload via walkStylesheets
 */
void guessStylesheetHelper(void *payload, void *data,
                           xmlChar * name ATTRIBUTE_UNUSED);


/**
 * guessStylesheetHelper2:
 * @payload : valid xmlNodePtr of the included stylesheet 
 * @data : valid searchInfoPtr of type SEARCH_NODE
 * @name : not used
 *
 * Try to guess what the complete file/URI is. If successful the search
 *   info will be set to found and the search data will contain the
 *   file name found. We are given our payload via walkIncludes
 */
void guessStylesheetHelper2(void *payload, void *data,
                            xmlChar * name ATTRIBUTE_UNUSED);

/* ------------------------------------- 
    End private functions
---------------------------------------*/




FILE *terminalIO;
/* No longer needed
static FILE *oldStdin, *oldStdout, *oldStderr;
char *ttyName, *termName;

*/

/**
 * redirectToTerminal:
 * @device: terminal to redirect i/o to , will not work under win32
 *
 * Open communications to the terminal device @device
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

    if (terminalIO != NULL)
        fclose(terminalIO);

    if (device[0] == '\0') {
        /* look like we are supposed to close the terminal */
        selectNormalIO();       /* shouldn't be needed but just in case */
    } else {
        terminalIO = fopen((char *) device, "w");
        if (terminalIO != NULL) {
            termName = (char *) device; /* JRF: Not sure how safe this is */
            /* This can't be done reliably; really need more thought */
            result++;
        } else {
            xsltGenericError(xsltGenericErrorContext,
                             "Unable to open terminal %s", device);
            termName = NULL;
        }
    }
#else

#ifdef HAVE_UNISTD_H              /* fix me for WinNT */
    if (terminalIO != NULL)
        fclose(terminalIO);

    if ((device[0] >= '0') && (device[0] <= '9')) {
        /* look like we are supposed to close the terminal */
        selectNormalIO();       /* shouldn't be needed but just in case */
    } else {

        terminalIO = fopen(device, "w");
        if (terminalIO != NULL) {
	  /*
            termName = device;
            dup2(fileno(terminalIO), fileno(stdin));
            dup2(fileno(terminalIO), fileno(stderr));
            dup2(fileno(terminalIO), fileno(stdout));
	  */
            result++;
        } else {
            xsltGenericError(xsltGenericErrorContext,
                             "Unable to open terminal %s", device);
	    /*
            termName = NULL;
	    */
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
    int result = 0;

    if (termName) {
        freopen(termName, "w", stdout);
        freopen(termName, "w", stderr);
        freopen(termName, "r", stdin);
        result++;
    } else
        result++;

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
    int result = 0;

#ifdef UNISTD_H
    if (ttyName) {
        freopen(ttyName, "w", stdout);
        freopen(ttyName, "w", stderr);
        freopen(ttyName, "r", stdin);
    }
#endif
    result++;
    */
    return 1;
}


/**
 * guessStylesheetHelper:
 * @payload : valid xsltStylesheetPtr
 * @data : valid searchInfoPtr of type SEARCH_NODE
 * @name : not used
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
    searchInfoPtr searchInf = (searchInfoPtr) data;
    nodeSearchDataPtr searchData = NULL;

    if (!style || !style->doc || !searchInf || !searchInf->data ||
        (searchInf->type != SEARCH_NODE))
        return;

    searchData = (nodeSearchDataPtr) searchInf->data;
    if (searchData->nameInput && (searchData->absoluteNameMatch == NULL)) {
        /* at this point we know that we have not made an absolute match 
         * but we may have made a relative match */
        if (xmlStrCmp(style->doc->URL, searchData->nameInput) == 0) {
            /* absolute path match great! */
            searchData->absoluteNameMatch =
                (xmlChar *) xmlMemStrdup((char *) style->doc->URL);
            searchData->node = (xmlNodePtr) style->doc;
            searchInf->found = 1;
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
            searchInf->found = 1;
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
            searchInf->found = 1;
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
                    searchInf->found = 1;
                }
            }
        }
    }
}


/**
 * guessStylesheetHelper2:
 * @payload : valid xmlNodePtr of the included stylesheet 
 * @data : valid searchInfoPtr of type SEARCH_NODE
 * @name : not used
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
    searchInfoPtr searchInf = (searchInfoPtr) data;
    nodeSearchDataPtr searchData = NULL;

    if (!node || !node->doc || !searchInf || !searchInf->data ||
        (searchInf->type != SEARCH_NODE))
        return;

    searchData = (nodeSearchDataPtr) searchInf->data;
    if (searchData->nameInput && (searchData->absoluteNameMatch == NULL)) {
        /* at this point we know that we have not made an absolute match 
         * but we may have made a relative match */
        if (xmlStrCmp(node->doc->URL, searchData->nameInput) == 0) {
            /* absolute path match great! */
            searchData->absoluteNameMatch =
                (xmlChar *) xmlMemStrdup((char *) node->doc->URL);
            searchData->node = node;
            searchInf->found = 1;
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
            searchInf->found = 1;
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
            searchInf->found = 1;
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
                    searchInf->found = 1;
                }
            }
        }
    }
}

/**
 * guessStyleSheetName:
 *
 * Try to find a matching stylesheet name
 * Sets the values in @searchinf depending on outcome of search
 * 
 */
void
guessStylesheetName(searchInfoPtr searchInf)
{
    nodeSearchDataPtr searchData;

    if (!searchInf)
        return;

    searchData = (nodeSearchDataPtr) searchInf->data;
    if (searchData->nameInput == NULL)
        return;                 /* must supply name of file to look for */

    walkStylesheets((xmlHashScanner) guessStylesheetHelper,
                    searchInf, getStylesheet());
    if (!searchInf->found) {
        /* try looking in the included stylesheets */
        walkIncludes((xmlHashScanner) guessStylesheetHelper2,
                     searchInf, getStylesheet());
    }
}


/**
 * stylePath:
 *
 * Returns the base path for the top stylesheet ie
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
 * Returns the working directory as set by changeDir function
 */
xmlChar *
workingPath(void)
{
    return workingDirPath;
}


/**
 * changeDir:
 * @path : path to adopt as new working directory
 *
 * Change working directory to path 
 */
int
changeDir(const xmlChar * path)
{
    int result = 0;
    const xmlChar endString[2] = { PATHCHAR, '\0' };

    if (path && chdir((char *) path) == 0) {
        if (workingDirPath)
            xmlFree(workingDirPath);
        /* must have path char at end of path name */
        xmlStrCpy(buffer, path);
        xmlStrCat(buffer, endString);
        workingDirPath = (xmlChar *) xmlMemStrdup((char *) buffer);
        result++;
    }
    if (!result)
        xsltGenericError(xsltGenericErrorContext,
                         "Unable to change to directory %s\n", path);
    else
        xsltGenericError(xsltGenericErrorContext,
                         "Change to directory %s\n", path);
    return result;
}


/**
 * loadXmlFile:
 * @path : xml file to load
 * @type : a valid FileTypeEnum 
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
                }
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
 * @type : a valid FileTypeEnum 
 * 
 * Free memory associated with the xml file 
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
 * Returns the topmost stylesheet non-null on success,
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
 * Returns the current "temporary" document
 */
xmlDocPtr
getTemporaryDoc(void)
{
    return tempDocument;
}


/**
 * getMainDoc:
 *
 * Returns the main docment
 */
xmlDocPtr
getMainDoc(void)
{
    return topDocument;
}


/**
 * filesReloaded:
 * @reloaded : if = -1 then ignore @reloaded
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
    result = 1;                 /* nothing else  to do for the moment */

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

    if (terminalIO != NULL)
        fclose(terminalIO);

    result = freeXmlFile(FILES_SOURCEFILE_TYPE);
    if (result)
        result = freeXmlFile(FILES_XMLFILE_TYPE);
    if (result)
        result = freeXmlFile(FILES_TEMPORARYFILE_TYPE);
    if (!result)
        xsltGenericError(xsltGenericErrorContext,
                         "Unable to free memory used by xml/xsl files\n");
    if (workingDirPath)
        xmlFree(workingDirPath);
}
