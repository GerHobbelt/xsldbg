include(src_common.pri)
include(../xsldbg.pri)

TEMPLATE = app
TARGET = xsldbg
target.path = $$BIN_DIR
CONFIG	+= console
CONFIG-=app_bundle
INSTALLS += target

DISTFILES += \
    ../xsldbg.pri \
    src_common.pri
