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
	debug.h			\
	arraylist.h		\
	callstack.h		\
	breakpoint.h		\
	search.h		\
	files.h			\
	utils.h			\
	options.h		\
	xsldbg.h		\
	xsldbgconfig.h		\
	xsldbgnotifier.h	\
	xsldbgmsg.h		\
	xsldbgthread.h		\
	xsldbgio.h		\
	qtnotifier2.h		\
	xsldbgevent.h		\
	xsldbgdebuggerbase.h	\
	xsldbgwin32config.h	


# Each supported platform will have a small number of platform
#   specific files though since CYGWIN is realy *nix we use this for 
#   both CYGWIN and *nix
libxsldbg_la_SOURCES = \
	xsldbg.c 	\
	cmds.h		\
	debugXSL.c 	\
	arraylist.c	\
	callstack.c	\
	breakpoint.c 	\
	search.c	\
	debug.c		\
	files.c			\
	utils.c			\
	options.c 		\
	breakpoint_cmds.c 	\
	template_cmds.c		\
	nodeview_cmds.c		\
	os_cmds.c		\
	file_cmds.c		\
	param_cmds.c		\
	option_cmds.c		\
	trace_cmds.c		\
	search_cmds.c		\
	variable_cmds.c		\
	xsldbgthread.c		\
	options_unix.c		\
	files_unix.c		\
	help_unix.c	


SOURCES = \
	$(noinst_HEADERS)
	xsldbg.c 	\
	cmds.h		\
	debugXSL.c 	\
	arraylist.c	\
	callstack.c	\
	breakpoint.c 	\
	search.c	\
	debug.c		\
	files.c			\
	utils.c			\
	options.c 		\
	breakpoint_cmds.c 	\
	template_cmds.c		\
	nodeview_cmds.c		\
	os_cmds.c		\
	file_cmds.c		\
	param_cmds.c		\
	option_cmds.c		\
	trace_cmds.c		\
	search_cmds.c		\
	variable_cmds.c		\
	xsldbgthread.c		\
	options_unix.c		\
	files_unix.c		\
	help_unix.c		\
	options_riscos.c	\
	files_riscos.c		\
	help_riscos.c	

libxsldbg_la_LDFLAGS= -version-info @XSLDBG_VERSION_INFO@


# list win32 specific files here as well as other misc files
EXTRA_DIST=\
	readme.txt		\
	xsldbgwin32config.h.in	\
	xsldbgwin32config.h	\
	options_win32.c		\
	files_win32.c		\
	help_win32.c	


lib_LTLIBRARIES = libxsldbg.la

install-exec-hook:
	if test ! -d $(DESTDIR)/$(prefix)/include/libxsldbg ; then  $(mkinstalldirs) $(DESTDIR)/$(prefix)/include/libxsldbg ; fi
	$(INSTALL_DATA) $(noinst_HEADERS) $(DESTDIR)/$(prefix)/include/libxsldbg

clean:
	rm -f *.o

very-clean:clean
	rm -f *.*~
	rm -f *~
	rm -f Makefile.in
	rm -f Makefile

