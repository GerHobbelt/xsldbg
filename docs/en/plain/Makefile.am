EXTRA_DIST = *.html *.xsl
DATADIR = ..
XSLDIR = plain

all: index.html

$(top_builddir)/src/xsldbg:
	@(cd $(top_builddir)/src ; $(MAKE) xsldbg)
 
index.html : xsldoc2html.xsl $(DATADIR)/xsldoc.xml $(top_builddir)/src/xsldbg
	$(top_builddir)/src/xsldbg --cd $(DATADIR)  --param xsldbg_version "'$(VERSION)'"  -o $(XSLDIR)/index.html  $(XSLDIR)/xsldoc2html.xsl xsldoc.xml 
 

install-data-local: all
	$(mkinstalldirs) $(HTML_DIR)/xsldbg/plain
	@INSTALL@ -m 0644 $(EXTRA_DIST) $(HTML_DIR)/xsldbg/plain

clean:
	rm index.html	

