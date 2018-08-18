#-------------------------------------------------
#
# Project created by QtCreator 2018-06-25T13:20:10
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = app-ictest
TEMPLATE = app

HEADERS += \
    app/appwindow.h \
    qjson/QJsonArray.h \
    qjson/QJsonDocument.h \
    qjson/QJsonObject.h \
    qjson/QJsonParseError.h \
    qjson/QJsonParser.h \
    qjson/QJsonRoot.h \
    qjson/QJsonValue.h \
    qjson/QJsonValueRef.h \
    app/apprt3070.h \
    app/appscreen.h \
    app/appwether.h \
    app/boxqinput.h

SOURCES += \
    app/main.cpp \
    app/appwindow.cpp \
    qjson/QJsonArray.cpp \
    qjson/QJsonDocument.cpp \
    qjson/QJsonObject.cpp \
    qjson/QJsonParseError.cpp \
    qjson/QJsonParser.cpp \
    qjson/QJsonValue.cpp \
    qjson/QJsonValueRef.cpp \
    app/apprt3070.cpp \
    app/appscreen.cpp \
    app/appwether.cpp \
    app/boxqinput.cpp

RESOURCES += \
    qrc/appsource.qrc

