TEMPLATE=subdirs

include($$PWD/../xsldbg.pri)

SUBDIRS=en

xsldbgPackageDocs.files = \
    $$PWD/../COPYING \
    $$PWD/../ChangeLog \
    $$PWD/../README.md
xsldbgPackageDocs.path = $$DOCS_ROOT
INSTALLS += xsldbgPackageDocs
