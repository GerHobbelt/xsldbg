EXTRA_DIST=  *.html *.xsl

DATADIR = ..
XSLDIR = KDE
XSLDBG=$(top_builddir)/src/xsldbg
APISRC_DIR=src
APISRC_HEADER_FILES= \
$(APISRC_DIR)/xsldbg.h		\
$(APISRC_DIR)/help.h		\
$(APISRC_DIR)/options.h 	\
$(APISRC_DIR)/arraylist.h	\
$(APISRC_DIR)/xslbreakpoint.h 	\
$(APISRC_DIR)/xslcallpoint.h	\
$(APISRC_DIR)/xslsearch.h	\
$(APISRC_DIR)/debugXSL.h	\
$(APISRC_DIR)/files.h		

APISRC_SOURCE_FILES=\
$(top_builddir)/src/*.c


KDEXSL_FILES= \
bookinfo.xsl  	\
commands.xsl  	\
credits.xsl  	\
overview.xsl  	\
xsldoc2kde.xsl 

APIDOCS_DIR=apidocs

EXTRA_DIST=$(KDEXSL_FILES) $(APISRC_HEADER_FILES)

# must use -P or else kdoc will be confused!!
CPPFLAGS=-E -C -P


$(XSLDBG):
	@(cd $(top_builddir)/src ; $(MAKE) xsldbg)

index.docbook : $(KDEXSL_FILES) $(DATADIR)/xsldoc.xml 
	$(XSLDBG_EXPORTS) $(XSLDBG) --cd $(DATADIR) --param xsldbg_version "'$(VERSION)'"  -o $(XSLDIR)/index.docbook $(XSLDIR)/xsldoc2kde.xsl xsldoc.xml

index.cache.bz2 : index.docbook $(DATADIR)/xsldoc.xml 
	@echo ""
	@echo ""
	@echo "Making KDE style application documentation see `pwd`/index.html"
	@echo ""
	@echo ""
	meinproc index.docbook
	meinproc --htdig index.docbook > index.cache
	rm -f index.cache.bz2
	bzip2 index.cache
	ln -fs $(KDEDIR)/share/doc/HTML/en/common ./

apidocs : $(APIDOCS_DIR)/index.html

$(APIDOCS_DIR)/index.html: $(APISRC_DIR) $(APISRC_HEADER_FILES)
	@echo ""
	@echo ""
	@echo "Making KDE style API documentation see `pwd`/$(APIDOCS_DIR)/index.html"
	@echo ""
	@echo ""
	if ! test -d  $(APIDOCS_DIR); then mkdir $(APIDOCS_DIR); fi
	kdoc -d $(APIDOCS_DIR)  $(APISRC_HEADER_FILES)


$(APISRC_DIR): 
	if ! test -d $(APISRC_DIR) ; then mkdir $(APISRC_DIR) ; fi


# now use the preprocessor to create the KDE version of 
#  header suitable to pass onto kdoc
$(APISRC_HEADER_FILES): $(APISRC_DIR)
	$(CC) -DUSE_KDOC -DBUILD_DOCS -DUSE_KDE_DOCS $(CPPFLAGS) $(top_builddir)/src/libxsldbg/$(@F) > $@



if USE_KDEDOCS
all: index.docbook index.cache.bz2 $(DATADIR)/xsldoc.xml apidocs 

clean: 
	rm -f index.docbook
	rm -f *.html 
	rm -f *.bz2
	rm -f $(APIDOCS_DIR)/*.html
	rm -f $(APISRC_DIR)/*.h

very-clean:clean
	rm -f *.*~
	rm -f *~


install-data-local: 
	$(mkinstalldirs) $(KDEDIR)/share/doc/HTML/en/khelpcenter/xsldbg/
	@INSTALL@ -m 0644 index.docbook index.cache.bz2 $(KDEDIR)/share/doc/HTML/en/khelpcenter/xsldbg
	ln -fs $(KDEDIR)/share/doc/HTML/en/common $(KDEDIR)/share/doc/HTML/en/khelpcenter/xsldbg/
else
# empty
all:

clean:

install-data-local:
endif