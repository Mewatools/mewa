/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXPAINTER_H
#define MXPAINTER_H

#include "mxdebug.h"
#include "mxvectordraw.h"
#include "mxlist.h"
#include "mxtextdraw.h"
#include "mxicondraw.h"


class MxWidget;
class MxAbstractSvg;
class MxGuiRenderer;


class MxPainter
{
public:
    MxPainter();
    ~MxPainter();

    void initArrays();
    void initializeGL( MxRenderer *renderer );

    void prepareRender(MxGuiRenderer &renderer );
    void render(MxGuiRenderer &renderer );

    void setTranslation( const MxVector2F &translation );
    MxIconDraw &iconDraw();
    void drawSvg(MxAbstractSvg *svg , const MxRectF &targetRect);
    MxVectorDraw & vectorDraw();
    MxTextDraw & textDraw();




private:
    MxVectorDraw pVectorDraw;
    MxIconDraw pIconDraw;


    MxTextDraw pTextDraw; // colored text
    MxVector2F pTranslation;

};

#endif // QXPAINTERBUFFER_H
