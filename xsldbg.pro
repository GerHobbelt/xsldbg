TEMPLATE = subdirs
SUBDIRS = src docs tests

unix:SUBDIRS+=man1

DISTFILES +=\
     ChangeLog \
     README.md \
     COPYING \
     xsldbg.spec
