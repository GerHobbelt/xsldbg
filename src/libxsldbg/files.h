
#ifndef FILES_H
#define FILES_H

#include "search.h"
#include "arraylist.h"
#include <QString>

#ifdef HAVE_UNISTD_H
#include <unistd.h>             /* need chdir function */
#endif

extern FILE *terminalIO;

#define XSLDBG_XML_NAMESPACE (const xmlChar *) "http://xsldbg.sourceforge.net/namespace"

typedef struct _entityInfo entityInfo;
typedef entityInfo *entityInfoPtr;
struct _entityInfo {
    xmlChar *SystemID;
    xmlChar *PublicID;
};


/* how many lines do we print before pausing when 
 * performing "more" on a UTF-8 file. See function filesMoreFile */
#define FILES_NO_LINES 20

/* Define the types of file names that we are intested in when creating
 * search results */
typedef enum {
    FILES_SEARCHINPUT,
    FILES_SEARCHXSL,
    FILES_SEARCHRESULT
} FilesSearchFileNameEnum;



/*-----------------------------------------------------------
*  General function for working with files
* -----------------------------------------------------------*/


/**
 * Fixes the nodes from firstNode to lastNode so that debugging can occur
 *
 * @param uri Is valid as provided by libxslt
 * @param firstNode Is valid
 * @param lastNode Is Valid
 */
void filesEntityRef(xmlEntityPtr ent, xmlNodePtr firstNode, xmlNodePtr lastNode);


/**
 * Return the list entity names used for documents loaded
 *
 * @returns The list entity names used for documents loaded
 */
arrayListPtr filesEntityList(void);


/**
 * Set the base uri for this node. Function is used when xml file
 *    has external entities in its DTD
 * 
 * @param node Is valid and has a doc parent
 * @param uri Is Valid
 * 
 * @returns 1 if successful,
 *          0 otherwise
 */
int filesSetBaseUri(xmlNodePtr node, const xmlChar * uri);


/**
 * Get a copy of the base uri for this node. Function is most useful 
 *  used when xml file  has external entities in its DTD
 * 
 * @param node : Is valid and has a doc parent
 * 
 * @returns The a copy of the base uri for this node,
 *          NULL otherwise
 */
xmlChar *filesGetBaseUri(xmlNodePtr node);


/**
 * Return the name of tempfile requested.
 * @param fleNumber : Number of temp file required
 *     where @p fileNumber is 
 *      0 : file name used by cat command
 *      1 : file name used by profiling output
 *
 *  This is a platform specific interface
 *
 * Returns The name of temp file to be used for temporary results if successful,
 *         an empty QByteArray otherwise
 */
QByteArray filesTempFileName(int fileNumber);


/**
 * Load the catalogs specified by OPTIONS_CATALOG_NAMES if 
 *      OPTIONS_CATALOGS is enabled
 *
 * @returns 1 if successful
 *          0 otherwise   
 */
int filesLoadCatalogs(void);


/**
 * Return  A  string of converted @text
 *
 * @param text Is valid, text to translate from UTF-8, 
 *
 * Returns  A  string of converted @text, may be NULL
 */
xmlChar *filesEncode(const xmlChar * text);


/**
 * Return  A  string of converted @text
 *
 * @param test Is valid, text to translate from current encoding to UTF-8, 
 *
 * Returns  A  string of converted @text, may be NULL
 */
xmlChar *filesDecode(const xmlChar * text);


/**
 * Opens encoding for all standard output to @p encoding. If  @p encoding 
 *        is NULL then close current encoding and use UTF-8 as output encoding
 *
 * @param encoding Is a valid encoding supported by the iconv library or NULL
 *
 * Returns 1 if successful in setting the encoding of all standard output
 *           to @p encoding
 *         0 otherwise
 */
int filesSetEncoding(const char *encoding);


/* used by filesLoadXmlFile, filesFreeXmlFile functions */
typedef enum {
    FILES_XMLFILE_TYPE = 100,       /* pick a unique starting point */
    FILES_SOURCEFILE_TYPE,
    FILES_TEMPORARYFILE_TYPE
} FileTypeEnum;


/**
 * Open communications to the terminal device @p device
 *
 * @param device Terminal to redirect i/o to , will not work under win32
 *
 * @returns 1 if successful
 *          0 otherwise
 */
int openTerminal(xmlChar * device);


/**
 * Try to find a matching stylesheet name
 * Sets the values in @p searchinf depending on outcome of search
 *
 * @param searchInf Is valid
 */
