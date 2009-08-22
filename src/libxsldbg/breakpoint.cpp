
/**
 *
 *  This file is part of the kdewebdev package
 *  Copyright (c) 2001 Keith Isdale <keith@kdewebdev.org>
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


#include "breakpoint.h"
#include "xsldbg.h"
#include "arraylist.h"
#include "options.h"

extern int xsldbgValidateBreakpoints; /*located in debugXSL.c*/

/*-----------------------------------------------------------
       Private functions
-----------------------------------------------------------*/

xmlHashTablePtr lineNoItemNew(void);


void lineNoItemFree(void *item);


int lineNoItemDelete(xmlHashTablePtr breakPointHash,
                     breakPointPtr breakPtr);

int lineNoItemAdd(xmlHashTablePtr breakPointHash, breakPointPtr breakPtr);

/*-----------------------------------------------------------
       Breakpoint debugger functions
-----------------------------------------------------------*/


/* This is our major structure, it is a list of hash tables. Each 
 hash table has breakpoints with the same line number. A line
 number is used as an index into this list to get the right hash table.
 Then it is just a matter of a simple hash table lookup  */
arrayListPtr breakList;

/* keep track of what break point id we're up to*/
int breakPointCounter = 0;

/* What is the current breakpoint is only valid up to the start of 
 xsldbg command prompt. ie don't use it after deletion of breakpoints */
breakPointPtr activeBreakPointItem = NULL;


xmlHashTablePtr
lineNoItemNew(void)
{
    xmlHashTablePtr hash;

    hash = xmlHashCreate(4);

    return hash;
}


void lineNoItemFree(void *item)
{
    xmlHashTablePtr hash = (xmlHashTablePtr) item;

    if (item) {
#if 0
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Freeing breakpoint line hash"
                         " with %d elements \n", xmlHashSize(item));
#endif
#endif
        xmlHashFree(hash, breakPointItemFree);
    }
}


int lineNoItemDelete(xmlHashTablePtr breakPointHash, breakPointPtr breakPtr)
{
    int result = 0;

    if (breakPointHash && breakPtr) {
        if (xmlHashRemoveEntry(breakPointHash, breakPtr->url,
                               breakPointItemFree) == 0){
            result = 1;
	}else{
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
    xsltGenericError(xsltGenericErrorContext,"lineNoItemDelete failed");
#endif
	}

    }else {
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
    xsltGenericError(xsltGenericErrorContext, "lineNoItemDelete failed args %d %d", breakPointHash, breakPtr);
#endif
    }
    return result;
}


int lineNoItemAdd(xmlHashTablePtr breakPointHash, breakPointPtr breakPtr)
{
    int result = 0;

    if (breakPointHash && breakPtr) {
        if (xmlHashAddEntry(breakPointHash, breakPtr->url, breakPtr) == 0)
            result = 1;
    }
    return result;
}

xmlHashTablePtr breakPointGetLineNoHash(long lineNo)
{
    if (!breakList) {
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Breakpoints structures not initialized\n");
#endif
        return NULL;
    } else
        return (xmlHashTablePtr) arrayListGet(breakList, lineNo);
}


int breakPointInit(void)
{
    int result = 0;

    /* the average file has 395 lines of code so add 100 lines now */
    breakList = arrayListNew(100, lineNoItemFree);
    if (breakList) {
        /*
         * We don't need to do any thing else, as it is done when we add the 
         *    breakPoints
         */
        result = 1;
    } else {
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Unable to initialize breakPoints: memory error\n");
#endif
    }
    return result;
}


void breakPointFree(void)
{
    if (breakList)
        arrayListFree(breakList);
    breakList = NULL;
}


int breakPointEmpty(void)
{
    return arrayListEmpty(breakList);
}


breakPointPtr breakPointItemNew(void)
{
    breakPointPtr breakPtr = (breakPointPtr) xmlMalloc(sizeof(breakPoint));

    if (breakPtr) {
        breakPtr->url = NULL;
        breakPtr->lineNo = -1;
        breakPtr->templateName = NULL;
	breakPtr->modeName = NULL;
        breakPtr->flags = BREAKPOINT_ENABLED;
        breakPtr->id = ++breakPointCounter;
        breakPtr->type = DEBUG_BREAK_SOURCE;
    }
    return breakPtr;
}


