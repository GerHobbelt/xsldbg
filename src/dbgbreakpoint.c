
/***************************************************************************
                          breakpoint.c  -  description
                             -------------------
    begin                : Fri Nov 2 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

#include "xsldbg.h"
#include "breakpointInternals.h"


/*
-----------------------------------------------------------
       Breakpoint debugger functions
-----------------------------------------------------------
*/


/* This is our major structure, it is a list of hash tables. Each 
 hash table has breakpoints with the same line number. A line
 number is used as an index into this list to get the right hash table.
 Then its just a matter of a simple hash table lookup  */
ArrayListPtr breakList;

/* keep track of what break point id we're up to*/
static int breakPointCounter = 0;

/* What is the current breakpoint is only valid up to the start of 
 xsldbg command prompt. ie don't use it after deletion of breakpoints */
xslBreakPointPtr activeBreakPointItem = NULL;

/**
 * lineNoItemNew:
 * 
 * Returns a new hash table for breakPoints
 */
xmlHashTablePtr
lineNoItemNew(void)
{
    xmlHashTablePtr hash;

    hash = xmlHashCreate(4);

    return hash;
}


/**
 * lineNoItemFree:
 * @item : valid hashtable of breakpoints
 * 
 * Free @item and all its contents
 */
void
lineNoItemFree(void *item)
{
    xmlHashTablePtr hash = (xmlHashTablePtr) item;

    if (item) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Freeing breakpoint line hash"
                         " with %d elements \n", xmlHashSize(item));
#endif
        xmlHashFree(hash, breakPointItemFree);
    }
}


/**
 * lineNoItemDelete:
 * @breakPointHash : is valid
 * @breakPoint : is valid
 * 
 * Returns 1 if able to delete @breakPoint from @breakPointHash,
 *         0 otherwise
 */
int
lineNoItemDelete(xmlHashTablePtr breakPointHash,
                 xslBreakPointPtr breakPoint)
{
    int result = 0;

    if (breakPointHash && breakPoint) {
        if (xmlHashRemoveEntry(breakPointHash, breakPoint->url,
                               breakPointItemFree) == 0)
            result++;
    }
    return result;
}


/**
 * lineNoItemAdd:
 * @breakPointHash : is valid
 * @breakPoint : is valid
 *
 * Returns 1 if able to add @breakPoint to @breakPointHash,
 *         0 otherwise
 */
int
lineNoItemAdd(xmlHashTablePtr breakPointHash, xslBreakPointPtr breakPoint)
{
    int result = 0;

    if (breakPointHash && breakPoint) {
        if (xmlHashAddEntry(breakPointHash, breakPoint->url, breakPoint) ==
            0)
            result++;
    }
    return result;
}


/**
 * lineNoItemGet:
 * @lineNo : lineNo >= 0
 *
 * Returns the hash table for this line if successful, 
 *        NULL otherwise
 */
xmlHashTablePtr
lineNoItemGet(long lineNo)
{
    if (!breakList) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "BreakPoints not initialized\n");
#endif
        return NULL;
    } else
        return (xmlHashTablePtr) xslArrayListGet(breakList, lineNo);
}


/**
 * breakPointInit:
 *
 * Returns 1 if breakpoints have been initialized properly and all
 *               memory required has been obtained,
 *         0 otherwise
*/
int
breakPointInit(void)
{
    int result = 0;

    /* the average file has 395 lines of code so add 100 lines now */
    breakList = xslArrayListNew(100, lineNoItemFree);
    if (breakList) {
        /*
         * We don't need to do any thing else, as its done when we add the 
         *    breakPoints
         */
        result++;
    } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Unable to intialize breakPoints: memory error\n");
#endif
    }
    return result;
}


/**
 * breakPointFree:
 *
 * Free all memory used by breakPoints 
 */
void
breakPointFree(void)
{
    if (breakList)
        xslArrayListFree(breakList);
    breakList = NULL;
}


/**
 * xslEmptyBreakPoint:
 *
 * Returns 1 if able to empty the breakpoint list of its contents,
 *         0  otherwise
 */
int
xslEmptyBreakPoint(void)
{
    return xslArrayListEmpty(breakList);
}


/** 
 * breakPointItemNew:
 * 
 * Create a new break point item
 * Returns valid breakPoint with default values set if successful, 
 *         NULL otherwise
 */
xslBreakPointPtr
breakPointItemNew(void)
{
    xslBreakPointPtr breakPoint =
        (xslBreakPointPtr) xmlMalloc(sizeof(xslBreakPoint));
    if (breakPoint) {
        breakPoint->url = NULL;
        breakPoint->lineNo = -1;
        breakPoint->templateName = NULL;
        breakPoint->enabled = 1;
        breakPoint->id = ++breakPointCounter;
        breakPoint->type = DEBUG_BREAK_SOURCE;
    }
    return breakPoint;
}



