/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxdebug.h"
#include "mxapplication.h"
#include "mxwidget.h"


MxApplication * MxApplication::sApp = NULL;

MxApplication::MxApplication()
{
    mSizeChanged = false;
    pRedrawRequested = false;
    pWidgetInFocus = NULL;
    pPainterBuffer.initArrays();
    pWidgetsToUpdate.reserve(8);
}

MxApplication::~MxApplication()
{


}

MxApplication* MxApplication::instance()
{
    if( sApp == NULL ) {
        sApp = new MxApplication();
    }
    return sApp;
}

void MxApplication::initializeGL()
{
    pRenderer.initializeGL();
    pPainterBuffer.initializeGL( &pRenderer );
}

void MxApplication::cleanupGL()
{
}


void MxApplication::paintGL()
{
    pRedrawRequested = false;

    if(mSizeChanged)
    {
        mSizeChanged = false;
        onResizeGL();
    }
    onDrawGL();
}

void MxApplication::onResizeWindow( int width, int height )
{
    if(width > 0 && height > 0)
    {
        mSizeChanged = true;
        pRenderer.setWindowSize( width, height );
        pRenderer.pScreenProjectionMatrix.setToIdentity();
        pRenderer.pScreenProjectionMatrix.ortho(0.0, width, 0.0, height, -1.0, 1.0);
        mainWidget()->setPos( MxVector2F(0.0f, 0.0f ) );
        mainWidget()->setSize( MxVector2F(width, height));
        mainWidget()->update();
    }
}

void MxApplication::onMousePress( int x, int y, unsigned int button, unsigned int modifiers )
{
    // if a second mouse button is pressed ignore it
    if( pMouseEvent.isAccepted() ) {
        return;
    }
    /*
     Its better to correct yy of mouse events over translating drawing,
     because all widgets, including the main window, can use bottom left
     corner has reference.
    */
    float openGLY = pRenderer.windowSize().height() - y;
    MxVector2F mousePos( (float)x, openGLY);

    pMouseEvent.pMousePressedPos = mousePos;


    Q_ASSERT( ! pMouseEvent.isAccepted() );
    pMouseEvent.pPos = mousePos - mainWidget()->pos();
    pMouseEvent.pGlobalPos = mousePos;
    pMouseEvent.pButton = (MxMouseEvent::MouseButton)button;
    pMouseEvent.pModifiers = modifiers;

    pMouseEvent.pAcceptedItem = NULL;
    mainWidget()->mousePressEvent(pMouseEvent);



}


void MxApplication::onMouseMove( int x, int y, unsigned int modifiers )
{
    int openGLY = pRenderer.windowSize().height() - y;
    MxVector2F screenPos( (float)x, openGLY );
    if( pMouseEvent.isAccepted()  )
    {
        // overwrite only global pos
        pMouseEvent.pGlobalPos = screenPos;
        pMouseEvent.pModifiers = modifiers;
        pMouseEvent.pAcceptedItem->mouseMoveEvent(pMouseEvent);
    }
    else
    {

        Q_ASSERT( ! pMouseEvent.isAccepted() );
        pMouseEvent.pGlobalPos = screenPos;
        pMouseEvent.pPos = screenPos;
        pMouseEvent.pModifiers = modifiers;
        mainWidget()->mouseMoveEvent(pMouseEvent);


    }
}

void MxApplication::onMouseRelease( int x, int y )
{
    float openGLY = pRenderer.windowSize().height() - y;
    MxVector2F screenPos((float)x, openGLY);

    // overwrite only global pos
    pMouseEvent.pGlobalPos = screenPos;
    pMouseEvent.pButton = MxMouseEvent::NoButton;
    if( pMouseEvent.isAccepted() )
    {
        pMouseEvent.pAcceptedItem->mouseReleaseEvent(pMouseEvent);
        pMouseEvent.pAcceptedItem = NULL;
    }
}


void MxApplication::onDrawGL()
{
    glClear(0x0);

    pRenderer.glDisable(GL_CULL_FACE);

    mainWidget()->collectDirtyWidgets( pWidgetsToUpdate );
    if( pWidgetsToUpdate.size() > 0 )
    {

        drawWidgetList();
        pWidgetsToUpdate.clear();
    }
}


void MxApplication::onResizeGL()
{
    const GLsizei w = mainWidget()->size().width();
    const GLsizei h = mainWidget()->size().height();
    glViewport(0, 0, w, h);
}

void MxApplication::drawWidgetList()
{
    //pRenderer.pPainterBuffer.pDrawingOverlay = overlay;
    int listSize = pWidgetsToUpdate.size();

    // store points to array
    pPainterBuffer.prepareRender(pRenderer);
    MxVector2F screenPos;
    for(int i=0; i < listSize; ++i)
    {
        MxWidget *widget = pWidgetsToUpdate[i];
        widget->getGlobalPos(screenPos);
        pPainterBuffer.setTranslation(screenPos);
        widget->paint(pPainterBuffer);
    }
    pPainterBuffer.render( pRenderer );

    pRenderer.clearGpuBuffers();
}

void MxApplication::setMainWidget( MxWidget *widget )
{
    pWidget = widget;
}

MxWidget* MxApplication::mainWidget()
{
    return pWidget;
}

void MxApplication::requestRedraw()
{
    pRedrawRequested = true;
}

// called only at the end of input events
bool MxApplication::isRedrawNeeded()
{
    return pRedrawRequested;
}





