if USE_XSLDBG_AS_THREAD

INCLUDES= -I $(QTDIR)/include -I .. $(XSLDBG_CFLAGS)

libqtnotifier_la_SOURCES =\
	qtnotifier.cpp        \
	xsldbgnotifier.cpp		\
	xsldbgthread.cpp			

noinst_HEADERS = \
	qtnotifier.h

SOURCES = \
	$(libqtnotifier_la_SOURCES) \
	$(noinst_HEADERS)



lib_LTLIBRARIES = libqtnotifier.la

libqtnotifier_la_LIBADD =
libqtnotifier_la_DEPENDENCIES=qtnotifier.moc

qtnotifier.cpp : qtnotifier.moc
qtnotifier.moc : qtnotifier.h
	$(QTDIR)/bin/moc -o qtnotifier.moc qtnotifier.h


install-exec-hook:
	if test ! -d $(prefix)/include/libxsldbg ; then  $(mkinstalldirs) $(prefix)/include/libxsldbg ; fi
	$(INSTALL_DATA) $(noinst_HEADERS) $(prefix)/include/libxsldbg
else

install-exec-hook:

endif

