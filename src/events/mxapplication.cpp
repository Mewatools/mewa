/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxdebug.h"
#include "mxapplication.h"
#include "mxwidget.h"



MxApplication::MxApplication()
{
    MxWidget::sApplication = this;

    pRedrawRequested = false;
    pWidgetInFocus = NULL;
    pPainterBuffer.initArrays();
    pWidgetsToUpdate.reserve(8);
}

MxApplication::~MxApplication()
{


}

void MxApplication::init( MxAbstractAtlas *atlas )
{
    pRenderer.pIconAtlas = atlas;
}

void MxApplication::initialize()
{
    pRenderer.initialize();
    pPainterBuffer.initializeGL( &pRenderer );
}

void MxApplication::onRender()
{
    pRedrawRequested = false;


    pRenderer.renderBegin();


    mainWidget()->collectDirtyWidgets(pWidgetsToUpdate, true);
    if( pWidgetsToUpdate.size() > 0 )
    {

        drawWidgetList();
        pWidgetsToUpdate.clear();
    }
    pRenderer.renderEnd();

}

void MxApplication::onResizeWindow( int width, int height )
{
    if(width > 0 && height > 0)
    {
        pRenderer.setWindowSize( width, height );
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
    float openGLY = (float)(pRenderer.windowSize().height() - y);
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
    MxVector2F screenPos( (float)x, (float)openGLY );
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


void MxApplication::drawWidgetList()
{
    int listSize = pWidgetsToUpdate.size();

    // store points to array
    pPainterBuffer.prepareRender(pRenderer);
    MxVector2F screenPos;
    for(int i=0; i < listSize; i++)
    {
        MxWidget *widget = pWidgetsToUpdate[i];
        widget->getGlobalPos(&screenPos);
        pPainterBuffer.setTranslation(screenPos);
        widget->paint(pPainterBuffer);
    }
    pPainterBuffer.render( pRenderer );

    // OpenGL drawing
    listSize = pPainterBuffer.pWidgetsToCallRender.size();
    for(int i=0; i < listSize; ++i)
    {
        MxWidget *widget = pPainterBuffer.pWidgetsToCallRender[i];

        // viewport is set inside render()
        widget->render(pRenderer);
    }
    pPainterBuffer.pWidgetsToCallRender.clear();
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

/*! Returns 1 on desktop, higher values on mobile devices */
float MxApplication::pixelRatio() const
{
    return 1.0f;
}



