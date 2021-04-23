/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXAPPLICATION_H
#define MXAPPLICATION_H

#include "mxdebug.h"
#include "mxwidgetlist.h"
#include "mxrenderer.h"
#include "mxmouseevent.h"
#include "mxpainter.h"


#define QX_COLORBUFFER_PRESERVED


class MxApplication
{
public:

    MxApplication();
    virtual ~MxApplication();

    static MxApplication* instance();


    // graphics
    virtual void initializeGL();
    virtual void cleanupGL();
    virtual void paintGL();
    virtual void onResizeWindow( int width, int height );

    // events
    virtual void onMousePress( int x, int y, unsigned int button, unsigned int modifiers );
    virtual void onMouseMove( int x, int y, unsigned int modifiers );
    virtual void onMouseRelease( int x, int y );

private:
    void onDrawGL();
    void onResizeGL();
    void drawWidgetList();


public:
    void setMainWidget( MxWidget *widget );
    MxWidget* mainWidget();
    void requestRedraw();
    virtual bool isRedrawNeeded();


    static MxApplication *sApp;
    MxRenderer pRenderer;
    MxWidget *pWidget;

    bool mSizeChanged;
    MxPainter pPainterBuffer;
private:
    bool pRedrawRequested;

    MxWidget *pWidgetInFocus;
    MxWidgetList pWidgetsToUpdate;
    MxWidgetList pQueueToRedrawLater;



    MxMouseEvent pMouseEvent;



};

#endif

