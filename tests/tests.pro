include(../xsldbg.pri)

TEMPLATE = subdirs

testdocuments.files =\
     $$files(*.x*l) \
     $$files(*.dtd) \
    $$files(*.txt)

testdocuments.path = $$DOCS_ROOT/xsldbg/tests
INSTALLS += testdocuments

testsubdocuments.files =\
     $$files(subdir\*.x*l)
testsubdocuments.path = $$DOCS_ROOT/xsldbg/tests/subdir
INSTALLS += testsubdocuments