void guessStylesheetName(searchInfoPtr searchInf);


/**
 * Return the base path for the top stylesheet ie
 *        ie URL minus the actual file name
 *
 * @returns The base path for the top stylesheet ie
 *        ie URL minus the actual file name
 */
QString stylePath(void);


/**
 * Return the working directory as set by changeDir function
 *
 * @return the working directory as set by changeDir function
 */
QString workingPath(void);


/**
 * Change working directory to path 
 *
 * @param path The operating system path(directory) to adopt as 
 *         new working directory
 *
 * @returns 1 on success,
 *          0 otherwise
 */
int changeDir(QString path);


/**
 * Load specified file type, freeing any memory previously used 
 *
 * @returns 1 on success,
 *         0 otherwise 
 *
 * @param path The xml file to load
 * @param fileType A valid FileTypeEnum
 */
int filesLoadXmlFile(const xmlChar * path, FileTypeEnum fileType);


/**
 * Free memory associated with the xml file 
 *
 * @returns 1 on success,
 *         0 otherwise
 *
 * @param fileType : A valid FileTypeEnum
 */
int filesFreeXmlFile(FileTypeEnum fileType);


/**
 * Return the topmost stylesheet 
 *
 * @returns Non-null on success,
 *         NULL otherwise
 */
xsltStylesheetPtr filesGetStylesheet(void);


/**
 * Return the current "temporary" document
 *
 * @returns non-null on success,
 *          NULL otherwise
 */
xmlDocPtr filesGetTemporaryDoc(void);


/**
 * Return the main docment
 *
 * @returns the main document
 */
xmlDocPtr filesGetMainDoc(void);


/**
 * @returns 1 if stylesheet or its xml data file has been "flagged" as reloaded,
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


/**
 * Test if filename could be  a stylesheet 
 *
 * @returns True if @name has the ".xsl" extension
 *
 *  @param fileName Is valid
 */
int filesIsSourceFile(xmlChar * fileName);


/**
 * Do a "more" like print of file specified by @fileName OR
 *   @file. If both are provided @file will be used. The content 
 *   of file chosen must be in UTF-8, and will be  printed in 
 *   the current encoding selected. The function will pause output 
 *   after FILES_NO_LINES lines have been printed waiting for
 *   user to enter "q" to quit or any other text to continue.
 *
 * @returns 1 if successful,
 *          0 otherwise
 *
 * @param fileName May be NULL
 * @param file May be NULL
 *
 */
int filesMoreFile(const xmlChar * fileName, FILE * file);


/**
 * Get the base path to be used for storing search results
 *
 * @returns The base path to be used for storing search results
 */
QString filesSearchResultsPath(void);


/**
 * Return A copy of the conversion of @uri to a file name
 *        that is suitable to be used with the fopen function.
 *        May be NULL, if out of memory, @uri does not use the
 *        "file://" protocol, or unable to convert to a valid file name
 *
 * Returns A copy of the conversion of @uri to a file name
 *        that is suitable to be used with the fopen function.
 *        May be NULL, if out of memory, @uri does not use the
 *        "file://" prefix, or unable to convert to a valid file name
 *    
 * @param uri A valid URI that uses the "file://" prefix
 *
 */
xmlChar *filesURItoFileName(const xmlChar* uri);


/**
 * Update the URL and  line number that we stoped at 
 *
 * @param node A valid node
 */
void xsldbgUpdateFileDetails(xmlNodePtr node);


/**
 * What line number are we at 
 *
 * @returns The current line number of xsldbg, may be -1
 */
int xsldbgLineNo(void);


/**
 * What URL did we stop at
 *
 * @returns A NEW copy of URL stopped at. Caller must free memory for URL,   
 *         may be NULL
 */
xmlChar *xsldbgUrl(void);


/* -----------------------------------------------------------
*  Platform specific file functions
* -----------------------------------------------------------*/



/**
 * Initialize the platform specific files module
 *
 *  This is a platform specific interface
 *
 * @returns 1 if successful
 *          0 otherwise  
 */
int filesPlatformInit(void);


/**
 * Free memory used by the platform specific files module
 *
 *  This is a platform specific interface
 *
 */
void filesPlatformFree(void);


/**
 * Return a copy of the file name to use as an argument to searching
 *
 * @returns A copy of the file name to use as an argument to searching
 *
 * @param fileType Is valid
 *
 */
QString filesSearchFileName(FilesSearchFileNameEnum fileType);

#endif
