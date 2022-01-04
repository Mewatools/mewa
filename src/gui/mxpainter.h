/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXPAINTER_H
#define MXPAINTER_H

#include "mxdebug.h"
#include "mxvectordraw.h"
#include "mxicondraw.h"
//#include "mxtextdraw.h"
#include "mxlist.h"


class MxWidget;
class MxAbstractSvg;


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

    void prepareRender( MxRenderer &renderer );
    void render(MxRenderer &renderer );

    void setTranslation( const MxVector2F &translation );
    void drawSvg(MxAbstractSvg *svg , const MxRectF &targetRect);

    MxVectorDraw & vectorDraw();
    MxIconDraw & iconDraw( IconColor color );
   // MxTextDraw & textDraw();

private:
    MxVectorDraw pVectorDraw;
    MxIconDraw pIconDraw[ColorCount];
   // MxTextDraw pTextDraw;

    MxVector2F pTranslation;

};

#endif // QXPAINTERBUFFER_H
