# set the include path found by configure
# cheet a bit because I need HELP_DOC_PATH defined
INCLUDES= $(all_includes) -DHELP_DOC_PATH=\"$(HTML_DIR)/xsldbg\" \
 -DVERSION=\"@VERSION@\" $(LIBXML_CFLAGS) $(LIBXSLT_CFLAGS)

EXTRA_DIST = \
	help.c help.h 		\
	debugXSL.c debugXSL.h 	\
	dbgarraylist.c arraylist.h \
	dbgbreakpoint.c		\
	dbgcallstack.c 		\
	dbgsearch.c		\
	dbgmain.c		\
	files.c files.h		\
	options.c options.h	\
	xsldbg.h

bin_PROGRAMS = xsldbg
xsldbg_SOURCES = \
	xsldbg.c 	\
	files.c		\
	options.c	\
	debugXSL.c 	\
	dbgarraylist.c	\
	dbgbreakpoint.c	\
	dbgcallstack.c 	\
	dbgsearch.c	\
	dbgmain.c	\
	help.c	
xsldbg_LDADD   = 



# use of configure defined libraries
xsldbg_LDADD = $(XSLDBG_LIBS)

# the library search path.
xsldbg_LDFLAGS = $(all_libraries)


clean:
	rm -f *.o



