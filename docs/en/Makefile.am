SUBDIRS = plain KDE GNOME website

XSLDBG=$(top_builddir)/src/xsldbg

$(XSLDBG):
	@(cd $(top_builddir)/src ; $(MAKE) xsldbg)

xsldoc.txt:
	$(XSLDBG_EXPORTS) $(XSLDBG) --param alldocs "1" --param xsldbg_version "'$(VERSION)'" xsldoc.xsl xsldoc.xml > xsldoc.txt
	cp xsldoc.txt ./plain/xsldoc.txt

EXTRA_DIST= xsldoc.xsl xsldoc.xml xsldoc.dtd search.xsl xsldoc.txt

all: xsldoc.txt

install-data-local: all
	$(mkinstalldirs) $(HTML_DIR)/xsldbg
	@INSTALL@ -m 0644 $(EXTRA_DIST) $(HTML_DIR)/xsldbg	




