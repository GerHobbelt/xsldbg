
/* *************************************************************************
                          xslcallpoint.h  -  public functions for the
                                               the call stack
                             -------------------
    begin                : Fri Dec 7 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ************************************************************************* */

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ************************************************************************* */

#ifndef XSLCALLPOINT_H
#define XSLCALLPOINT_H

/**
 * Provide a call stack support
 *
 * @short call stack support
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */

#ifndef BUILD_DOCS
#include "xslbreakpoint.h"
#endif

#ifdef WITH_XSLT_DEBUG
#ifndef WITH_XSLT_DEBUG_BREAKPOINTS
#define WITH_XSLT_DEBUG_BREAKPOINTS
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif



    typedef struct _xslCallPointInfo xslCallPointInfo;
    typedef xslCallPointInfo *xslCallPointInfoPtr;
    struct _xslCallPointInfo {
        xmlChar *templateName;
        xmlChar *url;
        xslCallPointInfoPtr next;
    };

    typedef struct _xslCallPoint xslCallPoint;
    typedef xslCallPoint *xslCallPointPtr;
    struct _xslCallPoint {
        xslCallPointInfoPtr info;
        long lineNo;
        xslCallPointPtr next;
    };


#ifdef USE_GNOME_DOCS
/**
 * callStackInit:
 *
 * Returns If callStack has been initialized properly and all
 *               memory required has been obtained,
 *         0 otherwise
 *
 * Returns 1 if callStack has been initialized properly and all
 *               memory required has been obtained,
 *         0 otherwise
*/
#endif
int
callStackInit(void);


#ifdef USE_GNOME_DOCS
/**
 * callStackFree:
 *
 *
 * Free all memory used by callStack
 */
#endif
void
callStackFree(void);




#ifdef USE_GNOME_DOCS

/**
 * addCallInfo:
 * @templatename: Template name to add
 * @url: The url for the template
 *
 * Add template "call" to call stack
 *
 * Returns A reference to the added info if successful, 
 *          NULL otherwise
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Add template "call" to call stack
 *
 * @returns A reference to the added info if successful, 
 *          NULL otherwise
 *
 * @param templateName Template name to add
 * @param url The url for the template
 */
#endif
#endif
    xslCallPointInfoPtr addCallInfo(const xmlChar * templateName,
                                    const xmlChar * url);



#ifdef USE_GNOME_DOCS

/**
 * addCall:
 * @templ: The current template being applied
 * @source: The source node being processed
 *
 * Add template "call" to call stack
 *
 * Returns 1 on success, 
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

  /**
   * Add template "call" to call stack
   *
   * @param templ The current template being applied 
   * @param source The source node being processed
   *
   * @returns 1 on success, 
   *          0 otherwise
   */
#endif
#endif
    int addCall(xsltTemplatePtr templ, xmlNodePtr source);


#ifdef USE_GNOME_DOCS

/**
 * dropCall:
 *
 * Drop the topmost item off the call stack
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Drop the topmost item off the call stack
 */
#endif
#endif
    void dropCall(void);


#ifdef USE_GNOME_DOCS

/** 
 * stepupToDepth:
 * @depth:The frame depth to step up to  
 *             0 < @depth <= callDepth()
 *
 * Set the frame depth to step up to
 *
 * Returns 1 on success,
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/** 
 * Set the frame depth to step up to
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param depth The frame depth to step up to  
 *            0 < @p depth <= callDepth()
 */
#endif
#endif
    int stepupToDepth(int depth);


#ifdef USE_GNOME_DOCS

/** 
 * stepdownToDepth:
 * @depth: The frame depth to step down to, 
 *             0 < @depth <= callDepth()
 *
 * Set the frame depth to step down to
 *
 * Returns 1 on success, 
 *         0 otherwise
 */
#else
#ifdef USE_KDE_DOCS

/** 
 * Set the frame depth to step down to
 *
 * @returns 1 on success, 
 *          0 otherwise
 *
 * @param depth The frame depth to step down to  
 *            0 < @p depth <= callDepth()
 */
#endif
#endif
    int stepdownToDepth(int depth);


#ifdef USE_GNOME_DOCS

/**
 * getCall:
 * @depth: 0 < @depth <= callDepth()
 *
 * Retrieve the call point at specified call depth 

 * Returns Non-null a if depth is valid,
 *         NULL otherwise 
 */
#else
#ifdef USE_KDE_DOCS

/**
 * Retrieve the call point at specified call depth 

 * @returns Non-null a if depth is valid,
 *          NULL otherwise 
 *
 * @param depth 0 < @p depth <= callDepth()
 */
#endif
#endif
    xslCallPointPtr getCall(int depth);


#ifdef USE_GNOME_DOCS

/** 
 * getCallStackTop:
 *
 * Get the top item in the call stack
 *
 * Returns the top of the call stack
 */
#else
#ifdef USE_KDE_DOCS

/**  
 * Get the top item in the call stack
 *
 * @returns The top of the call stack
 */
#endif
#endif
    xslCallPointPtr getCallStackTop(void);


#ifdef USE_GNOME_DOCS

/**
 * callDepth:
 *
 * Return the depth of call stack
 *
 * Returns the depth of call stack
 */
#else
#ifdef USE_KDE_DOCS

/**
 * @returns the depth of call stack
 */
#endif
#endif
    int callDepth(void);

#ifdef __cplusplus
}
#endif
#endif
