
TEMPLATE = app
QT += opengl

CONFIG += debug

include(../../src/qtmewa.pri)


INCLUDEPATH += .

SOURCES += $$PWD/main.cpp \
           $$PWD/simplewidget.cpp

HEADERS += $$PWD/simplewidget.h


