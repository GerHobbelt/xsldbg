bin_PROGRAMS = xsldbg
xsldbg_SOURCES = \
	xsldbg.c 	\
	debugXSL.c 	\
	dbgbreakpoint.c	\
	dbgcallstack.c 	\
	dbgsearch.c	\
	dbgmain.c	\
	help.c	
xsldbg_LDADD   = 

EXTRA_DIST = \
	help.h help.c 		\
	debugXSL.c debugXSL.h 	\
	dbgbreakpoint.c		\
	dbgcallstack.c 		\
	dbgsearch.c		\
	dbgmain.c		\
	xsldbg.h

bin_PROGRAMS = xsldbg
# use of configure defined libraries
xsldbg_LDADD = $(XSLDBG_LIBS)

# set the include path found by configure
# cheet a bit because I need HELP_DOC_PATH defined
INCLUDES= $(all_includes) -DHELP_DOC_PATH=\"$(HTML_DIR)/xsldbg\" \
 -DVERSION=\"@VERSION@\" $(LIBXML_CFLAGS) $(LIBXSLT_CFLAGS)

# the library search path.
xsldbg_LDFLAGS = $(all_libraries)

clean:
	rm -f *.o

