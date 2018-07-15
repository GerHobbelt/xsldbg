include(../xsldbg.pri)

TEMPLATE = subdirs

testdocuments.files =\
     $$files(*.x*l) \
     $$files(*.dtd) \
     $$files(*.pl)

testdocuments.path = $$DOCS_ROOT/en/tests
INSTALLS += testdocuments

testsubdocuments.files =\
     $$files(subdir\*.x*l)
testdocuments.path = $$DOCS_ROOT/en/tests/subdir
INSTALLS += testsubdocuments
