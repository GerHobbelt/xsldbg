TEMPLATE=subdirs
SUBDIRS=
include(../../xsldbg.pri)

extradocs.files = $$PWD/*.xsl \
             $$PWD/*.xml \
             $$PWD/*.docbook \
             $$PWD/*.dtd \
             $$PWD/CATALOG \
             $$PWD/catalog.xml \
             $$PWD/xsldoc.xsl $$PWD/xsldoc.xml $$PWD/xsldoc.txt \
             $$PWD/../../COPYING


extradocs.path= $$DOCS_ROOT/en/

INSTALLS += extradocs

unix{
    message(doc root $$DOCS_ROOT)

    # created XML catalog as needed
    xmldocCatalog.target = .buildfile5
    xmldocCatalog.commands = touch $$xmldocCatalog.target
    xmldocCatalog.depends = xmldocCatalog2

    # nasty shell script to seek to install xsldbg catalog
    xmldocCatalog2.commands=sh $$PWD/createCatalog.sh $${DOCS_ROOT}/en
    xmldocCatalog2.files =
    xmldocCatalog2.path = $$DOCS_ROOT

    INSTALLS+=xmldocCatalog2

    #generate the html text documentation
    xsldoctxt.target = .buildfile1
    xsldoctxt.commands = touch $$xsldoctxt.target
    xsldoctxt.depends = xsldoctxt2

    xsldoctxt2.depends = $$PWD/xsldoc.xsl $$PWD/xsldoc.xml $$PWD/xsldoc.dtd install_extradocs
    xsldoctxt2.commands = ../../src/xsldbg --noautoloadconfig --noshell --param alldocs:\"\'1\'\" --param xsldbg_version:\"\'$${XSLDBG_VERSION}\'\" --output $$PWD/xsldoc.txt $$PWD/xsldoc.xsl $$PWD/xsldoc.xml

    # generate the HTML documentation
    xsldocHtml.target = .buildfile2
    xsldocHtml.commands = touch $$xsldocHtml.target
    xsldocHtml.depends = xsldocHtml2

    xsldocHtml2.depends = $$PWD/html/xsldoc2html.xsl $$PWD/xsldoc.xml $$PWD/xsldoc.dtd install_extradocs
    xsldocHtml2.commands = ../../src/xsldbg --noautoloadconfig --noshell --param alldocs:\"\'1\'\" --param xsldbg_version:\"\'$${XSLDBG_VERSION}\'\" --output $$PWD/html/index.html $$PWD/html/xsldoc2html.xsl $$PWD/xsldoc.xml

    #generate KDE docbook if saxon tool is present
    SAXON=$$system(saxon -t 2>&1)
    contains(SAXON, SAXON){
       message(found saxon so will generate docbook documentation)
       # generate the docbook documentation for KDE
        xsldocKDEdocs.target = .buildfile3
        xsldocKDEdocs.commands = touch $$xsldocKDEdocs.target
        xsldocKDEdocs.depends = xsldocKDEdocs2 install_extradocs xmldocCatalog

        xsldocKDEdocs2.files = $$PWD/KDE/index.docbook
        xsldocKDEdocs2.depends = $$PWD/KDE/xsldoc2kde.xsl $$PWD/xsldoc.xml $$PWD/xsldoc.dtd
        xsldocKDEdocs2.path = $$KDEDOCS_ROOT/en/xsldbg
        xsldocKDEdocs2.commands = saxon -o $$PWD/KDE/index.docbook $$PWD/xsldoc.xml $$PWD/KDE/xsldoc2kde.xsl alldocs=\"\'1\'\" xsldbg_version=\"\'$${XSLDBG_VERSION}\'\"
        INSTALLS+=xsldocKDEdocs2

        # generate the docbook documentation for Gnome
        xsldocGnomedocs.target = .buildfile4
        xsldocGnomedocs.commands = touch $$xsldocGnomedocs.target
        xsldocGnomedocs.depends = xsldocGnomedocs2 install_extradocs xmldocCatalog

        xsldocGnomedocs2.files = $$PWD/GNOME/gnome.docbook
        xsldocGnomedocs2.depends = $$PWD/GNOME/xsldoc2gnome.xsl $$PWD/xsldoc.xml $$PWD/xsldoc.dtd
        xsldocGnomedocs2.path = $$DOCS_ROOT/en/GNOME
        xsldocGnomedocs2.commands = saxon -o $$PWD/GNOME/gnome.docbook $$PWD/xsldoc.xml $$PWD/GNOME/xsldoc2gnome.xsl alldocs=\"\'1\'\" xsldbg_version=\"\'$${XSLDBG_VERSION}\'\"

        # disable Gnome documentation creation for now
        # INSTALLS+=xsldocGnomedocs2

    }else {
       message(Did not find saxon skipping generation of docbook documentation)
    }

    #generate KDE docbook cache if meinproc5 is present
    MEINPROC5=$$system(meinproc5  --version 2>/dev/null)
    contains(MEINPROC5, meinproc){
       message(found meinproc5 so will generate KDE documenation cache)
        xsldocCache.target = .buildfile5
        xsldocCache.commands = touch $$xsldocCache.target
        xsldocCache.depends = xsldocCache2 install_extradocs

        xsldocCache2.files = $$PWD/KDE/index.cache.bz2
        xsldocCache2.depends = $$PWD/KDE/index.docbook
        xsldocCache2.commands = meinproc5 --cache $$xsldocCache2.files $$xsldocCache2.depends
        xsldocCache2.path = $$KDEDOCS_ROOT/en/xsldbg
        INSTALLS+=xsldocCache2
    }else {
       message(Did not find meinproc skipping generation of KDE documentation cache)
    }

}


#install text docs
xsldoctxt2.files = $$PWD/xsldoc.txt
unix:xsldoctxt2.path = $$DOCS_ROOT/en
win32:xsldoctxt2.path = $$DOCS_ROOT\\en
INSTALLS+=xsldoctxt2

#install html docs
xsldocHtml2.files = $$PWD/html/index.html
unix:xsldocHtml2.path = $$DOCS_ROOT/en/html
win32:xsldocHtml2.path = $$DOCS_ROOT\\en\\html
INSTALLS+=xsldocHtml2


