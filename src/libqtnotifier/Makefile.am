libqtnotifier_la_LDFLAGS= -version-info @XSLDBG_VERSION_INFO@

if USE_XSLDBG_AS_THREAD

INCLUDES= -I $(QTDIR)/include -I .. $(XSLDBG_CFLAGS)

libqtnotifier_la_SOURCES =\
	qtnotifier2.cpp        	\
	xsldbgnotifier.cpp  	\
	xsldbgevent.cpp	    	\
	xsldbgdebuggerbase.cpp	\
	xsldbgthread.cpp	\
	moc_xsldbgdebuggerbase.cpp 

lib_LTLIBRARIES = libqtnotifier.la

libqtnotifier_la_LIBADD =
libqtnotifier_la_DEPENDENCIES=

xsldbgdebuggerbase.cpp: moc_xsldbgdebuggerbase.cpp
moc_xsldbgdebuggerbase.cpp: ../libxsldbg/xsldbgdebuggerbase.h
	$(QTDIR)/bin/moc ../libxsldbg/xsldbgdebuggerbase.h -o moc_xsldbgdebuggerbase.cpp


install-exec-hook:
	if test ! -d $(prefix)/include/libxsldbg ; then  $(mkinstalldirs) $(prefix)/include/libxsldbg ; fi
	$(INSTALL_DATA) $(noinst_HEADERS) $(prefix)/include/libxsldbg
else

install-exec-hook:

endif


SOURCES = \
	qtnotifier2.cpp        	\
	xsldbgnotifier.cpp  	\
	xsldbgevent.cpp	    	\
	xsldbgdebuggerbase.cpp	\
	xsldbgthread.cpp

