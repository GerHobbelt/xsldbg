
/***************************************************************************
                          xsldbgwin32.h  - make win32 specific definitions
                             -------------------
    begin                : Sun Dec 16 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef XSLDBG_WIN32CONFIG_H
#define XSLDBG_WIN32CONFIG_H

#define XSLDBG_BIN "xsldbg"

#define VERSION "2.1.6"

#define TIMESTAMP __DATE__

#ifndef WITH_DEBUGGER
#define WITH_DEBUGGER
#endif

#ifndef WITH_DEBUG_HELP
#define WITH_DEBUG_HELP
#endif

#if defined _MSC_VER
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#endif


#endif