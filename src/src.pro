include(src_common.pri)
include(../xsldbg.pri)

TEMPLATE = app
TARGET = xsldbg
target.path = $$BIN_DIR
CONFIG	+= console
CONFIG-=app_bundle
INSTALLS += target

unix {
	xsldbg_shortcut {	
		Desktop_entries.path = $$DESKTOP_DIR
		Desktop_entries.files = xsldbg.desktop
		INSTALLS += Desktop_entries
	}
}

DISTFILES += \
    ../xsldbg.pri \
    src_common.pri
