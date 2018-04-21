
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

#ifndef XSLDBG_HELP_H
#define XSLDBG_HELP_H


/**
 * Display help about the command in @p arg
 *
 *
 *  This is a platform specific interface
 *
 * @param args Is valid or empty string
 * @returns 1 on success,
 *          0 otherwise
 */
int helpTop(const xmlChar * args);


#endif
