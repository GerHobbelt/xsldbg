TEMPLATE=subdirs
SUBDIRS=
include(../../xsldbg.pri)

unix{
    #generate the plain text documentation
    xsldoctxt.target = .buildfile1
    xsldoctxt.commands = touch $$xsldoctxt.target
    xsldoctxt.depends = xsldoctxt2

    xsldoctxt2.files = $$PWD/xsldoc.txt
    xsldoctxt2.depends = $$PWD/xsldoc.xsl $$PWD/xsldoc.xml $$PWD/xsldoc.dtd
    xsldoctxt2.path = $$DOCS_PATH
    xsldoctxt2.commands = ../../src/xsldbg -noshell -param alldocs:\"\'1\'\" --param xsldbg_version:\"\'$${XSLDBG_VERSION}\'\" --output $$PWD/xsldoc.txt $$PWD/xsldoc.xsl $$PWD/xsldoc.xml

    INSTALLS+=xsldoctxt2
    
    # generate the HTML documentation
    xsldoctxtplain.target = .buildfile2
    xsldoctxtplain.commands = touch $$xsldoctxtplain.target
    xsldoctxtplain.depends = xsldoctxtplain2
    
    xsldoctxtplain2.files = $$PWD/plain/index.html
    xsldoctxtplain2.depends = $$PWD/plain/xsldoc2html.xsl $$PWD/xsldoc.xml $$PWD/xsldoc.dtd
    xsldoctxtplain2.path = $$DOCS_ROOT/en/html
    xsldoctxtplain2.commands = ../../src/xsldbg --noshell --param alldocs:\"\'1\'\" --param xsldbg_version:\"\'$${XSLDBG_VERSION}\'\" --output $$PWD/plain/index.html $$PWD/plain/xsldoc2html.xsl $$PWD/xsldoc.xml
    
    INSTALLS+=xsldoctxtplain2
    
    #generate KDE docbook if saxon tool is present
    SAXON=$$system(saxon -t 2>&1)
    contains(SAXON, SAXON){
       message(found saxon so will generate docbook documentation)
       # generate the docbook documentation for KDE
        xsldocKDEdocs.target = .buildfile3
        xsldocKDEdocs.commands = touch $$xsldocKDEdocs.target
        xsldocKDEdocs.depends = xsldocKDEdocs2
        
        xsldocKDEdocs2.files = $$PWD/KDE/index.docbook 
        xsldocKDEdocs2.depends = $$PWD/KDE/xsldoc2kde.xsl $$PWD/xsldoc.xml $$PWD/xsldoc.dtd
        xsldocKDEdocs2.path = $$DOCS_ROOT/en/KDE
        xsldocKDEdocs2.commands = saxon -o $$PWD/KDE/index.docbook $$PWD/xsldoc.xml $$PWD/KDE/xsldoc2kde.xsl alldocs=\"\'1\'\" xsldbg_version=\"\'$${XSLDBG_VERSION}\'\" 
        
        INSTALLS+=xsldocKDEdocs2
        
        # generate the docbook documentation for Gnome
        xsldocGnomedocs.target = .buildfile4
        xsldocGnomedocs.commands = touch $$xsldocGnomedocs.target
        xsldocGnomedocs.depends = xsldocGnomedocs2
        
        xsldocGnomedocs2.files = $$PWD/GNOME/gnome.docbook 
        xsldocGnomedocs2.depends = $$PWD/GNOME/xsldoc2gnome.xsl $$PWD/xsldoc.xml $$PWD/xsldoc.dtd
        xsldocGnomedocs2.path = $$DOCS_ROOT/en/GNOME
        xsldocGnomedocs2.commands = saxon -o $$PWD/GNOME/gnome.docbook $$PWD/xsldoc.xml $$PWD/GNOME/xsldoc2gnome.xsl alldocs=\"\'1\'\" xsldbg_version=\"\'$${XSLDBG_VERSION}\'\" 
        
        INSTALLS+=xsldocGnomedocs2
        
    }else {
       message(Did not find saxon, skipping generation of docbook documentation)
    }
    

    xmldocCatalog.target = .buildfile5
    xmldocCatalog.commands = touch $$xmldocCatalog.target
    xmldocCatalog.depends = xmldocCatalog2
 

    # nasty shell script to seek to install xsldbg catalog
    xmldocCatalog2.commands=sh $$PWD/createCatalog.sh $${INSTALL_PREFIX}
    xmldocCatalog2.files =
    xmldocCatalog2.path = $$INSTALL_PREFIX

    INSTALLS+=xmldocCatalog2
}

docs.files = *.xsl \
	     *.xml \
	     *.docbook \
	     *.dtd \
	     *.txt \
	     CATALOG \
	     $PWD/xsldoc.xsl $$PWD/xsldoc.xml $$PWD/xsldoc.txt
	     

docs.path= $$DOCS_ROOT/en/

INSTALLS += docs
