TEMPLATE=subdirs
SUBDIRS=
include(../../xsldbg.pri)

unix{
    xsldoctxt.target = .buildfile
    xsldoctxt.commands = touch $$xsldoctxt.target
    xsldoctxt.depends = xsldoctxt2

    xsldoctxt2.files = $$PWD/xsldoc.txt
    xsldoctxt2.depends = $$PWD/xsldoc.xsl $$PWD/xsldoc.xml $$PWD/xsldoc.dtd
    xsldoctxt2.path = ./plain/
    xsldoctxt2.commands = ../../src/xsldbg -param alldocs:\"\'1\'\" --param xsldbg_version:\"\'$${XSLDBG_VERSION}\'\" --output $$PWD/xsldoc.txt $$PWD/xsldoc.xsl $$PWD/xsldoc.xml

    INSTALLS+=xsldoctxt2

    xmldocCatalog.target = .buildfile
    xmldocCatalog.commands = touch $$xmldocCatalog.target
    xmldocCatalog.depends = xmldocCatalog2


    # nasty shell script to seek to install xsldbg's catalog
    xmldocCatalog2.commands=sh $$PWD/createCatalog.sh $${INSTALL_PREFIX}

    xmldocCatalog2.files=
    xmldocCatalog2.path=.
   INSTALLS+=xmldocCatalog2

}

docs.files = *.xsl \
	     *.xml \
	     *.docbook \
	     *.dtd \
	     *.txt \
	     CATALOG

docs.path= $$INSTALL_PREFIX/docs/en/

INSTALLS += docs
