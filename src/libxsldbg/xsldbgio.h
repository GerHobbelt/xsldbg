
#ifndef XSLDBGIO_H
#define XSLDBGIO_H

#include <libxml/tree.h>        /* needed for definition of xmlChar */

/**
 * xslShellReadline:
 * @prompt:  the prompt value
 *
 * Read a string
 *
 * Returns a copy of the text inputed or NULL if EOF in stdin found. 
 *    The caller is expected to free the returned string.
 */
xmlChar *xslDbgShellReadline(xmlChar * prompt);


#define DEBUG_BUFFER_SIZE 500   /*used by xslDbgShell */


#endif
