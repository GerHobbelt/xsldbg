
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
           xsldbgthread.h 

SOURCES += arraylist.c \
           breakpoint.c \
           breakpoint_cmds.c \
           callstack.c \
           debug.c \
           debugXSL.c \
           file_cmds.c \
           files.c \
           nodeview_cmds.c \
           option_cmds.c \
           options.c \
           os_cmds.c \
           param_cmds.c \
           search.c \
           search_cmds.c \
           template_cmds.c \
           trace_cmds.c \
           utils.c \
           variable_cmds.c \
           xsldbg.c \
           xsldbgthread.c

unix:SOURCES += \
           files_unix.c \
           help_unix.c \
           options_unix.c 

win32:SOURCES += \
           files_win32.c \
           help_win32.c \
           options_win32.c 

TARGET = xsldbg