/**
 * breakPointItemFree:
 * @payload : valid xslBreakPointPtr 
 * @name : don't care
 *
 * Free memory associated with this breakPoint
 */
void
breakPointItemFree(void *payload, xmlChar * name ATTRIBUTE_UNUSED)
{
    if (payload) {
        xslBreakPointPtr breakPoint = (xslBreakPointPtr) payload;

#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext, "Freeing breakpoint: ");
        printBreakPoint(stderr, breakPoint);
        xsltGenericError(xsltGenericErrorContext, "\n");
#endif
        if (breakPoint->url)
            xmlFree(breakPoint->url);
        if (breakPoint->templateName)
            xmlFree(breakPoint->templateName);
        xmlFree(breakPoint);
    }
}


/**
 * activeBreakPoint(void);
 *
 * Returns the last breakPoint that we stoped at
 */
xslBreakPointPtr
activeBreakPoint(void)
{
    return activeBreakPointItem;
}


/**
 * setActiveBreakPoint:
 * @breakPoint : is valid breakPoint or NULL
 *
 * Set the active breakPoint
 */
void
setActiveBreakPoint(xslBreakPointPtr breakPoint)
{
    activeBreakPointItem = breakPoint;
}


/**
 * xslAddBreakPoint:
 * @url : url non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber : number >= 0 and is available in url specified and points to 
 *               an xml element
 * @temlateName : the template name of breakPoint or NULL
 * @type : DEBUG_BREAK_SOURCE if are we stopping at a xsl source line
 *         DEBUG_BREAK_DATA otherwise
 *
 * Add break point at file and line number specifiec
 * Returns  1 if successfull,
 *	    0 otherwise 
*/
int
xslAddBreakPoint(const xmlChar * url, long lineNumber,
                 const xmlChar * templateName, int type)
{
    int result = 0;
    xmlHashTablePtr breakPointHash;     /* hash of breakPoints */
    xslBreakPointPtr breakPoint;

    if (!breakList) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error breakPoints not initialized\n");
#endif
        return result;
    }

    if (!url || (lineNumber == -1)) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Invalid url or line number to xslAddBreakPoint\n");
#endif
        return result;
    }

    /* if breakpoint already exists then don;t add it */
    if (xslIsBreakPoint(url, lineNumber)) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Breakpoint at file %s: line %d exists\n",
                         url, lineNumber);
#endif
        return result;
    }

    breakPoint = breakPointItemNew();
    if (breakPoint) {
        breakPoint->url = (xmlChar *) xmlMemStrdup((char *) url);
        breakPoint->lineNo = lineNumber;
        if (templateName)
            breakPoint->templateName =
                (xmlChar *) xmlMemStrdup((char *) templateName);
        else
            breakPoint->templateName = NULL;
        breakPoint->type = type;

        /* add new breakPoint to the right hash table */
        breakPointHash = lineNoItemGet(lineNumber);
        if (breakPointHash) {
            result = lineNoItemAdd(breakPointHash, breakPoint);
        } else {
            /* Grow breakList size */
            int lineIndex;
            int newEntries = breakList->count;
            xmlHashTablePtr hash;

            result = 1;
            if ((lineNumber < breakList->count) && breakList->count) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
                xsltGenericError(xsltGenericErrorContext,
                                 "Unable to find breakpoint line hash at %d\n",
                                 lineNumber);
#endif
            } else {
                if (breakList->count + newEntries < lineNumber)
                    newEntries = lineNumber - breakList->count + 1;


#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
                xsltGenericError(xsltGenericErrorContext,
                                 "Size of line list was %d adding %d entries\n",
                                 breakList->count, newEntries);
#endif
                lineIndex = 0;
                while ((lineIndex < newEntries) && result) {
                    hash = lineNoItemNew();
                    if (hash) {
                        result = result
                            && xslArrayListAdd(breakList, hash);
                    } else {
                        result = 0;
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
                        xsltGenericError(xsltGenericErrorContext,
                                         "Unable to create hash table breakPoint list: memory error\n");
#endif
                        return result;
                    }
                    lineIndex++;
                }
                /* find the newly added hashtable of breakpoints */
                breakPointHash = lineNoItemGet(lineNumber);
                if (breakPointHash) {
                    result = lineNoItemAdd(breakPointHash, breakPoint);
                } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
                    xsltGenericError(xsltGenericErrorContext,
                                     "Unable to create new breakPoint :interal error\n");
#endif
                    return result;
                }
            }

        }
    } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Unable to create new breakPoint : memory error\n");
