XSLDBG_VERSION="4.7.0"
DEFINES+=XSLDBG_VERSION=\\\"$$XSLDBG_VERSION\\\"

!xsldbg_GUI {
    INSTALL_PREFIX=$$(INSTALL_PREFIX)
    # Documentation is now installed via ../docs/en/en.pro
    unix {
            isEmpty(INSTALL_PREFIX)|equals(INSTALL_PREFIX, "") {
                DOCS_ROOT="/usr/share/doc/packages/xsldbg"
                BIN_DIR="/usr/bin"
            } else {
                DOCS_ROOT="$${INSTALL_PREFIX}/doc/packages/xsldbg"
                BIN_DIR="$${INSTALL_PREFIX}/bin"
                message(xsldbg install prefix is '$${INSTALL_PREFIX}')
            }
            DEFINES+= DOCS_PATH="\"\\\"$$DOCS_ROOT/en\\\"\""
            DEFINES+= USE_DOCS_MACRO
    }

    win32{
        isEmpty(INSTALL_PREFIX)|equals(INSTALL_PREFIX, "") {
            DOCS_ROOT="\\xsldbg\\docs"
            BIN_DIR="\\xsldbg\\bin"
        } else {
            DOCS_ROOT="$${INSTALL_PREFIX}\\docs"
            BIN_DIR=$${INSTALL_PREFIX}\bin
        }
        DEFINES+= DOCS_PATH="\"\\\"$${DOCS_ROOT}\\\\en\\\"\""
        DEFINES+= USE_DOCS_MACRO
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



