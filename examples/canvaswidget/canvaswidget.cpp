/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "canvaswidget.h"
#include "mxpainter.h"
#include "mxmouseevent.h"



CanvasWidget::CanvasWidget()
{
    pDraggingRect = MxRectF(30.0f, 80.0f, 30.0f, 80.0f );
    pIsHovering  = false;
}

CanvasWidget::~CanvasWidget()
{}


MxVector2F CanvasWidget::sizePolicy()
{
    // take whole available area
    return MxVector2F(0.0f, 0.0f);
}

void CanvasWidget::sizeChanged()
{
    // called when resizing widget
}

void CanvasWidget::mousePressEvent( MxMouseEvent &event )
{
    const MxVector2F &mousePos = event.pos();
    if( pDraggingRect.contains(mousePos) )
    {
        pInitialPos = pDraggingRect.bottomLeft();
    }

    event.accept(this); // this widget will receive all mouse-move events until release
}

void CanvasWidget::mouseMoveEvent( MxMouseEvent &event )
{

    if( event.isAccepted() ) // dragging
    {
        MxVector2F displacement = event.globalPos() - event.pressedGlobalPos();
        MxVector2F rectPos = pInitialPos + displacement;
        pDraggingRect.setRectFromPointAndSize(rectPos, MxVector2F(50.0f, 50.0f) );
        update(); // redraw this widget
    }
    else { // hovering
        const MxVector2F &mousePos = event.pos();
        pIsHovering = pDraggingRect.contains(mousePos);
        // \TODO call update only if pIsHovering changed value
        update();
    }
}

void CanvasWidget::mouseReleaseEvent( MxMouseEvent &event )
{
}

void CanvasWidget::paint( MxPainter &painter )
{
    const MxVector2F &areaSize = size();
    MxRectF areaRect(areaSize);
    MxVectorDraw &lbTriangles = painter.vectorDraw();

    MxVector4UC bgColor(80, 80, 80, 255);
    MxVector4UC itemColor;
    if( pIsHovering ) {
        itemColor = MxVector4UC(180, 180, 20, 255);
    } else {
        itemColor = MxVector4UC(160, 160, 220, 255);
    }

    lbTriangles.fillRect( areaRect, bgColor);
    lbTriangles.fillRect( pDraggingRect, itemColor);
}
