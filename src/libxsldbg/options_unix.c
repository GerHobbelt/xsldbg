/***************************************************************************
                          options_unix.c  -  *nix specific option functions
                             -------------------
    begin                : Tue Jan 29 2002
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

#include "stdlib.h"
#include "xsldbg.h"
#include "options.h"

  /**
   * optionsConfigFileName:
   * 
   * Returns A copy of the file name that will be used to load xsldbgs
   *           configuration from,
   *         NULL otherwise
   */
  xmlChar* optionsConfigFileName(void)
{
  xmlChar *result = NULL;
  const char *homeDir = getenv("HOME");
  const char *configName = "xsldbg.rc";
  int bufferSize = 0;
  if (homeDir){
    /* give ourselves a bit of room to move */
    bufferSize = strlen(homeDir) + strlen(configName) + 10;
    result = (xmlChar*)xmlMalloc(bufferSize);
    snprintf((char*)result, bufferSize, "%s/%s", homeDir, configName);
  }
  return result;
}

  /**
   * optionsLoad:
   *
   * Load options from configuation file/registry
   *
   * This is a platform specific interface
   * 
   * Returns 1 if able to load options
   *         0 otherwise
   */
int optionsLoad(void)
{
  return 0;
}
