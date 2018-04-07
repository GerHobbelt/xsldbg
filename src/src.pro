include(src_common.pri)
include(../xsldbg.pri)

TARGET = xsldbg
TEMPLATE = app
CONFIG	+= console
CONFIG-=app_bundle

DISTFILES += \
    ../xsldbg.pri \
    src_common.pri