void breakPointItemFree(void *payload, xmlChar * name)
{
    Q_UNUSED(name);
    if (payload) {
        breakPointPtr breakPtr = (breakPointPtr) payload;

        if (breakPtr->url)
            xmlFree(breakPtr->url);
        if (breakPtr->templateName)
            xmlFree(breakPtr->templateName);
	if (breakPtr->modeName)
	  xmlFree(breakPtr->modeName);
        xmlFree(breakPtr);
    }
}


breakPointPtr breakPointActiveBreakPoint(void)
{
    /* This function is depreciated */
    return NULL;                /* activeBreakPointItem; */
}



void breakPointSetActiveBreakPoint(breakPointPtr breakPtr)
{
    Q_UNUSED(breakPtr);
    /*
     * activeBreakPointItem = breakPtr;
     */

}


int breakPointAdd(const xmlChar * url, long lineNumber,
              const xmlChar * templateName, 
	      const xmlChar * modeName,
	      BreakPointTypeEnum type)
{
    int result = 0, breakPointType = type;
    xmlHashTablePtr breakPointHash = NULL;     /* hash of breakPoints */
    breakPointPtr breakPtr;

    if (!breakList) {
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Breakpoints structures not initialized\n");
#endif
        return result;
    }

    if (!url || (lineNumber == -1)) {
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Invalid url or line number to breakPointAdd\n");
#endif
        return result;
    }

    /* if breakpoint already exists then don;t add it */
    if (breakPointIsPresent(url, lineNumber)) {
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Warning: Breakpoint at file %s: line %d exists\n",
                         url, lineNumber);
#endif
        return result;
    }

    breakPtr = breakPointItemNew();
    if (breakPtr) {
        breakPtr->url = (xmlChar *) xmlMemStrdup((char *) url);
        breakPtr->lineNo = lineNumber;
        if (templateName)
            breakPtr->templateName =
                xmlStrdup( templateName);
        else
            breakPtr->templateName = NULL;
	if (modeName)
	    breakPtr->modeName = 
	      xmlStrdup(modeName);
	else
	  breakPtr->modeName = NULL;
        breakPtr->type = BreakPointTypeEnum(breakPointType);

        /* add new breakPoint to the right hash table */
        breakPointHash = breakPointGetLineNoHash(lineNumber);
        if (breakPointHash) {
            result = lineNoItemAdd(breakPointHash, breakPtr);
        } else {
            /* Grow breakList size */
            int lineIndex;
            int newEntries = breakList->count;
            xmlHashTablePtr hash;

            result = 1;
            if ((lineNumber < breakList->count) && breakList->count) {
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: Unable to find breakpoint line hash at %d\n",
                                 lineNumber);
#endif
            } else {
                if (breakList->count + newEntries < lineNumber)
                    newEntries = lineNumber - breakList->count + 1;


#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
                /*
                 * xsltGenericError(xsltGenericErrorContext,
                 * "Size of line list was %d adding %d entries\n",
                 * breakList->count, newEntries);
                 */
#endif
                lineIndex = 0;
                while ((lineIndex < newEntries) && result) {
                    hash = lineNoItemNew();
                    if (hash) {
                        result = result && arrayListAdd(breakList, hash);
                    } else {
                        result = 0;
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
                        xsltGenericError(xsltGenericErrorContext,
                                         "Error: Unable to create hash table breakPoint list: memory error\n");
#endif
                        return result;
                    }
                    lineIndex++;
                }
                /* find the newly added hashtable of breakpoints */
                breakPointHash = breakPointGetLineNoHash(lineNumber);
                if (breakPointHash) {
                    result = lineNoItemAdd(breakPointHash, breakPtr);
                } else {
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
                    xsltGenericError(xsltGenericErrorContext,
                                     "Error: Unable to create new breakPoint:interal error\n");
#endif
                    return result;
                }
            }

        }
    } else {
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Unable to create new breakPoint: memory error\n");
#endif
    }

    if (result && (optionsGetIntOption(OPTIONS_GDB) > 1) && 
	    (xsldbgValidateBreakpoints != BREAKPOINTS_BEING_VALIDATED)){
      breakPointPrint(breakPtr);
      xsldbgGenericErrorFunc("\n");
    }
    return result;
}


