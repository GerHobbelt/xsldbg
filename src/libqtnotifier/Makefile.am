INCLUDES= -I $(QTDIR)/include -I ..

libqtnotifier_la_SOURCES =\
	qtnotifier.cpp        \
	xsldbgnotifier.cpp		\
	xsldbgthread.cpp			


EXTRA_DIST= \
	qtnotifier.cpp        \
	qtnotifier.h					\
	xsldbgnotifier.cpp		\
	xsldbgthread.cpp	


lib_LTLIBRARIES = libqtnotifier.la

libqtnotifier_la_LIBADD =
libqtnotifier_la_DEPENDENCIES=qtnotifier.moc

qtnotifier.moc :  qtnotifier.h
	$(QTDIR)/bin/moc -o qtnotifier.moc qtnotifier.h


install-exec-hook:
	if test ! -d $(prefix)/include/libxsldbg ; then  $(mkinstalldirs) $(prefix)/include/libxsldbg ; fi
	$(INSTALL_DATA) qtnotifier.h $(prefix)/include/libxsldbg

