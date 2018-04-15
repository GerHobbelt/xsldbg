include(src_common.pri)
include(../xsldbg.pri)

TARGET = xsldbg
target.path = $$[QT_INSTALL_BINS]
TEMPLATE = app
CONFIG	+= console
CONFIG-=app_bundle
INSTALLS += target

DISTFILES += \
    ../xsldbg.pri \
    src_common.pri
