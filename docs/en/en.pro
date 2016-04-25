TEMPLATE=subdirs
SUBDIRS=

unix{

    INSTALL_PREFIX="/usr/local/xsldbg"
    xsldoctxt.target = .buildfile
    xsldoctxt.commands = touch $$xsldoctxt.target
    xsldoctxt.depends = xsldoctxt2

    xsldoctxt2.files = $$PWD/xsldoc.txt
    xsldoctxt2.depends = $$PWD/xsldoc.xsl $$PWD/xsldoc.xml $$PWD/xsldoc.dtd
    xsldoctxt2.path = ./plain/
    xsldoctxt2.commands = ../../src/xsldbg -param alldocs:\"\'1\'\" --param xsldbg_version:\"\'4.4.1\'\" --output $$PWD/xsldoc.txt $$PWD/xsldoc.xsl $$PWD/xsldoc.xml

    INSTALLS+=xsldoctxt2

    xmldocCatalog.target = .buildfile
    xmldocCatalog.commands = touch $$xmldocCatalog.target
    xmldocCatalog.depends = xmldocCatalog2


    # nasty shell script to seek to install xsldbg's catalog
    xmldocCatalog2.commands=\
                echo "" > result.txt; \
		if ! test -e /etc/xml/catalog; then \
		     echo "Creating /etc/xml/catalog" >> result.txt 2>&1  ; \
		     xmlcatalog -v --noout --create /etc/xml/catalog >> result.txt 2>&1 ; \
		fi;  \
		if test "x`grep xsldbg.*catalog.xml /etc/xml/catalog`" = "x" ;then \
		    xmlcatalog -v --noout --add nextCatalog '' "file://$$INSTALL_PREFIX/docs/en/catalog.xml" "" /etc/xml/catalog >> result.txt 2>&1 ;   \
		    echo "Adding xsldbg's DTD's to default xml catalog /etc/xml/catalog" >> result.txt 2>&1; \ 
		    echo "Warning you may need to edit these by hand using xmlcatalog" >> result.txt 2>&1; \ 
		else \
		    echo "skipping addition of xsldbg catalog as it is already present" >> result.txt 2>&1 ; \
		fi; \
		echo "Results of seeking to add xsldbg\'s catalog is"; \
		echo "----------------------------------------------"; \
		cat result.txt; \
		echo "----------------------------------------------";  

    xmldocCatalog2.files=
    xmldocCatalog2.path=.
   INSTALLS+=xmldocCatalog2

}
win32{
	INSTALL_PREFIX="\xsldbg"
}

docs.files = *.xsl \
	     *.xml \
	     *.docbook \
	     *.dtd \
	     *.txt \
	     CATALOG

docs.path= $$INSTALL_PREFIX/docs/en/

INSTALLS += docs
