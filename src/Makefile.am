# set the include path found by configure
# cheet a bit because I need DOCS_PATH defined
INCLUDES= $(all_includes) -DDOCS_PATH=\"$(HTML_DIR)/xsldbg/\" \
 -DVERSION=\"@VERSION@\" $(LIBXML_CFLAGS) $(LIBXSLT_CFLAGS)

EXTRA_DIST = \
	help.c 			\
	help.h 			\
	debugXSL.c 		\
	debugXSL.h 		\
	dbgmain.c		\
	dbgarraylist.c		\
	arraylist.h		\
	dbgcallstack.c		\
	dbgsearch.c		\
	dbgbreakpoint.c	\
	breakpoint.h 		\
	breakpointInternals.h	\
	breakpoint_cmds.c	\
	template_cmds.c		\
	nodeview_cmds.c		\
	os_cmds.c		\
	param_cmds.c		\
	trace_cmds.c		\
	search_cmds.c		\
	files.c 		\
	files.h			\
	options.c 		\
	options.h		\
	xsldbg.h		\
	xslcallpoint.h		\
	xslsearch.h		\
	xslbreakpoint_2.h

bin_PROGRAMS = xsldbg
xsldbg_SOURCES = \
	xsldbg.c 	\
	files.c		\
	files.h		\
	options.c 	\
	options.h	\
	dbgmain.c	\
	dbgarraylist.c	\
	arraylist.h	\
	dbgcallstack.c	\
	dbgsearch.c		\
	dbgbreakpoint.c 	\
	breakpoint.h 		\
	breakpointInternals.h	\
	breakpoint_cmds.c 	\
	template_cmds.c	\
	nodeview_cmds.c	\
	os_cmds.c	\
	param_cmds.c	\
	trace_cmds.c	\
	search_cmds.c	\
	debugXSL.c 	\
	debugXSL.h	\
	help.c	
xsldbg_LDADD   = 



# use of configure defined libraries
xsldbg_LDADD = $(XSLDBG_LIBS)

# the library search path.
xsldbg_LDFLAGS = $(all_libraries)


clean:
	rm -f *.o

very-clean:clean
	rm -f *.*~
	rm -f *~
	rm -f Makefile.in
	rm -f Makefile

