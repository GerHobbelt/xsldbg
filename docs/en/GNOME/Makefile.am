#EXTRA_DIST =  *.html *.xsl
DATADIR = ..
XSLDIR = GNOME
XSLDBG=$(top_builddir)/src/xsldbg

APISRC_DIR=src
APISRC_FILES= \
$(APISRC_DIR)/xsldbg.h		\
$(APISRC_DIR)/help.h		\
$(APISRC_DIR)/options.h 	\
$(APISRC_DIR)/arraylist.h	\
$(APISRC_DIR)/xslbreakpoint_2.h \
$(APISRC_DIR)/xslcallpoint.h	\
$(APISRC_DIR)/xslsearch.h	\
$(APISRC_DIR)/debugXSL.h	\
$(APISRC_DIR)/files.h	


CPPFLAGS=-E -C -P


GNOMEXSL_FILES= \
articleinfo.xsl \
commands.xsl  	\
credits.xsl  	\
overview.xsl  	\
authors.xsl	\
xsldoc2gnome.xsl 

APIDOCS_DIR=apidocs

EXTRA_DIST=$(GNOMEXSL_FILES)

# The name of the module.
API_MODULE=xsldbg-$(VERSION)

# The top-level SGML file.
API_MAIN_SGML_FILE=xsldbg.sgml

$(XSLDBG):
	@(cd $(top_builddir)/src ; $(MAKE) xsldbg)

if USE_GNOMEDOCS
scan: $(APISRC_FILES)
	gtkdoc-scan --module=xsldbg --source-dir=$(APISRC_DIR) 

templates: scan
	 gtkdoc-mktmpl --module=xsldbg

sgml:
	gtkdoc-mkdb --module=xsldbg --source-dir=$(APISRC_DIR)

html:
	if ! test -d html ; then mkdir html ; fi
	-cd html && gtkdoc-mkhtml xsldbg ../$(API_MAIN_SGML_FILE)

xsldbg-decl-list.txt : templates

xsldbg-sections.txt : scan
	cp xsldbg-decl-list.txt xsldbg-sections.txt

$(APISRC_DIR): 
	if ! test -d $(APISRC_DIR) ; then mkdir $(APISRC_DIR) ; fi

# now use the preprocessor to create the GNOME version of 
#  header suitable to pass onto gtkdoc-*
$(APISRC_FILES): $(APISRC_DIR)
	$(CC) -DBUILD_DOCS -DUSE_GNOME_DOCS $(CPPFLAGS) $(top_builddir)/src/$(@F) > $@

gnome.docbook : $(GNOMEXSL_FILES) ../xsldoc.xml
	$(XSLDBG) --param xsldbg_version "'$(VERSION)'" --param xsldbg_date "'$(TIMESTAMP)'" xsldoc2gnome.xsl ../xsldoc.xml  > gnome.docbook

index.html appDocs : gnome.docbook
	$(XSLDBG) $(DOCBOOK_PREFIX)/xsl/html/docbook.xsl  gnome.docbook > index.html

rebuild: xsldbg-sections.txt templates sgml html appDocs

all: appDocs rebuild 

clean:
	rm -f ./plain/xsldoc.txt
	rm -rf html sgml tmpl src
	rm -f xsldbg-*

very-clean:clean
	rm -f *.*~
	rm -f *~
	rm -f Makefile.in
	rm -f Makefile
else

all:

endif


