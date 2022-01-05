/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXPAINTER_H
#define MXPAINTER_H

#include "mxdebug.h"
#include "mxvectordraw.h"
#include "mxicondraw.h"
#include "mxlist.h"
#include "mxtextdraw.h"


class MxWidget;
class MxAbstractSvg;
class MxGuiRenderer;


class MxPainter
{
public:
    enum IconColor {
        OriginalColor = 0,
        BlueColor,
        LightColor,
        ColorCount
    };

    MxPainter();
    ~MxPainter();

    void discardGLResources();
    void initArrays();
    void initializeGL( MxRenderer *renderer );

    void prepareRender(MxGuiRenderer &renderer );
    void render(MxGuiRenderer &renderer );

    void setTranslation( const MxVector2F &translation );
    MxIconDraw &iconDraw(IconColor color );
    void drawSvg(MxAbstractSvg *svg , const MxRectF &targetRect);
    MxVectorDraw & vectorDraw();
    MxTextDraw & textDraw();




private:
    MxVectorDraw pVectorDraw;
    MxIconDraw pIconDraw[ColorCount];


    MxTextDraw pTextDraw; // colored text
    MxVector2F pTranslation;

};

#endif // QXPAINTERBUFFER_H
