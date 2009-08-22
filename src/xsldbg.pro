TEMPLATE	= app
CONFIG	+= warn_on release console thread
CONFIG += qt
unix {
	INSTALL_PREFIX="/usr/local/xsldbg"
	DEFINES+= DOCS_PATH="\"\\\"$$INSTALL_PREFIX/docs/en\\\"\""
	target.path = $$INSTALL_PREFIX/bin/
	INSTALLS += target

	docs.files = ../docs/en/*.xsl \
		     ../docs/en/*.xml \
		     ../docs/en/*.docbook \
		     ../docs/en/*.txt
	docs.path= $$INSTALL_PREFIX/docs/en/
	INSTALLS += docs
}


unix:INCLUDEPATH += $$(LIBXSLT_PREFIX)/include
unix:INCLUDEPATH += $$(LIBXML_PREFIX)/include/libxml2
unix:INCLUDEPATH += $$(INCONV_PREFIX)/include
win32:INCLUDEPATH += $$(LIBXSLT_PREFIX)/include
win32:INCLUDEPATH += $$(LIBXML_PREFIX)/include
win32:INCLUDEPATH += $$(ICONV_PREFIX)/include
INCLUDEPATH += .. 
INCLUDEPATH += . 
INCLUDEPATH += ../..
win32:DEFINES+=WIN32 _WINDOWS _MBCS _REENTRANT


SOURCES	=  main.cpp simpleio.cpp

HEADERS += \
    libxsldbg/breakpoint.h \
    libxsldbg/arraylist.h \
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
    libxsldbg/xsldbgthread.h \
    libxsldbg/xsldbgsettingsmodel.h

SOURCES += \
    libxsldbg/breakpoint.cpp \
    libxsldbg/arraylist.cpp \
    libxsldbg/breakpoint_cmds.cpp \
    libxsldbg/callstack.cpp \
    libxsldbg/debug.cpp \
    libxsldbg/debugXSL.cpp \
    libxsldbg/file_cmds.cpp \
    libxsldbg/files.cpp \
    libxsldbg/nodeview_cmds.cpp \
    libxsldbg/option_cmds.cpp \
    libxsldbg/options.cpp \
    libxsldbg/os_cmds.cpp \
    libxsldbg/param_cmds.cpp \
    libxsldbg/search.cpp \
    libxsldbg/search_cmds.cpp \
    libxsldbg/template_cmds.cpp \
    libxsldbg/trace_cmds.cpp \
    libxsldbg/utils.cpp \
    libxsldbg/variable_cmds.cpp \
    libxsldbg/xsldbg.cpp \
    libxsldbg/xsldbgsettingsmodel.cpp \
    libxsldbg/options_common.cpp

unix:SOURCES += \
    libxsldbg/files_unix.cpp \
    libxsldbg/help_unix.cpp \
    libxsldbg/options_unix.cpp 

win32:SOURCES += \
    libxsldbg/files_win32.cpp \
    libxsldbg/help_win32.cpp \
    libxsldbg/options_win32.cpp 

SOURCES+=libxsldbg/xsldbgthread.cpp


#unix:LIBS +=  -lreadline -lhistory -Llibxsldbg -lxsldbg -lexslt $(LIBXSLT_LIBS)
unix:LIBS +=  -lreadline -lhistory -lexslt $(LIBXSLT_LIBS)
win32:LIBS	=	kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib  $(LIBXSLT_PREFIX)\lib\libexslt.lib $(LIBXSLT_PREFIX)\lib\libxslt.lib $(LIBXML_PREFIX)\lib\libxml2.lib

TARGET = xsldbg
