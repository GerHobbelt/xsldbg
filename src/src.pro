TARGET = xsldbg
TEMPLATE = app
CONFIG	+= warn_on release console thread
CONFIG += qt
QT *=core

# enable readline and history support if possible
unix {
    system("$$QMAKE_CC -lreadline configtests/readlinetest.c -o configtests/readlinetest > /dev/null 2>&1") {
	message(Enabling readline support)
	DEFINES+=HAVE_READLINE
	LIBS+=-lreadline
    }
    system("$$QMAKE_CC -lhistory configtests/historytest.c -o configtests/historytest > /dev/null 2>&1") {
	message(Enabling history support)
	DEFINES+=HAVE_HISTORY
	LIBS+=-lhistory
    }
}

system("xslt-config --help >/dev/null"){
    unix {
       system( 'echo "QMAKE_CXXFLAGS+=\c" > xslt-config.pri')
       system( 'xslt-config --cflags >> xslt-config.pri')
       system( 'echo "QMAKE_LFLAGS+=\c" >> xslt-config.pri')
       system( 'xslt-config --libs >> xslt-config.pri')
       LIBS+=-lexslt
       include(xslt-config.pri)
       USED_XSLT_CONFIG=true
    }
} else {
    message(xslt-config not found)
} 

system("xml2-config --help >/dev/null"){
    unix {
       system( 'echo "QMAKE_CXXFLAGS+=\c" > xml2-config.pri')
       system( 'xml2-config --cflags >> xml2-config.pri')
       system( 'echo "QMAKE_LFLAGS+=\c" >> xml2-config.pri')
       system( 'xml2-config --libs >> xml2-config.pri')
       LIBS+=-lexslt
       include(xml2-config.pri)
       USED_XML2_CONFIG=true
    }
} else {
    message(xml2-config not found)
}

#       error($${QMAKE_CFLAGS} here!! $${QMAKE_LFLAGS} there!!)

win32 {
   OBJECTS_DIR = ./build/obj
   MOC_DIR = ./build/moc
   LIBS+=-L$(QTDIR)/lib
   CONFIG+=console
  }

!equals(USED_XSLT_CONFIG,true) {
    TEMP_LIBXSLT_PREFIX=$$(LIBXSLT_PREFIX)
    # try to guess the LIBS and INCLUDE paths needed to be added
    isEmpty(TEMP_LIBXSLT_PREFIX) {
	 message(warning \$LIBXSLT_PREFIX environment variable not set)
	 message($$(LIBXSLT_PREFIX))
    } else {
    	unix{
	   INCLUDEPATH += $$(LIBXSLT_PREFIX)/include
	   LIBS+=-L$$(LIBXSLT_PREFIX)/lib -lxslt
	} else {
	   exists($$(LIBXSLT_PREFIX)/win32/bin.msvc){
	      LIBS+=-L$$(LIBXSLT_PREFIX)/win32/bin.msvc
	   } else {
	      message(probably will not find libxslt libraries)
	      LIBS+=-L$$(LIBXSLT_PREFIX)/lib/
	   }
	   LIBS+=-llibexslt -llibxslt
	}
    }
    TEMP_LIBXML_PREFIX=$$(LIBXML_PREFIX)
    isEmpty(TEMP_LIBXML_PREFIX) {
	 message(warning \$LIBXML_PREFIX environment variable not set)
    } else {
	INCLUDEPATH += $$(LIBXML_PREFIX)/include/libxml2
	INCLUDEPATH += $$(LIBXML_PREFIX)/include
	unix {
	    LIBS+=-L$$(LIBXML_PREFIX)/lib -lxml2 -lz -lm 
	} else {
	   exists($$(LIBXML_PREFIX)/win32/bin.msvc){
	      LIBS+=-L$$(LIBXML_PREFIX)/win32/bin.msvc
	   } else {
	      message(Probably will not find libxml libraries)
	      LIBS+=-L$$(LIBXML_PREFIX)/lib/
	   }
	   LIBS+=-llibxml2
       }
    }
    TEMP_ICONV_PREFIX=$$(ICONV_PREFIX)
    isEmpty(TEMP_ICONV_PREFIX) {
	   message(warning \$ICONV_PREFIX environment variable not set)
    } else {
		exists($$(ICONV_PREFIX)/include/iconv.h){
			INCLUDEPATH += $$(ICONV_PREFIX)/include
		    LIBS+=$$(ICONV_PREFIX)/lib/iconv.lib
		}else {
			error(Did not found iconv.h in \$ICONV_PREFIX, ie $$(ICONV_PREFIX)/include)
		}
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





