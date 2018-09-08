TEMPLATE=subdirs

unix {
    # install desktop file related icons
    include($$PWD/../src/qxsldbg.pri)

    22x22icons.path=$$ICON_DIR/hicolor/22x22/apps
    22x22icons.files=$$files($$PWD/22x22/*.png)
    INSTALLS += 22x22icons

    128x128icons.path=$$ICON_DIR/hicolor/128x128/apps
    128x128icons.files=$$files($$PWD/128x128/*.png)
    INSTALLS += 128x128icons

    256x256icons.path=$$ICON_DIR/hicolor/256x256/apps
    256x256icons.files=$$files($$PWD/256x256/*.png)
    INSTALLS += 256x256icons

    512x512icons.path=$$ICON_DIR/hicolor/512x512/apps
    512x512icons.files=$$files($$PWD/512x512/*.png)
    INSTALLS += 512x512icons
}

DISTFILES += \
    $$PWD/22x22/*.png \
    $$PWD/128x128/*.png \
    $$PWD/256x256/*.png \
    $$PWD/512x512/*.png
