# Makefile.in generated automatically by automake 1.5 from Makefile.am.

# Copyright 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001
# Free Software Foundation, Inc.
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
pkgdatadir = $(datadir)/xsldbg
pkglibdir = $(libdir)/xsldbg
pkgincludedir = $(includedir)/xsldbg
top_builddir = ../..

ACLOCAL = ${SHELL} /home/keith/xsldbg/missing --run aclocal
AUTOCONF = ${SHELL} /home/keith/xsldbg/missing --run autoconf
AUTOMAKE = ${SHELL} /home/keith/xsldbg/missing --run automake
AUTOHEADER = ${SHELL} /home/keith/xsldbg/missing --run autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
INSTALL_HEADER = $(INSTALL_DATA)
transform = s,x,x,
NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
host_alias = i686-suse-linux
host_triplet = i686-suse-linux
AMTAR = ${SHELL} /home/keith/xsldbg/missing --run tar
AS = @AS@
AWK = gawk
CC = gcc
CFLAGS = -g -O2
CPPFLAGS = 
CXX = g++
DEPDIR = .deps
DLLTOOL = @DLLTOOL@
DOCBOOK_PREFIX = /usr/share/xml/docbook/
DOCBOOK_VERSION = 4.1.2
ECHO = echo
EXEEXT = 
EXTRA_LIBS = 
HAVE_GNOME = 0
HAVE_PTHREAD = 1
HAVE_QT = 1
HTML_DIR = $(prefix)/share/packages
INSTALL_STRIP_PROGRAM = ${SHELL} $(install_sh) -c -s
KDEDOC_PREFIX = /opt/kde3
LDFLAGS = 
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LIBXML_CFLAGS = -I/usr/include/libxml2
LIBXML_LIBS = -L/usr/lib -lxml2 -lz -lm
LIBXML_PREFIX = xml2-config --prefix
LIBXML_REQUIRED_VERSION = 2.4.13
LIBXSLT_CFLAGS = -I/usr/include/libxml2
LIBXSLT_LIBS = -L/usr/lib -lxslt -lxml2 -lz -lm
LIBXSLT_PREFIX = xslt-config --prefix
LIBXSLT_REQUIRED_VERSION = 1.0.10
LN_S = ln -s
MAINT = #
OBJDUMP = @OBJDUMP@
OBJEXT = o
PACKAGE = xsldbg
PERL = perl
RANLIB = ranlib
STRIP = strip
THREAD_LIBS =  -lpthread 
TIMESTAMP = 
USE_XSLDBG_AS_THREAD = 1
VERSION = 3.1.2
WITH_MEM_DEBUG = 0
XML_CONFIG = xml2-config
XSLDBG_CFLAGS = -I/usr/local/include -I/usr/include/libxml2 
XSLDBG_INCLUDEDIR = -I/usr/local/include
XSLDBG_LIBDIR = /usr/local/lib
XSLDBG_LIBS =  -L/usr/local/lib -lxsldbg -L/usr/lib -lxslt -lxml2 -lz -lm -lexslt -lreadline -lncurses -lhistory
XSLDBG_MAJOR_VERSION = 3
XSLDBG_MICRO_VERSION = 2
XSLDBG_MINOR_VERSION = 1
XSLDBG_STATIC_FLAGS =  -static
XSLDBG_TEST_LIBS =  -L/usr/lib -lxslt -lxml2 -lz -lm -lexslt -lreadline -lncurses -lhistory
XSLDBG_VERSION = 3.1.2
XSLDBG_VERSION_INFO = 4:2:1
XSLDBG_VERSION_NUMBER = 30102
XSLT_CONFIG = xslt-config
am__include = include
am__quote = 
install_sh = /home/keith/xsldbg/install-sh

EXTRA_DIST = \
xsldbg.dsp 		\
libqtnotifier_so.dsp 	\
libxsldbg.dsp 		\
xsldbg.dsw 		\
createcatalog.bat

subdir = win32/dsp_6_0
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = $(top_builddir)/config.h
CONFIG_CLEAN_FILES =
DIST_SOURCES =
DIST_COMMON = Makefile.am Makefile.in
all: all-am

.SUFFIXES:

mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs

distclean-libtool:
	-rm -f libtool
$(srcdir)/Makefile.in: # Makefile.am  $(top_srcdir)/configure.in $(ACLOCAL_M4)
	cd $(top_srcdir) && \
	  $(AUTOMAKE) --gnu  win32/dsp_6_0/Makefile
Makefile: # $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) && \
	  CONFIG_HEADERS= CONFIG_LINKS= \
	  CONFIG_FILES=$(subdir)/$@ $(SHELL) ./config.status
uninstall-info-am:
tags: TAGS
TAGS:


DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)

top_distdir = ../..
distdir = $(top_distdir)/$(PACKAGE)-$(VERSION)

distdir: $(DISTFILES)
	@for file in $(DISTFILES); do \
	  if test -f $$file; then d=.; else d=$(srcdir); fi; \
	  dir=`echo "$$file" | sed -e 's,/[^/]*$$,,'`; \
	  if test "$$dir" != "$$file" && test "$$dir" != "."; then \
	    $(mkinstalldirs) "$(distdir)/$$dir"; \
	  fi; \
	  if test -d $$d/$$file; then \
	    cp -pR $$d/$$file $(distdir) \
	    || exit 1; \
	  else \
	    test -f $(distdir)/$$file \
	    || cp -p $$d/$$file $(distdir)/$$file \
	    || exit 1; \
	  fi; \
	done
check-am: all-am
check: check-am
all-am: Makefile

installdirs:

install: install-am
install-exec: install-exec-am
install-data: install-data-am
uninstall: uninstall-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	  `test -z '$(STRIP)' || \
	    echo "INSTALL_PROGRAM_ENV=STRIPPROG='$(STRIP)'"` install
mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES) stamp-h stamp-h[0-9]*

maintainer-clean-generic:
	@echo "This command is intended for maintainers to use"
	@echo "it deletes files that may require special tools to rebuild."
clean: clean-am

clean-am: clean-generic clean-libtool mostlyclean-am

distclean: distclean-am

distclean-am: clean-am distclean-generic distclean-libtool

dvi: dvi-am

dvi-am:

info: info-am

info-am:

install-data-am:

install-exec-am:

install-info: install-info-am

install-man:

installcheck-am:

maintainer-clean: maintainer-clean-am

maintainer-clean-am: distclean-am maintainer-clean-generic

mostlyclean: mostlyclean-am

mostlyclean-am: mostlyclean-generic mostlyclean-libtool

uninstall-am: uninstall-info-am

.PHONY: all all-am check check-am clean clean-generic clean-libtool \
	distclean distclean-generic distclean-libtool distdir dvi \
	dvi-am info info-am install install-am install-data \
	install-data-am install-exec install-exec-am install-info \
	install-info-am install-man install-strip installcheck \
	installcheck-am installdirs maintainer-clean \
	maintainer-clean-generic mostlyclean mostlyclean-generic \
	mostlyclean-libtool uninstall uninstall-am uninstall-info-am


all:
# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
