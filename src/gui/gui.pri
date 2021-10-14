
include($$PWD/colorselector/colorwheel.pri)
include($$PWD/renderer/renderer.pri)
include($$PWD/vectordraw/vectordraw.pri)
include($$PWD/icondraw/icondraw.pri)
include($$PWD/textdraw/textdraw.pri)
include($$PWD/svg/svg.pri)

INCLUDEPATH += $$PWD

SOURCES += $$PWD/mxaggregation.cpp \
           $$PWD/mxpainter.cpp \
           $$PWD/mxwidget.cpp \
           $$PWD/mxshaderprogram.cpp \
           $$PWD/gpubuffer.cpp \
           $$PWD/gpuvbo.cpp
  

HEADERS += $$PWD/mxaggregation.h \
           $$PWD/mxorthotransform.h \
           $$PWD/mxpainter.h \
           $$PWD/mxwidget.h \
           $$PWD/mxwidgetlist.h \
           $$PWD/mxshaderprogram.h \
           $$PWD/mxthemecolors.h \
           $$PWD/gpubuffer.h \
           $$PWD/gpuvbo.h
   

