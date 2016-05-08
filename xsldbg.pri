XSLDBG_VERSION="4.5.0"
DEFINES+=XSLDBG_VERSION=\\\"$$XSLDBG_VERSION\\\"

# Documentation is now installed via ../docs/en/en.pro
unix {
        INSTALL_PREFIX="/opt/local/xsldbg"
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

