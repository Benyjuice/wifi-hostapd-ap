QT       += core gui

TARGET = wifi-hostapd-ap
TEMPLATE = app

HEADERS  += src/mainwindow.h \
    src/aboutwindow.h \
    src/optionswindow.h \
    src/activateap.h \
    src/disactiveap.h \
    src/editorconfig.h \
    src/dialogindicator.h \
    src/getroot.h \
    src/logsystem.h \
    src/checkprocess.h

SOURCES += src/main.cpp\
    src/mainwindow.cpp \
    src/aboutwindow.cpp \
    src/optionswindow.cpp \
    src/activateap.cpp \
    src/disactiveap.cpp \
    src/editorconfig.cpp \
    src/dialogindicator.cpp \
    src/getroot.cpp \
    src/logsystem.cpp \
    src/CheckProcess.cpp \
    src/PlatformVariable.cpp


FORMS    += src/mainwindow.ui \
    src/aboutwindow.ui \
    src/optionswindow.ui \
    src/editorconfig.ui \
    src/dialogindicator.ui \
    src/getroot.ui \
    src/logsystem.ui

OTHER_FILES += \
    src/pic/WiFi_logo_litle.png \
    src/app_ru.qm \
    src/app_en.qm \
    src/app_cn.qm \
    src/about/about_en.html \
    src/about/about_ru.htm \
    src/about/about_zh_CN.html

TRANSLATIONS += src/app_ru.ts\
    src/app_en.ts\
    src/app_cn.ts\
    src/about/about_en_EN.html\
    src/about/about_ru_RU.html\
    src/about/about_zh_CN.html

RC_FILE = src/myapp.rc

RESOURCES = src/images.qrc

unix {
    isEmpty (PREFIX):PREFIX = /usr

    INSTALLS           += target desktop logo translations

    desktop.path        = $$PREFIX/share/applications
    desktop.files      += WiFi-Hostapd-AP.desktop
    desktop.files      += WiFi-Hostapd-AP-KDE.desktop

    logo.path           = $$PREFIX/share/icons/hicolor/scalable/apps
    logo.files         += src/pic/WiFi_logo_litle.png

    translations.path   = $$PREFIX/share/wifi-hostapd-ap/translations
    translations.files += src/app_ru.qm \
                          src/app_en.qm \
                          src/app_cn.qm\
                          about/about_en.html\
                          about/about_ru.html

    target.path         = $$PREFIX/bin
}
