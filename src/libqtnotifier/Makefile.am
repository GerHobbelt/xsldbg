libqtnotifier_la_LDFLAGS= -version-info @XSLDBG_VERSION_INFO@

if USE_XSLDBG_AS_THREAD

INCLUDES= -I $(QT_DIR)/include -I .. $(XSLDBG_CFLAGS)

libqtnotifier_la_SOURCES =\
	qtnotifier2.cpp        	\
	xsldbgnotifier.cpp  	\
	xsldbgevent.cpp	    	\
	xsldbgdebuggerbase.cpp	\
	xsldbgthread.cpp	

lib_LTLIBRARIES = libqtnotifier.la

xsldbgdebuggerbase.cpp: moc_xsldbgdebuggerbase.cpp
moc_xsldbgdebuggerbase.cpp: ../libxsldbg/xsldbgdebuggerbase.h
	$(QT_DIR)/bin/moc ../libxsldbg/xsldbgdebuggerbase.h -o moc_xsldbgdebuggerbase.cpp



else



endif


SOURCES = \
	qtnotifier2.cpp        	\
	xsldbgnotifier.cpp  	\
	xsldbgevent.cpp	    	\
	xsldbgdebuggerbase.cpp	\
	xsldbgthread.cpp
