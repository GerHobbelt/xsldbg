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
	xsldbg.spec xsldbg.suse8.0.spec 

dist_win32: distdir
	zip -r -l xsldbg-$(XSLDBG_MAJOR_VERSION)_$(XSLDBG_MINOR_VERSION)_$(XSLDBG_MICRO_VERSION) xsldbg-$(VERSION)
	rm -rf xsldbg-$(VERSION)

dist-all: dist dist_win32
