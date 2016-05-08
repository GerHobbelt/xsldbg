
/***************************************************************************
                          options_win32.c  -  win32 specific option functions
                             -------------------
    begin                : Tue Jan 29 2002
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
#include "stdlib.h"
#include <string.h>
#include "xsldbg.h"
#include "options.h"

  /**
   * optionsPlatformInit:
   *
   * Intialize the platform specific options module
   *
   *  This is a platform specific interface
   *
   * Returns 1 if sucessful
   *         0 otherwise  
   */
int
optionsPlatformInit(void)
{
    return 1;
}


  /**
   * optionsPlatformFree:
   *
   * Free memory used by the platform specific options module
   *
   *  This is a platform specific interface
   *
   */
void
optionsPlatformFree(void)
{
    /* empty */
}
