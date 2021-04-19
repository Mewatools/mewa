
include($$PWD/colorselector/colorwheel.pri)
include($$PWD/renderer/renderer.pri)

INCLUDEPATH += $$PWD

SOURCES += $$PWD/mxshaderprogram.cpp \
           $$PWD/gpubuffer.cpp \
           $$PWD/gpuvbo.cpp
  

HEADERS += $$PWD/mxshaderprogram.h \
           $$PWD/mxthemecolors.h \
           $$PWD/gpubuffer.h \
           $$PWD/gpuvbo.h
   

