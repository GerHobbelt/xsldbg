
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





/**
 * Provide a file support
 *
 * @short file support
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */






extern FILE *terminalIO;


/* used by loadXmlFile, freeXmlFile functions */
    enum FileTypeEnum {
        FILES_XMLFILE_TYPE = 100,       /* pick a unique starting point */
        FILES_SOURCEFILE_TYPE,
        FILES_TEMPORARYFILE_TYPE
    };







    int openTerminal(xmlChar * device);





/**
 * @returns 1 if able to use prevously opened terminal 
 *          0 otherwise
*/


    int selectTerminalIO(void);




/** 
 * @returns 1 if able to select orginal stdin, stdout, stderr
 *          0 otherwise
*/


    int selectNormalIO(void);




/**
 * @returns the last template node found, if any
 */


    xsltTemplatePtr getTemplate(void);





/**
 * Open communications to the terminal device @p device
 *
 * @returns 1 if sucessful
 *          0 otherwise
 *
 * @param device Terminal to redirect i/o to, will not work under win32
 */


    void guessStylesheetName(searchInfoPtr searchInf);





/**
 * @return the base path for the top stylesheet ie
 *        ie URL minus the actual file name
 */


    xmlChar *stylePath(void);





/**
 * @return the working directory as set by changeDir function
 */


    xmlChar *workingPath(void);





/**
 * Change working directory to path 
 *
 * @param path The operating system path(directory) to adopt as 
 *         new working directory
 */


    int changeDir(const xmlChar * path);




/**
 * @returns 1 on success,
 *         0 otherwise 
 *
 * @param path The xml file to load
 * @param type A valid FileTypeEnum
 */


    int loadXmlFile(const xmlChar * path, FileTypeEnum fileType);




/**
 * Free memory associated with the xml file 
 *
 * @returns 1 on success,
 *         0 otherwise
 *
 * @param type : a valid FileTypeEnum
 */


    int freeXmlFile(FileTypeEnum fileType);







    xsltStylesheetPtr getStylesheet(void);




/**
 * Return the topmost stylesheet 
 *
 * @returns non-null on success,
 *          NULL otherwise
 */


    xmlDocPtr getTemporaryDoc(void);




/**
 * @returns the main document
 */


    xmlDocPtr getMainDoc(void);




/**
 * @returns 1 if stylesheet or its xml data file has been "flaged" as reloaded,
 *         0 otherwise
 *
 * @param reloaded If = -1 then ignore @p reloaded
 *             otherwise change the status of files to value of @p reloaded
 */


    int filesReloaded(int reloaded);




/**
 * Initialize the file module
 *
 * @returns 1 on success,
 *          0 otherwise
 */


    int filesInit(void);




/**
 * Free memory used by file related structures
 */


    void filesFree(void);




