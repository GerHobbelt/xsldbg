EXTRA_DIST=  *.html *.xsl

DATADIR = ..
XSLDIR = KDE


index.docbook : xsldoc2kde.xsl $(DATADIR)/xsldoc.xml 
	xsldbg --cd $(DATADIR) --param xsldbg_version "'$(VERSION)'"  -o $(XSLDIR)/index.docbook $(XSLDIR)/xsldoc2kde.xsl xsldoc.xml

index.cache.bz2 : index.docbook
	meinproc index.docbook
	meinproc --htdig index.docbook > index.cache
	rm -f index.cache.bz2
	bzip2 index.cache
	ln -fs $(KDEDIR)/share/doc/HTML/en/common ./


if USE_KDEDOCS
all: index.docbook index.cache.bz2

clean:
	rm -f index.docbook
	rm -f *.html
	rm -f *.bz2

install-data-local: all
	$(mkinstalldirs) $(KDEDIR)/share/doc/HTML/en/khelpcenter/xsldbg/
	@INSTALL@ -m 0644 index.docbook index.cache.bz2 $(KDEDIR)/share/doc/HTML/en/khelpcenter/xsldbg
	ln -fs $(KDEDIR)/share/doc/HTML/en/common $(KDEDIR)/share/doc/HTML/en/khelpcenter/xsldbg/
else
# empty
all:

clean:

install-data-local:
endif