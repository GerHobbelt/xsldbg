include(../xsldbg.pri)

TEMPLATE = subdirs

testdocuments.files =\
     $$files(*.x*l) \
     $$files(*.dtd) \
     $$files(*.pl) \
    $$files(*.data) \
    $$files(*.txt)

testdocuments.path = $$DOCS_ROOT/en/tests
INSTALLS += testdocuments

testsubdocuments.files =\
     $$files(subdir\*.x*l)
testsubdocuments.path = $$DOCS_ROOT/en/tests/subdir
INSTALLS += testsubdocuments
