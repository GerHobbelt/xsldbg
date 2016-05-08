
/***************************************************************************
                          options_unix.cpp  - unix specific code to work with options
                             -------------------
    begin                : Mon May 2 2016
    copyright            : (C) 2016 by Keith Isdale
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


#include <libxml/parser.h>
#include <stdlib.h>
#include <string.h>
#include "xsldbg.h"
#include "options.h"

int optionsPlatformInit(void)
{
    return 1;
}


void optionsPlatformFree(void)
{
    /* empty */
}

