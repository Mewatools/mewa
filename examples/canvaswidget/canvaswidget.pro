
TEMPLATE = app

CONFIG += debug

include(../../src/qtmewa.pri)


INCLUDEPATH += .

SOURCES += $$PWD/main.cpp \
           $$PWD/canvaswidget.cpp

HEADERS += $$PWD/canvaswidget.h


