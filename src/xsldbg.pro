TEMPLATE	= app
CONFIG	+= warn_on release console thread
CONFIG -= qt


unix:INCLUDEPATH += $$(LIBXSLT_PREFIX)/include
unix:INCLUDEPATH += $$(LIBXML_PREFIX)/include/libxml2
unix:INCLUDEPATH += $$(INCONV_PREFIX)/include
win32:INCLUDEPATH += $$(LIBXSLT_PREFIX)/include
win32:INCLUDEPATH += $$(LIBXML_PREFIX)/include
win32:INCLUDEPATH += $$(ICONV_PREFIX)/include
INCLUDEPATH += .. 
INCLUDEPATH += . 
INCLUDEPATH += ../..
unix:DEFINES += DOCS_PATH=\"$(DOCSPATH)\"
win32:DEFINES+=WIN32 _WINDOWS _MBCS _REENTRANT

SOURCES	=  main.c simpleio.c

HEADERS += \
    libxsldbg/arraylist.h \
    libxsldbg/breakpoint.h \
    libxsldbg/callstack.h \
    libxsldbg/cmds.h \
    libxsldbg/debug.h \
    libxsldbg/debugXSL.h \
    libxsldbg/files.h \
    libxsldbg/help.h \
    libxsldbg/options.h \
    libxsldbg/search.h \
    libxsldbg/utils.h \
    libxsldbg/xsldbg.h \
    libxsldbg/xsldbgconfig.h \
    libxsldbg/xsldbgevent.h \
    libxsldbg/xsldbgio.h \
    libxsldbg/xsldbgmsg.h \
    libxsldbg/xsldbgthread.h 

SOURCES += \
    libxsldbg/arraylist.c \
    libxsldbg/breakpoint.c \
    libxsldbg/breakpoint_cmds.c \
    libxsldbg/callstack.c \
    libxsldbg/debug.c \
    libxsldbg/debugXSL.c \
    libxsldbg/file_cmds.c \
    libxsldbg/files.c \
    libxsldbg/nodeview_cmds.c \
    libxsldbg/option_cmds.c \
    libxsldbg/options.c \
    libxsldbg/os_cmds.c \
    libxsldbg/param_cmds.c \
    libxsldbg/search.c \
    libxsldbg/search_cmds.c \
    libxsldbg/template_cmds.c \
    libxsldbg/trace_cmds.c \
    libxsldbg/utils.c \
    libxsldbg/variable_cmds.c \
    libxsldbg/xsldbg.c \
    libxsldbg/xsldbgthread.c

unix:SOURCES += \
    libxsldbg/files_unix.c \
    libxsldbg/help_unix.c \
    libxsldbg/options_unix.c 

win32:SOURCES += \
    libxsldbg/files_win32.c \
    libxsldbg/help_win32.c \
    libxsldbg/options_win32.c 


unix:LIBS +=  -lreadline -lhistory -Llibxsldbg -lxsldbg -lexslt $(LIBXSLT_LIBS)
win32:LIBS	=	kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib  $(LIBXSLT_PREFIX)\lib\libexslt.lib $(LIBXSLT_PREFIX)\lib\libxslt.lib $(LIBXML_PREFIX)\lib\libxml2.lib

TARGET = xsldbg
