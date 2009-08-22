
/**
 *
 *  This file is part of the kdewebdev package
 *  Copyright (c) 2002 Keith Isdale <keith@kdewebdev.org>
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
