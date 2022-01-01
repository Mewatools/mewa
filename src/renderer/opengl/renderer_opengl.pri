
INCLUDEPATH += $$PWD

include($$PWD/colorwheelprogram/colorwheelprogram.pri)
include($$PWD/vectordraw/vectordraw.pri)
include($$PWD/icondraw/icondraw.pri)

HEADERS += $$PWD/mxrenderer.h \
           $$PWD/mxshaderprogram.h \
           $$PWD/mxcachedgpuarray.h \
           $$PWD/mxgpuarray.h

SOURCES += $$PWD/mxrenderer.cpp \
           $$PWD/mxshaderprogram.cpp \
           $$PWD/mxcachedgpuarray.cpp \
           $$PWD/mxgpuarray.cpp
