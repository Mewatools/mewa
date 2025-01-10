/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef SIMPLEWIDGET_H
#define SIMPLEWIDGET_H

#include "mxwidget.h"



class SimpleWidget : public MxWidget
{
public:
    SimpleWidget();
    virtual ~SimpleWidget();

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
