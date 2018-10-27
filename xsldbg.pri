XSLDBG_VERSION="4.8.1"
DEFINES+=XSLDBG_VERSION=\\\"$$XSLDBG_VERSION\\\"

isEmpty(INSTALL_PREFIX) {
    INSTALL_PREFIX = $$getenv(INSTALL_PREFIX)
}

!xsldbg_GUI {
    # Documentation is now installed via ../docs/en/en.pro
    unix {
            isEmpty(INSTALL_PREFIX) {
                INSTALL_PREFIX="/usr"
            }
            DOCS_ROOT="$${INSTALL_PREFIX}/share/doc/xsldbg"
            KDEDOCS_ROOT="$${INSTALL_PREFIX}/share/doc/HTML"
            BIN_DIR="$${INSTALL_PREFIX}/bin"
            DESKTOP_DIR="$${INSTALL_PREFIX}/share/applications"
            ICON_DIR="$${INSTALL_PREFIX}/share/icons"
            MAN_DIR="$${INSTALL_PREFIX}/share/man/man1"
            message(xsldbg install prefix is '$${INSTALL_PREFIX}')
            DEFINES+= DOCS_PATH=\\\"$$DOCS_ROOT/en\\\"
            DEFINES+= USE_DOCS_MACRO
    }

    win32{
        isEmpty(INSTALL_PREFIX) {
            INSTALL_PREFIX="c:\\xsldbg"
        }
        DOCS_ROOT="$${INSTALL_PREFIX}\\docs"
        BIN_DIR="$${INSTALL_PREFIX}\\bin"
        DOCS_PATH="$${DOCS_ROOT}\\en"
        DOCS_PATH=$$replace(DOCS_PATH,$$re_escape("\\"),$$re_escape("\\\\"))
        message(xsldbg install prefix is '$${INSTALL_PREFIX}')
        message(xsldbg docs path is '$${DOCS_PATH}')
        DEFINES+= DOCS_PATH=\\\"$${DOCS_PATH}\\\"
        DEFINES+= USE_DOCS_MACRO
    }
}
