XSLDBG_VERSION="4.7.0"
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
    $$PWD/tests/*.xml \
    $$PWD/tests/*.dtd \
    $$PWD/tests/*.data \
    $$PWD/tests/*.txt \
    $$PWD/tests/*.pl \
    $$PWD/tests/*.xml \
    $$PWD/tests/*.xsl



