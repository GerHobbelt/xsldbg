EXTRA_DIST=index.html

XSLDBG=$(top_builddir)/src/xsldbg

$(XSLDBG):
	@(cd $(top_builddir)/src ; $(MAKE) xsldbg)

all:
	cp ../catalog.xml ./
	cp ../CATALOG ./
	cp ../../../win32/dsp/createcatalog.bat ./
install-data-local: all
#	$(mkinstalldirs) $(HTML_DIR)/xsldbg/website
#	@INSTALL@ -m 0644 $(EXTRA_DIST) $(HTML_DIR)/xsldbg/website