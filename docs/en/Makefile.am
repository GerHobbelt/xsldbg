SUBDIRS = plain KDE GNOME website

EXTRA_DIST= xsldoc.xsl xsldoc.xml

install-data-local: all
	$(mkinstalldirs) $(HTML_DIR)/xsldbg
	@INSTALL@ -m 0644 $(EXTRA_DIST) $(HTML_DIR)/xsldbg




