/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXPAINTER_H
#define MXPAINTER_H

#include "mxdebug.h"
#include "mxvectordraw.h"
#include "mxtextdraw.h"
#include "mxlist.h"


class MxWidget;
class MxAbstractSvg;


class MxPainter
{
public:
    MxPainter();
    ~MxPainter();

    void discardGLResources();
    void initArrays();
    void initializeGL(MxRenderer *);

    void prepareRender( MxRenderer &renderer );
    void render(MxRenderer &renderer );

    void setTranslation( const MxVector2F &translation );
    void drawSvg(MxAbstractSvg *svg , const MxRectF &targetRect);

    MxVectorDraw & vectorDraw();
    MxTextDraw & textDraw();

private:
    MxVectorDraw pVectorDraw;
    MxTextDraw pTextDraw;

    MxVector2F pTranslation;

};

#endif // QXPAINTERBUFFER_H
