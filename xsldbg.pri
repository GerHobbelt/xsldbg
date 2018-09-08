XSLDBG_VERSION="4.7.0"
DEFINES+=XSLDBG_VERSION=\\\"$$XSLDBG_VERSION\\\"

!xsldbg_GUI {
    # Documentation is now installed via ../docs/en/en.pro
    unix {
            isEmpty(INSTALL_PREFIX) {
                DOCS_ROOT="/usr/share/doc/packages/xsldbg"
                BIN_DIR="/usr/bin"
                DESKTOP_DIR="/usr/share/applications"
                ICON_DIR="/usr/share/icons"
                MAN_DIR="/usr/share/man/man1"
            } else {
                DOCS_ROOT="$${INSTALL_PREFIX}/share/doc/packages/xsldbg"
                BIN_DIR="$${INSTALL_PREFIX}/bin"
                DESKTOP_DIR="$${INSTALL_PREFIX}/applications"
                ICON_DIR="$${INSTALL_PREFIX}/icons"
                MAN_DIR="$${INSTALL_PREFIX}/share/man/man1"
                message(xsldbg install prefix is '$${INSTALL_PREFIX}')
            }
            DEFINES+= DOCS_PATH="\"\\\"$$DOCS_ROOT/en\\\"\""
            DEFINES+= USE_DOCS_MACRO
    }

    win32{
        isEmpty(INSTALL_PREFIX) {
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
