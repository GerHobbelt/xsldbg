# Generated automatically from Makefile.in by configure.
# Makefile.in generated automatically by automake 1.4 from Makefile.am

# Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

# we have no targets just the defintion of the distribution files for win32 when
# using  VC6.0


SHELL = /bin/sh

srcdir = .
top_srcdir = ../..
prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/xsldbg
pkglibdir = $(libdir)/xsldbg
pkgincludedir = $(includedir)/xsldbg

top_builddir = ../..

ACLOCAL = aclocal
AUTOCONF = autoconf
AUTOMAKE = automake
AUTOHEADER = autoheader

INSTALL = /usr/bin/ginstall -c
INSTALL_PROGRAM = ${INSTALL} $(AM_INSTALL_PROGRAM_FLAGS)
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL_PROGRAM}
transform = s,x,x,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
host_alias = i686-pc-linux
host_triplet = i686-pc-linux-gnu
AS = @AS@
CC = gcc
CFLAGS = -g -O2
CPPFLAGS = 
CXX = c++
DLLTOOL = @DLLTOOL@
DOCBOOK_PREFIX = /usr/share/xml/docbook/
DOCBOOK_VERSION = 4.1.2
EXTRA_LIBS = 
HAVE_GNOME = 0
HAVE_PTHREAD = 1
HAVE_QT = 1
HTML_DIR = $(prefix)/doc
LDFLAGS = 
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LIBXML_CFLAGS = -I/usr/include/libxml2 -I/usr/include/libxml2/libxml
LIBXML_LIBS = -L/usr/lib -lxml2 -lz -lm -L/usr/X11R6/lib
LIBXML_PREFIX = xml2-config --prefix
LIBXML_REQUIRED_VERSION = 2.4.13
LIBXSLT_CFLAGS = -I/usr/include/libxml2 -I/usr/include/libxml2/libxml
LIBXSLT_LIBS = -L/usr/lib -L/usr/X11R6/lib -lxslt -lxml2 -lz -lm
LIBXSLT_PREFIX = xslt-config --prefix
LIBXSLT_REQUIRED_VERSION = 1.0.10
LN_S = ln -s
MAINT = 
MAKEINFO = makeinfo
OBJDUMP = @OBJDUMP@
PACKAGE = xsldbg
PERL = perl
RANLIB = ranlib
THREAD_LIBS =  -lpthread 
TIMESTAMP = 
USE_XSLDBG_AS_THREAD = 1
VERSION = 3.0.5
WITH_MEM_DEBUG = 0
XML_CONFIG = xml2-config
XSLDBG_CFLAGS = -I/usr/local/include -I/usr/include/libxml2 -I/usr/include/libxml2/libxml 
XSLDBG_INCLUDEDIR = -I/usr/local/include
XSLDBG_LIBDIR = /usr/local/lib
XSLDBG_LIBS =  -L/usr/local/lib -lxsldbg -L/usr/lib -L/usr/X11R6/lib -lxslt -lxml2 -lz -lm -lexslt -lreadline -lncurses -lhistory
XSLDBG_MAJOR_VERSION = 3
XSLDBG_MICRO_VERSION = 5
XSLDBG_MINOR_VERSION = 0
XSLDBG_STATIC_FLAGS =  -static
XSLDBG_TEST_LIBS =  -L/usr/lib -L/usr/X11R6/lib -lxslt -lxml2 -lz -lm -lexslt -lreadline -lncurses -lhistory
XSLDBG_VERSION = 3.0.5
XSLDBG_VERSION_INFO = 3:5:0
XSLDBG_VERSION_NUMBER = 30005
XSLT_CONFIG = xslt-config

EXTRA_DIST =  xsldbg.dsp 		libqtnotifier_so.dsp 	libxsldbg.dsp 		xsldbg.dsw 		createcatalog.bat

mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = ../../config.h
CONFIG_CLEAN_FILES = 
DIST_COMMON =  Makefile.am Makefile.in


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = tar
GZIP_ENV = --best
all: all-redirect
.SUFFIXES:
$(srcdir)/Makefile.in:  Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --gnu win32/dsp_6_0/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status $(BUILT_SOURCES)
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status

tags: TAGS
TAGS:


distdir = $(top_builddir)/$(PACKAGE)-$(VERSION)/$(subdir)

subdir = win32/dsp_6_0

distdir: $(DISTFILES)
	here=`cd $(top_builddir) && pwd`; \
	top_distdir=`cd $(top_distdir) && pwd`; \
	distdir=`cd $(distdir) && pwd`; \
	cd $(top_srcdir) \
	  && $(AUTOMAKE) --include-deps --build-dir=$$here --srcdir-name=$(top_srcdir) --output-dir=$$top_distdir --gnu win32/dsp_6_0/Makefile
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -pr $$/$$file $(distdir)/$$file; \
	  else \
	    test -f $(distdir)/$$file \
	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	    || cp -p $$d/$$file $(distdir)/$$file || :; \
	  fi; \
	done
info-am:
info: info-am
dvi-am:
dvi: dvi-am
check-am: all-am
check: check-am
installcheck-am:
installcheck: installcheck-am
install-exec-am:
install-exec: install-exec-am

install-data-am:
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am:
uninstall: uninstall-am
all-am: Makefile
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs:


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean-am:  mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-generic mostlyclean-am

clean: clean-am

distclean-am:  distclean-generic clean-am
	-rm -f libtool

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-generic distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: tags distdir info-am info dvi-am dvi check check-am \
installcheck-am installcheck install-exec-am install-exec \
install-data-am install-data install-am install uninstall-am uninstall \
all-redirect all-am all installdirs mostlyclean-generic \
distclean-generic clean-generic maintainer-clean-generic clean \
mostlyclean distclean maintainer-clean


all:

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
