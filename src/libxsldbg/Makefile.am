# set the include path found by configure
# cheet a bit because I need DOCS_PATH defined
if USE_XSLDBG_AS_THREAD
INCLUDES= $(all_includes) -DDOCS_PATH=\"$(HTML_DIR)/xsldbg/\" \
 -DVERSION=\"@VERSION@\" -I .. $(LIBXML_CFLAGS) $(LIBXSLT_CFLAGS) -I $(QTDIR)/include
else
INCLUDES= $(all_includes) -DDOCS_PATH=\"$(HTML_DIR)/xsldbg/\" \
 -DVERSION=\"@VERSION@\" -I .. $(LIBXML_CFLAGS) $(LIBXSLT_CFLAGS)
endif

noinst_HEADERS=\
	help.h 			\
	cmds.h			\
	debugXSL.h 		\
	arraylist.h		\
	xslcallpoint.h		\
	xslbreakpoint.h		\
	xslsearch.h		\
	files.h			\
	options.h		\
	xsldbg.h		\
	xsldbgconfig.h		\
	xsldbgnotifier.h	\
	xsldbgmsg.h		\
	xsldbgthread.h		\
	xsldbgio.h	


# Each supported platform will have a small number of platform
#   specific files
if RISC_OS
libxsldbg_la_SOURCES = \
	xsldbg.c 	\
	cmds.h		\
	debugXSL.c 	\
	dbgarraylist.c	\
	dbgcallstack.c	\
	dbgbreakpoint.c \
	dbgsearch.c	\
	files.c			\
	options.c 		\
	dbgmain.c		\
	breakpoint_cmds.c 	\
	template_cmds.c		\
	nodeview_cmds.c		\
	os_cmds.c		\
	file_cmds.c		\
	param_cmds.c		\
	trace_cmds.c		\
	search_cmds.c		\
	variable_cmds.c		\
	xsldbgthread.c		\
	options_riscos.c	\
	files_riscos.c		\
	help_riscos.c		

else
libxsldbg_la_SOURCES = \
	xsldbg.c 	\
	debugXSL.c 	\
	dbgarraylist.c	\
	dbgcallstack.c	\
	dbgbreakpoint.c \
	dbgsearch.c	\
	files.c		\
	options.c 	\
	dbgmain.c	\
	breakpoint_cmds.c 	\
	template_cmds.c		\
	nodeview_cmds.c		\
	os_cmds.c		\
	file_cmds.c		\
	param_cmds.c		\
	trace_cmds.c		\
	search_cmds.c		\
	variable_cmds.c		\
	xsldbgthread.c		\
	options_unix.c		\
	files_unix.c		\
	help_unix.c	
endif


SOURCES = \
	$(noinst_HEADERS)
	help.c		\
	xsldbg.c 	\
	debugXSL.c 	\
	dbgarraylist.c	\
	dbgcallstack.c	\
	dbgbreakpoint.c \
	dbgsearch.c	\
	files.c		\
	options.c 	\
	dbgmain.c	\
	breakpoint_cmds.c 	\
	template_cmds.c		\
	nodeview_cmds.c		\
	os_cmds.c		\
	file_cmds.c		\
	param_cmds.c		\
	trace_cmds.c		\
	search_cmds.c		\
	variable_cmds.c		\
	xsldbgthread.h		\
	xsldbgthread.c		\
	options_unix.c		\
	files_unix.c		\
	help_unix.c		\
	options_riscos.c	\
	files_riscos.c		\
	help_riscos.c	



EXTRA_DIST=\
	readme.txt



lib_LTLIBRARIES = libxsldbg.la

install-exec-hook:
	if test ! -d $(prefix)/include/libxsldbg ; then  $(mkinstalldirs) $(prefix)/include/libxsldbg ; fi
	$(INSTALL_DATA) $(noinst_HEADERS) $(prefix)/include/libxsldbg

clean:
	rm -f *.o

very-clean:clean
	rm -f *.*~
	rm -f *~
	rm -f Makefile.in
	rm -f Makefile

