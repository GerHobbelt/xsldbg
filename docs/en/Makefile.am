SUBDIRS = plain KDE GNOME website

XSLDBG=$(top_builddir)/src/xsldbg

$(XSLDBG):
	@(cd $(top_builddir)/src ; $(MAKE) xsldbg)

xsldoc.txt: xsldoc.xsl xsldoc.xml xsldoc.dtd
	$(XSLDBG_EXPORTS) $(XSLDBG) --param alldocs "1" \
--param xsldbg_version "'$(VERSION)'" xsldoc.xsl xsldoc.xml > xsldoc.txt
	cp xsldoc.txt ./plain/xsldoc.txt

EXTRA_DIST= xsldoc.xsl xsldoc.xml xsldoc.dtd search.xsl search.dtd xsldoc.txt CATALOG catalog.xml

all: xsldoc.txt

install-data-local: all
	$(mkinstalldirs) $(HTML_DIR)/xsldbg
	@INSTALL@ -m 0644 $(EXTRA_DIST) $(HTML_DIR)/xsldbg
	@echo "Adding xsldbg's DTD's to default xml catalog /etc/xml/catalog "
	@echo ""
	@echo "Warning you may need to edit these by hand using xmlcatalog"
	@echo ""
	if test "x`grep xsldbg/catalog.xml /etc/xml/catalog`" = "x" ;then \
	  xmlcatalog -v --noout --add nextCatalog "file://$(HTML_DIR)/xsldbg/catalog.xml" "" /etc/xml/catalog; \
	else \
	  echo "skipping addition of xsldbg catalog as it is already present" ; \
	fi;
clean:
	rm -f xsldoc.txt

very-clean: clean
	rm -f *.*~
	rm -f *~
	rm -f Makefile.in
	rm -f Makefile






