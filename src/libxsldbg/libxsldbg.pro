
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
DEFINES += DOCS_PATH=\"$(DOCSPATH)\"
win32:DEFINES += WIN32 QT_DLL
win32:LIBS += $(LIBXSLT_PREFIX)\lib\libexslt.lib $(LIBXSLT_PREFIX)\lib\libxslt.lib $(LIBXML_PREFIX)\lib\libxml2.lib 

DESTDIR =../../lib

# Input
HEADERS += arraylist.h \
           breakpoint.h \
           callstack.h \
           cmds.h \
           debug.h \
           debugXSL.h \
           files.h \
           help.h \
           options.h \
           qtnotifier2.h \
           search.h \
           utils.h \
           xsldbg.h \
           xsldbgconfig.h \
           xsldbgevent.h \
           xsldbgdebuggerbase.h \
           xsldbgio.h \
           xsldbgmsg.h \
           xsldbgthread.h \
	   xsldbgwin32config.h \
	   xsldbgnotifier.h  

SOURCES += arraylist.cpp \
           breakpoint.cpp \
           breakpoint_cmds.cpp \
           callstack.cpp \
           debug.cpp \
           debugXSL.cpp \
           file_cmds.cpp \
           files.cpp \
           nodeview_cmds.cpp \
           option_cmds.cpp \
           options.cpp \
           os_cmds.cpp \
           param_cmds.cpp \
           search.cpp \
           search_cmds.cpp \
           template_cmds.cpp \
           trace_cmds.cpp \
           utils.cpp \
           variable_cmds.cpp \
           xsldbg.cpp \
           xsldbgthread.cpp

unix:SOURCES += \
           files_unix.cpp \
           help_unix.cpp \
           options_unix.cpp 

win32:SOURCES += \
           files_win32.cpp \
           help_win32.cpp \
           options_win32.cpp 

TARGET = xsldbg
