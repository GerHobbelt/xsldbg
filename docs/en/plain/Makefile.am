EXTRA_DIST = *.html *.xsl 
DATADIR = ..
XSLDIR = plain
XSLDBG=$(top_builddir)/src/xsldbg

all: index.html

$(XSLDBG):
	@(cd $(top_builddir)/src ; $(MAKE) xsldbg)
 
*.html index.html : xsldoc2html.xsl $(DATADIR)/xsldoc.xml $(XSLDBG)
	$(XSLDBG) --cd $(DATADIR)  --param xsldbg_version "'$(VERSION)'"  -o $(XSLDIR)/index.html  $(XSLDIR)/xsldoc2html.xsl xsldoc.xml 
 
install-data-local: all
	$(mkinstalldirs) $(HTML_DIR)/xsldbg/plain
	@INSTALL@ -m 0644 $(EXTRA_DIST) $(HTML_DIR)/xsldbg/plain

clean:
	rm index.html	

