
TEMPLATE = app

QT += opengl

CONFIG += debug


include($$PWD/../../../src/qtmewa.pri)
include($$PWD/../../../src/pluginsdk/pluginsdk.pri)
include($$PWD/../triangleplugin.pri)


SOURCES += $$PWD/main.cpp





