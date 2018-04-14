TEMPLATE=subdirs
SUBDIRS=
include(../../xsldbg.pri)

unix{
    #generate the plain text documentation
    xsldoctxt.target = .buildfile
    xsldoctxt.commands = touch $$xsldoctxt.target
    xsldoctxt.depends = xsldoctxt2

    xsldoctxt2.files = $$PWD/xsldoc.txt
    xsldoctxt2.depends = $$PWD/xsldoc.xsl $$PWD/xsldoc.xml $$PWD/xsldoc.dtd
    xsldoctxt2.path = ./plain/
    xsldoctxt2.commands = ../../src/xsldbg -param alldocs:\"\'1\'\" --param xsldbg_version:\"\'$${XSLDBG_VERSION}\'\" --output $$PWD/xsldoc.txt $$PWD/xsldoc.xsl $$PWD/xsldoc.xml

    INSTALLS+=xsldoctxt2
    
    # generate the HTML documentation
    xsldoctxtplain.target = .buildfile
    xsldoctxtplain.commands = touch $$xsldoctxt.target
    xsldoctxtplain.depends = xsldoctxtplain2
    
    xsldoctxtplain2.files = $$PWD/plain/index.html
    xsldoctxtplain2.depends = $$PWD/plain/xsldoc2html.xsl $$PWD/xsldoc.xml $$PWD/xsldoc.dtd
    xsldoctxtplain2.path = ./plain/
    xsldoctxtplain2.commands = ../../src/xsldbg --noshell --param alldocs:\"\'1\'\" --verbose --param xsldbg_version:\"\'$${XSLDBG_VERSION}\'\" --output $$PWD/plain/index.html $$PWD/plain/xsldoc2html.xsl $$PWD/xsldoc.xml
    
    INSTALLS+=xsldoctxtplain2
    

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
	     CATALOG \
	     $PWD/xsldoc.xsl $$PWD/xsldoc.xml $$PWD/xsldoc.txt
	     

docs.path= $$INSTALL_PREFIX/docs/en/

INSTALLS += docs
