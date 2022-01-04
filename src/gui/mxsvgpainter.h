/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXSVGPAINTER_H
#define MXSVGPAINTER_H

#include "mxvector.h"
#include "mxvectordraw.h"


class MxAbstractSvg;


class MxSvgPainter
{
public:
    MxSvgPainter( MxVectorDraw &stream, const MxVector2F &scale );


    void drawFilledRectangle( const MxRectF &rect, const MxVector4UC &color );
    void drawRoundedRect(const MxRectF &rect, float radius, const MxVector4UC &color );
    void drawLine( const MxVector2F &posA, const MxVector2F &posB, float thickness, const MxVector4UC &color);
    void drawCustomTriangle( const MxVector2F &posA, const MxVector4UC &colorA,
                             const MxVector2F &posB, const MxVector4UC &colorB,
                             const MxVector2F &posC, const MxVector4UC &colorC, MxVectorDraw::TriangleFill fill );



private:
    MxVector2F pScale;
    MxVectorDraw &pVectorDraw;
};

#endif // MXSVGPAINTER_H
