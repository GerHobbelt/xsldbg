
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


#include "xslbreakpoint.h"

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


/**
 * @returns A reference to the added info if successful, 
 *          NULL otherwise
 *
 * @param templateName Template name to add
 * @param url The url for the template
 */
    xslCallPointInfoPtr xslAddCallInfo(const xmlChar * templateName,
                                       const xmlChar * url);


/**
 * Drop the topmost item off the call stack
 */
    void xslDropCall(void);


/** 
 * Set the frame depth to step up to
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param depth The frame depth to step up to  
 *            0 < @p depth <= xslCallDepth()
 */
    int xslStepupToDepth(int depth);


/** 
 * Set the frame depth to step down to
 *
 * @returns 1 on success, 
 *          0 otherwise
 *
 * @param depth The frame depth to step down to  
 *            0 < @p depth <= xslCallDepth()
 */
    int xslStepdownToDepth(int depth);


/**
 * Retrieve the call point at specified call depth 

 * @returns Non-null a if depth is valid,
 *          NULL otherwise 
 *
 * @param depth 0 < @p depth <= xslCallDepth()
 */
    xslCallPointPtr xslGetCall(int depth);


/** 
 * @returns The top of the call stack
 */
    xslCallPointPtr xslGetCallStackTop(void);


/**
 * @returns the depth of call stack
 */
    int xslCallDepth(void);

#ifdef __cplusplus
}
#endif
#endif
