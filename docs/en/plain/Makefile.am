EXTRA_DIST := $(wildcard *.html)  $(wildcard *.xsl) $(wildcard *.xml) $(wildcard *.txt)
DATADIR = ..
XSLDIR = plain
XSLDBG=$(top_builddir)/src/xsldbgtest

all: index.html

$(XSLDBG):
	@(cd $(top_builddir)/src ; $(MAKE) xsldbgtest)
 
index.html : xsldoc2html.xsl $(DATADIR)/xsldoc.xml $(XSLDBG)
	$(XSLDBG) --cd $(DATADIR)  --param xsldbg_version "'$(VERSION)'"  -o $(XSLDIR)/index.html  $(XSLDIR)/xsldoc2html.xsl xsldoc.xml 
 
install-data-local: all
	$(mkinstalldirs) $(DESTDIR)/$(HTML_DIR)/xsldbg/plain
	@INSTALL@ -m 0644 $(EXTRA_DIST) $(DESTDIR)/$(HTML_DIR)/xsldbg/plain

clean:
	rm -f index.html	

