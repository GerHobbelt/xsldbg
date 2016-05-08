
#ifndef XSLDEBUGGER_H
#define XSLDEBUGGER_H
//##TODO
// #include <config-kxsldbg.h>

/* We want skip most of these includes when building documentation */
#ifndef __riscos
#  ifndef WIN32
      /* we don't need to use dll declares for *nix based machines */
#     define XSLDBG_SO_API
#  else
      /* dll declares get defined in xsldbgwin32config.h *nix based machines */
#    include "xsldbgwin32config.h"
#  endif
#else
   /* we don't need to use dll declares for risc_os*/
#  define XSLDBG_SO_API
#  include "config_riscos.h"
#  include "libxml/riscos.h"
#endif

#include <libxslt/xsltconfig.h>


#if !defined (FORCE_DEBUGGER) &&  (LIBXSLT_VERSION > 10019)
#  ifndef WITH_XSLT_DEBUGGER
#     ifndef WITH_DEBUGGER
#       error "WITH_DEBUGGER MACRO not defined in libxslt maybe you've disable debugger in libxslt." \
                "if your are sure then disable this check by defining WITH_FORCE_DEBUGGER. eg.\n" \
                 "\nmake CFLAGS=\"$CFLAGS -D FORCE_DEBUGGER\""
#     endif
#  endif
#endif

#ifdef  WITH_XSLDBG_DEBUG

#ifndef WITH_XSLDBG_DEBUG_PROCESS
#define WITH_XSLDBG_DEBUG_PROCESS
#endif

#ifndef WITH_XSLDBG_DEBUG_BREAKPOINTS
#define WITH_XSLDBG_DEBUG_BREAKPOINTS
#endif

#endif /* end of WITH_XSL_DEBUG */


#include <libxslt/xslt.h>
#include <libexslt/exslt.h>
#include <libxslt/xsltutils.h>

#include <QString>

#include "breakpoint.h"


/**
 * Load the stylesheet and return it
 *
 * @returns The stylesheet after reloading it if successful
 *         NULL otherwise
 */
xsltStylesheetPtr xsldbgLoadStylesheet(void);


/**
 * Load the xml data file and return it
 *
 * @returns The stylesheet after reloading it if successful
 *         NULL otherwise
 */
xmlDocPtr xsldbgLoadXmlData(void);


extern "C" {

/**
 * Load the temporary data file and return it
 *
 * @returns The temporary file after reloading it if successful,
 *         NULL otherwise
 * @param path The name of temporary file to loa
 */
xmlDocPtr xsldbgLoadXmlTemporary(const xmlChar * path);

void xsldbgGenericErrorFunc(void *ctx, const char *msg, ...)
#ifdef __GNUC__
__attribute__ ( ( format ( printf, 2, 3 ) ) )
#endif
;
}

int xsldbgMain(int argc, char **argv);


void xsldbgGenericErrorFunc(QString const &text);
QString xsldbgUrl(const char *utf8Url); 
QString xsldbgUrl(const xmlChar *utf8Url);
QString xsldbgText(const char *utf8Text);
QString xsldbgText(const xmlChar *utf8Text);

#ifndef KDE
    #define I18N_NOOP(a) a
    #define i18n(a) QObject::tr(a)
    #include <QObject> 
#endif
#define XSLDBG_BIN "xsldbg"
#define TIMESTAMP __DATE__

#endif // XSLDEBUGGER_H
