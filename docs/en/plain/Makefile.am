EXTRA_DIST = *.html *.xsl
DATADIR = ..
XSLDIR = plain

all: index.html

index.html : xsldoc2html.xsl $(DATADIR)/xsldoc.xml
	xsldbg --cd $(DATADIR)  --param xsldbg_version "'$(VERSION)'"  -o $(XSLDIR)/index.html  $(XSLDIR)/xsldoc2html.xsl xsldoc.xml 

install-data-local: all
	$(mkinstalldirs) $(HTML_DIR)/xsldbg/plain
	@INSTALL@ -m 0644 $(EXTRA_DIST) $(HTML_DIR)/xsldbg/plain

clean:
	rm index.html	

