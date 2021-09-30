/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include "mxwidget.h"



class CanvasWidget : public MxWidget
{
public:
    CanvasWidget();
    virtual ~CanvasWidget();

    virtual MxVector2F sizePolicy();
    virtual void sizeChanged();

    virtual void mousePressEvent( MxMouseEvent &event );
    virtual void mouseMoveEvent( MxMouseEvent &event );
    virtual void mouseReleaseEvent( MxMouseEvent &event );

    virtual void paint( MxPainter &painter );

private:
    MxRectF pDraggingRect;
    MxVector2F pInitialPos;
    bool pIsHovering;

};

#endif
