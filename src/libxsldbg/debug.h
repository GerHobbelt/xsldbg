
/***************************************************************************
    begin                : Sat Dec 22 2001
    copyright            : (C) 2001-2018 by Keith Isdale
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

#ifndef XSL_DEBUG_H
#define XSL_DEBUG_H


/**
 * Initialize debugger allocating any memory needed by debugger
 *
 * @returns 1 on success,
 *          0 otherwise
 */
int debugInit(void);


/**
 * Free up any memory taken by debugger
 */
void debugFree(void);


/**
 * Set flag that debuger has received control to value of @p reached
 *
 * @returns 1 if any breakpoint was reached previously,
 *          0 otherwise
 *
 * @param reached 1 if debugger has received control, -1 to read its value 
 *              or 0 to clear the flag
 */
int debugGotControl(int reached);

#endif
