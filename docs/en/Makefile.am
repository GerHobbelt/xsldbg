SUBDIRS = plain KDE GNOME website

XSLDBG=$(top_builddir)/src/xsldbg

$(XSLDBG):
	@(cd $(top_builddir)/src ; $(MAKE) xsldbg)

xsldoc.txt: xsldoc.xsl xsldoc.xml xsldoc.dtd
	$(XSLDBG_EXPORTS) $(XSLDBG) --param alldocs "1" \
--param xsldbg_version "'$(VERSION)'" xsldoc.xsl xsldoc.xml > xsldoc.txt
	cp xsldoc.txt ./plain/xsldoc.txt

EXTRA_DIST= xsldoc.xsl xsldoc.xml xsldoc.dtd search.xsl search.dtd xsldoc.txt CATALOG

all: xsldoc.txt

install-data-local: all
	$(mkinstalldirs) $(HTML_DIR)/xsldbg
	@INSTALL@ -m 0644 $(EXTRA_DIST) $(HTML_DIR)/xsldbg
	@echo "Adding xsldbg's DTD's to default xml catalog /etc/xml/catalog "
	@echo ""
	@echo "Warning you may need to edit these by hand using xmlcatalog"
	@echo ""
	xmlcatalog -v --noout --add public "-//xsldbg//DTD search XML V1.0//EN" \
		"file://$(HTML_DIR)/xsldbg/search.dtd" /etc/xml/catalog	
	xmlcatalog -v  --noout --add public "-//xsldbg//DTD xsldoc XML V1.0//EN" \
		"file://$(HTML_DIR)/xsldbg/xsldoc.dtd" /etc/xml/catalog
clean:
	rm -f xsldoc.txt

very-clean: clean
	rm -f *.*~
	rm -f *~
	rm -f Makefile.in
	rm -f Makefile






