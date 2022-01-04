

include($$PWD/textdraw/textdraw.pri)
include($$PWD/svg/svg.pri)

INCLUDEPATH += $$PWD

SOURCES += $$PWD/mxbuffer.cpp \
           $$PWD/mxguiaggregation.cpp \
           $$PWD/mxpainter.cpp \
           $$PWD/mxwidget.cpp
  

HEADERS += $$PWD/mxbuffer.h \
           $$PWD/mxguiaggregation.h \
           $$PWD/mxorthotransform.h \
           $$PWD/mxpainter.h \
           $$PWD/mxwidget.h \
           $$PWD/mxwidgetlist.h \
           $$PWD/mxthemecolors.h
   

