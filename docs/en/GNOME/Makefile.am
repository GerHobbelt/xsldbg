#EXTRA_DIST =  *.html *.xsl
DATADIR = ..
XSLDIR = GNOME
XSLDBG=$(top_builddir)/src/xsldbg
DOC_SOURCE_DIR=$(top_builddir)/src

# The name of the module.
DOC_MODULE=xsldbg-$(VERSION)

# The top-level SGML file.
DOC_MAIN_SGML_FILE=xsldbg.sgml

$(XSLDBG):
	@(cd $(top_builddir)/src ; $(MAKE) xsldbg)

if USE_GNOMEDOCS
scan:
	gtkdoc-scan --module=xsldbg --source-dir=$(DOC_SOURCE_DIR) 

templates: scan
	 gtkdoc-mktmpl --module=xsldbg

sgml:
	gtkdoc-mkdb --module=xsldbg --source-dir=$(DOC_SOURCE_DIR)

html:
	if ! test -d html ; then mkdir html ; fi
	-cd html && gtkdoc-mkhtml xsldbg ../sgml/$(DOC_MAIN_SGML_FILE)

xsldbg-decl-list.txt : templates

xsldbg-sections.txt : scan
	cp xsldbg-decl-list.txt xsldbg-sections.txt

rebuild: xsldbg-sections.txt templates sgml html

all: 

clean:
	rm -f ./plain/xsldoc.txt
	rm -rf html sgml tmpl
	rm xsldbg-*


else

all:

endif


