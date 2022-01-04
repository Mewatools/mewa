
INCLUDEPATH += $$PWD

include($$PWD/colorwheelprogram/colorwheelprogram.pri)
include($$PWD/vectorprogram/vectorprogram.pri)
include($$PWD/iconprogram/iconprogram.pri)

HEADERS += $$PWD/mxrenderer.h \
           $$PWD/mxshaderprogram.h \
           $$PWD/mxgpuarray.h

SOURCES += $$PWD/mxrenderer.cpp \
           $$PWD/mxshaderprogram.cpp \
           $$PWD/mxgpuarray.cpp
