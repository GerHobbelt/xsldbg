libqtnotifier_la_LDFLAGS= -version-info @XSLDBG_VERSION_INFO@

if USE_XSLDBG_AS_THREAD

INCLUDES= -I $(QTDIR)/include -I .. $(XSLDBG_CFLAGS)

libqtnotifier_la_SOURCES =\
	qtnotifier2.cpp        	\
	xsldbgnotifier.cpp  	\
	xsldbgevent.cpp	    	\
	xsldbgdebuggerbase.cpp	\
	xsldbgthread.cpp			


lib_LTLIBRARIES = libqtnotifier.la

libqtnotifier_la_LIBADD =
libqtnotifier_la_DEPENDENCIES=xsldbgdebuggerbase.moc

xsldbgdebuggerbase.cpp: xsldbgdebuggerbase.moc
xsldbgdebuggerbase.moc: xsldbgdebuggerbase.h
	$(QTDIR)/bin/moc xsldbgdebuggerbase.h -o xsldbgdebuggerbase.moc


install-exec-hook:
	if test ! -d $(prefix)/include/libxsldbg ; then  $(mkinstalldirs) $(prefix)/include/libxsldbg ; fi
	$(INSTALL_DATA) $(noinst_HEADERS) $(prefix)/include/libxsldbg
else

install-exec-hook:

endif


noinst_HEADERS = \
	qtnotifier2.h		\
	xsldbgevent.h		\
	xsldbgdebuggerbase.h	

SOURCES = \
	qtnotifier2.cpp        	\
	xsldbgnotifier.cpp  	\
	xsldbgevent.cpp	    	\
	xsldbgdebuggerbase.cpp	\
	xsldbgthread.cpp	\
	$(noinst_HEADERS)

