#EXTRA_DIST =  *.html *.xsl
DATADIR = ..
XSLDIR = GNOME
XSLDBG=$(top_builddir)/src/xsldbgtest

APISRC_DIR=src
APISRC_FILES= \
$(APISRC_DIR)/xsldbg.h		\
$(APISRC_DIR)/help.h		\
$(APISRC_DIR)/options.h 	\
$(APISRC_DIR)/arraylist.h	\
$(APISRC_DIR)/breakpoint.h 	\
$(APISRC_DIR)/callstack.h	\
$(APISRC_DIR)/search.h		\
$(APISRC_DIR)/debugXSL.h	\
$(APISRC_DIR)/files.h		\
$(APISRC_DIR)/cmds.h		\
$(APISRC_DIR)/utils.h	


CPPFLAGS=-E -C -P


GNOMEXSL_FILES= \
articleinfo.xsl \
commands.xsl  	\
overview.xsl  	\
about.xsl	\
xsldoc2gnome.xsl 

APIDOCS_DIR=html

EXTRA_DIST=$(GNOMEXSL_FILES)

# The name of the module.
API_MODULE=xsldbg-$(VERSION)

# The top-level SGML file.
API_MAIN_SGML_FILE=xsldbg.sgml

$(XSLDBG):
	@(cd $(top_builddir)/src ; $(MAKE) xsldbgtest)

gnome.docbook : $(GNOMEXSL_FILES) ../xsldoc.xml
	$(XSLDBG) --param xsldbg_version "'$(VERSION)'" --param xsldbg_date "'$(TIMESTAMP)'" xsldoc2gnome.xsl ../xsldoc.xml  > gnome.docbook


# You only need this to regenerate HTML pages
if USE_GNOMEDOCS
# we need both headers and source to make GNOME docs
# so just copy the source across
scan: $(APISRC_FILES)
	cp $(top_builddir)/src/libxsldbg/*.c $(APISRC_DIR)
	gtkdoc-scan --module=xsldbg --source-dir=$(APISRC_DIR) 

templates: scan
	 gtkdoc-mktmpl --module=xsldbg

sgml:
	gtkdoc-mkdb --module=xsldbg --source-dir=$(APISRC_DIR)

$(APIDOCS_DIR): $(API_MAIN_SGML_FILE)
	if ! test -d $(APIDOCS_DIR) ; then mkdir $(APIDOCS_DIR) ; fi
	-cd $(APIDOCS_DIR) && gtkdoc-mkhtml xsldbg ../$(API_MAIN_SGML_FILE)

xsldbg-decl-list.txt : templates

xsldbg-sections.txt : scan
	cp xsldbg-decl-list.txt xsldbg-sections.txt

$(APISRC_DIR): 
	if ! test -d $(APISRC_DIR) ; then mkdir $(APISRC_DIR) ; fi

# now use the preprocessor to create the GNOME version of 
#  header suitable to pass onto gtkdoc-*
$(APISRC_FILES): $(APISRC_DIR)
	$(CC) -DBUILD_DOCS -DUSE_GNOME_DOCS $(CPPFLAGS) $(top_builddir)/src/libxsldbg/$(@F) > $@

index.html appDocs : gnome.docbook
	$(XSLDBG) $(DOCBOOK_PREFIX)/html/docbook.xsl  gnome.docbook > index.html

rebuild: xsldbg-sections.txt templates sgml html

all: appDocs rebuild gnome.docbook

clean:
	rm -rf html sgml tmpl src
	rm -f xsldbg-*
	rm -f gnome.docbook index.html

very-clean: clean
	rm -f *.*~
	rm -f *~
	rm -f Makefile.in

else

all: gnome.docbook

clean:
	rm -f gnome.docbook
endif

docdir = $(datadir)/gnome/help/xsldbg/EN/

install-data-local: 
	if test -d $(GNOMEDIR) ; then  if ! test -d $(docdir) ; then mkdir -p $(docdir);fi; cp  gnome.docbook $(docdir);fi