#endif
    }

    return result;
}


/**
 * xslDeleteBreakPoint:
 * @breakPoint : is valid
 *
 * Delete the break point specified if it can be found using 
 *    @breakPoint's url and lineNo
 * Returns 1 if successfull,
 *	    0 otherwise
*/
int
deleteBreakPoint(xslBreakPointPtr breakPoint)
{
    int result = 0;
    xmlHashTablePtr breakPointHash;     /* hash of breakPoints */

    if (!breakPoint)
        return result;

    breakPointHash = lineNoItemGet(breakPoint->lineNo);
    if (breakPointHash) {
        result = lineNoItemDelete(breakPointHash, breakPoint);
    } else {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Break point not found: xslDeleteBreakPoint\n");
#endif
    }
    return result;
}


/**
 * xslEnableBreakPoint:
 * @breakPoint : a valid breakPoint
 * @enable : enable break point if 1, disable if 0, toggle if -1
 *
 * Enable or disable a break point
 * Returns 1 if successfull,
 *	    0 otherwise
*/
int
enableBreakPoint(xslBreakPointPtr breakPoint, int enable)
{
    int result = 0;

    if (breakPoint) {
        if (enable != XSL_TOGGLE_BREAKPOINT)
            breakPoint->enabled = enable;
        else {
            if (breakPoint->enabled)
                breakPoint->enabled = 0;
            else
                breakPoint->enabled = 1;
        }
        result++;
    }
    return result;
}


/**
 * xslBreakPointCount:
 *
 * Returns the number of hash tables of breakPoints with the same line number
 */
int
xslBreakPointLinesCount(void)
{
    if (!breakList) {
#ifdef WITH_XSLT_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "BreakPoints not initialized\n");
#endif
        return 0;
    } else
        return xslArrayListCount(breakList);
}


/**
 * xslBreakPointLinesList:
 *
 * Returns the list of hash tables for breakpoints
 *        Dangerous function to use!! 
 */
ArrayListPtr
xslBreakPointLineList(void)
{
    return breakList;
}


/**
 * xslGetBreakPoint:
 * @url : url non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber : number >= 0 and is available in url specified
 * Returns break point if break point exists at location specified,
 *	    NULL otherwise
*/
xslBreakPointPtr
getBreakPoint(const xmlChar * url, long lineNumber)
{
    xmlHashTablePtr breakHash = lineNoItemGet(lineNumber);
    xslBreakPointPtr breakPoint = NULL;

    if (!breakHash || !url)
        return breakPoint;

    breakPoint = xmlHashLookup(breakHash, url);
    return breakPoint;
}


/**
 * printBreakPoint:
 * @file :  is valid
 * @breakPoint : is a valid breakPoint
 *
 * Print the details of @breakPoint @file 
 *
 * Returns 1 if successfull,
 *	   0 otherwise
 */
int
printBreakPoint(FILE * file, xslBreakPointPtr breakPoint)
{
    int result = 0;

    if (!file || !breakPoint)
        return result;

    fprintf(file, "Breakpoint %d ", breakPoint->id);
    if (breakPoint->enabled)
        fprintf(file, "enabled ");
    else
        fprintf(file, "disabled ");

    if (breakPoint->templateName) {
        fprintf(file, "for template :\"%s\" ", breakPoint->templateName);
    }

    if (breakPoint->url) {
        fprintf(file, "in file %s : line %ld",
                breakPoint->url, breakPoint->lineNo);
    } else {
        fprintf(file, "in file <n/a>, line %ld", breakPoint->lineNo);
    }
    return ++result;
}


/**
 * xslIsBreakPoint:
 * @url : url non-null, non-empty file name that has been loaded by
 *                    debugger
 * @lineNumber : number >= 0 and is available in url specified
 *
 * Determine if there is a break point at file and line number specified
 * Returns 1  if successfull,
 *	   0 otherwise
*/
int
xslIsBreakPoint(const xmlChar * url, long lineNumber)
{
    int result = 0;

    if (!url || (lineNumber == -1))
        return result;

    result = (getBreakPoint(url, lineNumber) != NULL);

    return result;
}


/**
 * xslIsBreakPointNode:
 * @node : node != NULL
 *
 * Determine if a node is a break point
 * Returns  1 on success,
 *          0 otherwise
 */
int
xslIsBreakPointNode(xmlNodePtr node)
{
    int result = 0;

    if (!node || !node->doc)
        return result;

    if (xmlGetLineNo(node) == -1)
        return result;

    if (node->doc->URL) {
        result = xslIsBreakPoint(node->doc->URL, xmlGetLineNo(node));
    }

    return result;
}