int breakPointDelete(breakPointPtr breakPtr)
{
    int result = 0;
    xmlHashTablePtr breakPointHash;     /* hash of breakPoints */

    if (!breakPtr)
        return result;

    breakPointHash = breakPointGetLineNoHash(breakPtr->lineNo);
    if (breakPointHash) {
        result = lineNoItemDelete(breakPointHash, breakPtr);
    } else {
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Breakpoint not found: xslDeleteBreakPoint\n");
#endif
    }
    return result;
}


int breakPointEnable(breakPointPtr breakPtr, int enable)
{
    int result = 0;

    if (breakPtr) {
	int enableFlag = 1;
        if (enable != XSL_TOGGLE_BREAKPOINT){
	    enableFlag = enable;
        }else {
	    if (breakPtr->flags & BREAKPOINT_ENABLED)
		enableFlag = 0;
        }
	if (enableFlag)
	    breakPtr->flags |= BREAKPOINT_ENABLED;
	else
	    breakPtr->flags = breakPtr->flags & (BREAKPOINT_ALLFLAGS ^ BREAKPOINT_ENABLED);
        result = 1;
    }
    return result;
}


int breakPointLinesCount(void)
{
    if (!breakList) {
#ifdef WITH_XSLDBG_DEBUG_BREAKPOINTS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Breakpoints structures not initialized\n");
#endif
        return 0;
    } else
        return arrayListCount(breakList);
}


arrayListPtr breakPointLineList(void)
{
    return breakList;
}


breakPointPtr breakPointGet(const xmlChar * url, long lineNumber)
{
    xmlHashTablePtr breakHash = breakPointGetLineNoHash(lineNumber);
    breakPointPtr breakPtr = NULL;

    if (!breakHash || !url)
        return breakPtr;

    breakPtr = (breakPointPtr)xmlHashLookup(breakHash, url);
    return breakPtr;
}


int breakPointPrint(breakPointPtr breakPtr)
{
    int result = 0;
    const char *breakStatusText[2] = {
	I18N_NOOP("disabled"),
	I18N_NOOP("enabled")
	};
    const char *breakTemplate="";
    const char *breakMode = "";
    const char *breakStatus;


    if (!breakPtr)
        return result;

    if (breakPtr->templateName){
	 if (breakPtr->modeName)
	     breakMode = (const char*)breakPtr->modeName;
	breakTemplate = (const char*)breakPtr->templateName;
    }
    

    breakStatus = breakStatusText[breakPtr->flags & BREAKPOINT_ENABLED];
    if (breakPtr->url)
	xsldbgGenericErrorFunc(QObject::tr("Breakpoint %1 %2 for template: \"%3\" mode: \"%4\" in file \"%5\" at line %6").arg(breakPtr->id).arg(breakStatus).arg(xsldbgText(breakTemplate)).arg(xsldbgText(breakMode)).arg(xsldbgUrl(breakPtr->url)).arg(breakPtr->lineNo));
    else
	xsldbgGenericErrorFunc(QObject::tr("Breakpoint %1 %2 for template: \"%3\" mode: \"%4\"").arg(breakPtr->id).arg(breakStatus).arg(xsldbgText(breakTemplate)).arg(xsldbgText(breakMode)));
    return ++result;
}


int breakPointIsPresent(const xmlChar * url, long lineNumber)
{
    int result = 0;

    if (!url || (lineNumber == -1))
        return result;

    result = (breakPointGet(url, lineNumber) != NULL);

    return result;
}


int breakPointIsPresentNode(xmlNodePtr node)
{
    int result = 0;

    if (!node || !node->doc)
        return result;

    if (xmlGetLineNo(node) == -1)
        return result;

    if (node->doc->URL) {
        result = breakPointIsPresent(node->doc->URL, xmlGetLineNo(node));
    }

    return result;
}
