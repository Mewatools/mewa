
TEMPLATE = app

CONFIG += debug

include(../../src/qtmewa.pri)


INCLUDEPATH += .

SOURCES += $$PWD/main_qt.cpp \
           $$PWD/canvaswidget.cpp

HEADERS += $$PWD/canvaswidget.h


