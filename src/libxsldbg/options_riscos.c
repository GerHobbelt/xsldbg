
/***************************************************************************
                          options_unix.c  -  riscos specific option functions
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

#include <libxml/parser.h>
#include <stdlib.h>
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


  /**
   * optionsConfigFileName:
   * 
   * Returns A copy of the file name that will be used to load xsldbgs
   *           configuration from,
   *         NULL otherwise
   */
xmlChar *
optionsConfigFileName(void)
{
    xmlChar *result;

    /* Find out if are are reading or writing the configuration
     * and then go from there. This is because RISC OS uses different
     * files for reading than writing */
    switch (optionsConfigState(OPTIONS_CONFIG_READVALUE)) {
        case OPTIONS_CONFIG_READING:
            result = xmlStrdup((xmlChar *)
                               unixfilename("Choices:XSLDebug.Options"));
            break;

        case OPTIONS_CONFIG_WRITTING:
            result = xmlStrdup((xmlChar *)
                               unixfilename
                               ("<Choices$Write>.XSLDebug.Options"));
            break;

        default:
            result = NULL;
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Internal error configuration not in correct state\n");
    }
    if (result)
        xsltGenericError(xsltGenericErrorContext,
                         "Options filename %d : Returning filename = %s\n",
                         optionsConfigState(OPTIONS_CONFIG_READVALUE),
                         result);

    return result;
}


  /**
   * optionsLoad:
   *
   * Load options from configuration file/registry
   *
   * This is a platform specific interface
   * 
   * Returns 1 if able to load options
   *         0 otherwise
   */
int
optionsLoad(void)
{
    int result = 0;
    xmlDocPtr doc;

    optionsConfigState(OPTIONS_CONFIG_READING);
    doc = xmlParseFile((char *) optionsConfigFileName());

    if (doc)
        result = optionsReadDoc(doc);

    optionsConfigState(OPTIONS_CONFIG_IDLE);
    return 0;
}


  /**
   * optionsSave:
   *
   * Save options to configuration file/registry
   *
   * This is a platform specific interface
   * 
   * Returns 1 if able to save options
   *         0 otherwise
   */
int
optionsSave(void)
{
    int result;

    optionsConfigState(OPTIONS_CONFIG_WRITTING);
    result = optionsSavetoFile(optionsConfigFileName());
    optionsConfigState(OPTIONS_CONFIG_IDLE);

    return result;
}
