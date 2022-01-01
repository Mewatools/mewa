

include($$PWD/textdraw/textdraw.pri)
include($$PWD/svg/svg.pri)

INCLUDEPATH += $$PWD

SOURCES += $$PWD/mxguiaggregation.cpp \
           $$PWD/mxpainter.cpp \
           $$PWD/mxwidget.cpp
  

HEADERS += $$PWD/mxguiaggregation.h \
           $$PWD/mxorthotransform.h \
           $$PWD/mxpainter.h \
           $$PWD/mxwidget.h \
           $$PWD/mxwidgetlist.h \
           $$PWD/mxthemecolors.h
   

