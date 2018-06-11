XSLDBG_VERSION="4.6.1"
DEFINES+=XSLDBG_VERSION=\\\"$$XSLDBG_VERSION\\\"

!xsldbg_GUI {
    # Documentation is now installed via ../docs/en/en.pro
    unix {
            DOCS_ROOT="$$[QT_INSTALL_DOCS]/xsldbg"
            DEFINES+= DOCS_PATH="\"\\\"$$DOCS_ROOT/en\\\"\""
    }

    win32{
            # DEFINES+= DOCS_PATH="\"\\\"$$INSTALL_PREFIX\\docs\\en\\\"\""
    }
}

DISTFILES += \
    $$PWD/tests/test14.xml \
    $$PWD/tests/note.dtd \
    $$PWD/tests/notemessage1.xml \
    $$PWD/tests/note3.xml \
    $$PWD/tests/test14.xsl

