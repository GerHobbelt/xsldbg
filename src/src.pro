TEMPLATE	= app
CONFIG		+= qt warn_on release console

unix:INCLUDEPATH += $(LIBXSLT_PREFIX)/include
unix:INCLUDEPATH += $(LIBXML_PREFIX)/include/libxml2
unix:INCLUDEPATH += $(INCONV_PREFIX)/include
win32:INCLUDEPATH += $(LIBXSLT_PREFIX)/include
win32:INCLUDEPATH += $(LIBXML_PREFIX)/include
win32:INCLUDEPATH += $(ICONV_PREFIX)/include
INCLUDEPATH += .. 
INCLUDEPATH += . 
INCLUDEPATH += ../..
unix:DEFINES += DOCS_PATH=\"$(DOCSPATH)\"
win32:DEFINES+=WIN32

SOURCES		=  main.c \
			  simpleio.c
unix:LIBS +=  -lreadline -lhistory -Llibxsldbg -lxsldbg -L libqtnotifier -lqtnotifier -lexslt $(LIBXSLT_LIBS)
win32:LIBS	=	kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib ..\lib\qtnotifier.lib ..\lib\xsldbg.lib $(LIBXSLT_PREFIX)\lib\libexslt.lib $(LIBXSLT_PREFIX)\lib\libxslt.lib $(LIBXML_PREFIX)\lib\libxml2.lib 
#win32:LIBS += libxsldbg/libxsldbg.lib  libqtnotifier/libqtnotifier.lib  $(LIBXSLT_LIBS)

TARGET = xsldbg
