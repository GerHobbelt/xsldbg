# not a GNU package. You can remove this line, if
# have all needed files, that a GNU package needs
AUTOMAKE_OPTIONS = foreign 1.4

SUBDIRS = src tests results docs win32

EXTRA_DIST=\
	Makefile.cvs 		\
	config_riscos.h  	\
	config_riscos.h.in  	\
	xsldbg-config.in  	\
	xsldbgConf.sh.in	\
	xsldbg.spec xsldbg.suse8.0.spec \
	xsldbg.pro src/src.pro src/xsldbg.pro src/libxsldbg/libxsldbg.pro src/libqtnotifier/libqtnotifier.pro  \
	src/Makefile.msvc

dist_win32: distdir
	zip -r -l xsldbg-$(VERSION).zip xsldbg-$(VERSION)
	rm -rf xsldbg-$(VERSION)

dist-all: dist dist_win32
