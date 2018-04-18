TEMPLATE = lib
CONFIG += qt warn_on release thread

unix:INCLUDEPATH += $(LIBXSLT_PREFIX)/include
unix:INCLUDEPATH += $(LIBXML_PREFIX)/include/libxml2
unix:INCLUDEPATH += $(INCONV_PREFIX)/include
win32:INCLUDEPATH += $(LIBXSLT_PREFIX)/include
win32:INCLUDEPATH += $(LIBXML_PREFIX)/include
win32:INCLUDEPATH += $(ICONV_PREFIX)/include
INCLUDEPATH += ..
INCLUDEPATH += ../..
INCLUDEPATH += ../libxsldbg
DEFINES += DOCS_PATH=\"$(DOCSPATH)\" 
win32:DEFINES+=WIN32 QT_DLL

# we need runtime information for dynamic casts to work
win32:TMAKE_CXXFLAGS = -GR

DESTDIR =../../lib

HEADERS += \
	../libxsldbg/xsldbgdebuggerbase.h

SOURCES += qtnotifier2.cpp \
           xsldbgdebuggerbase.cpp \
           xsldbgevent.cpp\
           xsldbgnotifier.cpp


unix:SOURCES  += xsldbgthread.cpp
win32:SOURCES  += xsldbgthread_win32.cpp

TARGET = qtnotifier
