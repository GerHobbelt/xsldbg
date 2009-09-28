TARGET = xsldbg
TEMPLATE	= app
CONFIG	+= warn_on release console thread
CONFIG += qt
QT=core

# enable readline and history support if possible
unix {
    system("$$QMAKE_CC -lreadline configtests/readlinetest.c -o configtests/readlinetest") {
	message(Enabling readline support)
	DEFINES+=HAVE_READLINE
	LIBS+=-lreadline
    }
    system("$$QMAKE_CC -lhistory configtests/historytest.c -o configtests/historytest") {
	message(Enabling history support)
	DEFINES+=HAVE_HISTORY
	LIBS+=-lhistory
    }
}

system("xslt-config --help >/dev/null"){
    unix {
       system( "echo -n \"QMAKE_CFLAGS+=\" > xslt-config.pri") 
       system( "xslt-config --cflags >> xslt-config.pri")
       system( "echo -n \"QMAKE_LFLAGS+=\" >> xslt-config.pri") 
       system( "xslt-config --libs >> xslt-config.pri")
       include(xslt-config.pri)
       USED_XSLT_CONFIG=true
    }
} else {
    message(xslt-config not found)
} 

!equals(USED_XSLT_CONFIG,true) {
    # try to guess the LIBS and INCLUDE paths needed to be added
    isEmpty(LIBXSLT_PREFIX) {
	 message(warning \$LIBXSLT_PREFIX environment variable not set)
    } else {
	INCLUDEPATH += $$(LIBXSLT_PREFIX)/include
	LIBS+=-L$(LIBXSLT_PREFIX)/lib -lxslt
    }
    isEmpty(LIBXML_PREFIX) {
	 message(warning \$LIBXML_PREFIX environment variable not set)
    } else {
	INCLUDEPATH += $$(LIBXML_PREFIX)/include/libxml2
	INCLUDEPATH += $$(LIBXML_PREFIX)/include
	LIBS+=-L$(LIBXML_PREFIX)/lib -lxml2 -lz -lm 
    }
    isEmpty(ICONV_PREFIX) {
	 message(warning \$ICONV_PREFIX environment variable not set)
    } else {
	INCLUDEPATH += $$(INCONV_PREFIX)/include
    }
} else {
    message(xslt-config was found it will be used to set correct LIBS and INCLUDE)
}
INCLUDEPATH += .. 
INCLUDEPATH += . 
INCLUDEPATH += ../..


# Documentation is now installed via ../docs/en/en.pro
unix {
	INSTALL_PREFIX="/usr/local/xsldbg"
	DEFINES+= DOCS_PATH="\"\\\"$$INSTALL_PREFIX/docs/en\\\"\""
	target.path = $$INSTALL_PREFIX/bin/
	INSTALLS += target
}

win32{
	INSTALL_PREFIX="\xsldbg"
	# DEFINES+= DOCS_PATH="\"\\\"$$INSTALL_PREFIX\\docs\\en\\\"\""
	target.path = $$INSTALL_PREFIX/bin
	INSTALLS += target
}

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
    libxsldbg/files_unix.cpp \
    libxsldbg/help_unix.cpp \
    libxsldbg/options_win32.cpp 

SOURCES+=libxsldbg/xsldbgthread.cpp


#unix:LIBS +=  -lreadline -lhistory -Llibxsldbg -lxsldbg -lexslt $(LIBXSLT_LIBS)
unix:LIBS +=  -lreadline -lhistory -lexslt $(LIBXSLT_LIBS)
win32:LIBS	=	kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib  $(LIBXSLT_PREFIX)\lib\libexslt.lib $(LIBXSLT_PREFIX)\lib\libxslt.lib $(LIBXML_PREFIX)\lib\libxml2.lib -L\xsldbg\bin -L\xsldbg\lib



