QT       += core gui
QT += qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



QT += network

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    clientversionwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    requestparamssettingswidget.cpp \
    serversettingswidget.cpp \
    tunnel.cpp \
    tunnelworker.cpp \
    uisettingswidget.cpp \
    uploadfilewidget.cpp \
    uploadwidget.cpp \
    widget.cpp

HEADERS += \
    clientversionwidget.h \
    mainwindow.h \
    requestparamssettingswidget.h \
    serversettingswidget.h \
    settings_struct.h \
    tunnel.h \
    tunnelworker.h \
    uisettingswidget.h \
    uploadfilewidget.h \
    uploadwidget.h \
    widget.h

FORMS += \
    ui6/AbstractDialog.ui \
    ui6/AbstractFileDialog.ui \
    ui6/ClientVersionWidget.ui \
    ui6/DownloadFileWidget.ui \
    ui6/DownloadWidget.ui \
    ui6/LocalHostServerWidget.ui \
    ui6/MainWindow.ui \
    ui6/MainWindowPyQt5Edition.ui \
    ui6/RequestParamsSettingsWidget.ui \
    ui6/ServerSettingsWidget.ui \
    ui6/UiSettingsWidget.ui \
    ui6/UploadFileWidget.ui \
    ui6/UploadWidget.ui \
    widget.ui

TRANSLATIONS += \
    p2p_en.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
android: include(D:/Programming/androidSDK/android_openssl/openssl.pri)
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

RESOURCES += \
    MyQSS.qrc

DISTFILES += \
    ui6/AbstractDialog.py \
    ui6/AbstractFileDialog.py \
    ui6/ClientVersionWidget.py \
    ui6/DownloadFileWidget.py \
    ui6/DownloadWidget.py \
    ui6/LocalHostServerWidget.py \
    ui6/MainWindow.py \
    ui6/MainWindowPyQt5Edition.py \
    ui6/P2Ptunnel Логика Работы.png \
    ui6/RequestParamsSettingsWidget.py \
    ui6/ServerSettingsWidget.py \
    ui6/UiSettingsWidget.py \
    ui6/UploadFileWidget.py \
    ui6/UploadWidget.py \
    ui6/uigenpy.py

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/src/org/dail45/p2p/MyJNI.java
