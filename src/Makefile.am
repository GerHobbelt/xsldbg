# set the include path found by configure
# cheet a bit because I need HELP_DOC_PATH defined
INCLUDES= $(all_includes) -DHELP_DOC_PATH=\"$(HTML_DIR)/xsldbg\" \
 -DVERSION=\"@VERSION@\" $(LIBXML_CFLAGS) $(LIBXSLT_CFLAGS)

EXTRA_DIST = \
	help.c help.h 		\
	debugXSL.c debugXSL.h 	\
	breakpoint_cmds.c	\
	template_cmds.c		\
	nodeview_cmds.c		\
	os_cmds.c		\
	param_cmds.c		\
	trace_cmds.c		\
	search_cmds.c		\
	files.c files.h		\
	options.c options.h	\
	xsldbg.h

bin_PROGRAMS = xsldbg
xsldbg_SOURCES = \
	xsldbg.c 	\
	files.c		\
	options.c	\
	breakpoint_cmds.c \
	template_cmds.c	\
	nodeview_cmds.c	\
	os_cmds.c	\
	param_cmds.c	\
	trace_cmds.c	\
	search_cmds.c	\
	debugXSL.c 	\
	help.c	
xsldbg_LDADD   = 



# use of configure defined libraries
xsldbg_LDADD = $(XSLDBG_LIBS)

# the library search path.
xsldbg_LDFLAGS = $(all_libraries)


clean:
	rm -f *.o



