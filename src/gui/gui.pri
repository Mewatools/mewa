
include($$PWD/colorselector/colorwheel.pri)
include($$PWD/vectordraw/vectordraw.pri)
include($$PWD/icondraw/icondraw.pri)
include($$PWD/textdraw/textdraw.pri)
include($$PWD/svg/svg.pri)

INCLUDEPATH += $$PWD

SOURCES += $$PWD/mxaggregation.cpp \
           $$PWD/mxpainter.cpp \
           $$PWD/mxwidget.cpp
  

HEADERS += $$PWD/mxaggregation.h \
           $$PWD/mxorthotransform.h \
           $$PWD/mxpainter.h \
           $$PWD/mxwidget.h \
           $$PWD/mxwidgetlist.h \
           $$PWD/mxthemecolors.h
   

