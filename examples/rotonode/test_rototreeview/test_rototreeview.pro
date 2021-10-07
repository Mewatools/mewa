
TEMPLATE = app
CONFIG += debug
DEFINES += TEST_ROTOTREEVIEW

include($$PWD/../../../src/qtmewa.pri)

INCLUDEPATH += $$PWD ..

SOURCES += $$PWD/main.cpp \
           $$PWD/../rototreeview.cpp \
           $$PWD/../rotoshape.cpp \
           $$PWD/../rototransform.cpp \
           $$PWD/../rototreeitem.cpp \
           $$PWD/../shapelockedsvg.cpp

HEADERS += $$PWD/../rototreeview.h \
           $$PWD/../shapelist.h \
           $$PWD/../rotoshape.h \
           $$PWD/../shapelockedsvg.h


