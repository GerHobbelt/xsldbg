SUBDIRS = plain KDE GNOME website

XSLDBG=$(top_builddir)/src/xsldbg

$(XSLDBG):
	@(cd $(top_builddir)/src ; $(MAKE) xsldbg)

EXTRA_DIST= xsldoc.xsl xsldoc.xml xsldoc.dtd search.xsl

install-data-local: all
	$(mkinstalldirs) $(HTML_DIR)/xsldbg
	@INSTALL@ -m 0644 $(EXTRA_DIST) $(HTML_DIR)/xsldbg




