/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxsvgpainter.h"
#include "mxvectordraw.h"

MxSvgPainter::MxSvgPainter( MxVectorDraw &stream, const MxVector2F &scale )
    :pVectorDraw(stream)
{
    pScale = scale;
}


void MxSvgPainter::drawFilledRectangle( const MxRectF &rect, const MxVector4UC &color )
{
    MxRectF scaledRect( rect.left() * pScale.x(),
                        rect.right() * pScale.x(),
                        rect.bottom() * pScale.y(),
                        rect.top() * pScale.y()
                        );
    pVectorDraw.fillRect( scaledRect, color );
}

void MxSvgPainter::drawRoundedRect( const MxRectF &rect, float radius, const MxVector4UC &color )
{
    MxRectF scaledRect( rect.left() * pScale.x(),
                        rect.right() * pScale.x(),
                        rect.bottom() * pScale.y(),
                        rect.top() * pScale.y()
                        );

    // \TODO scale the radius as well

    pVectorDraw.roundedRect( scaledRect, radius, color );
}

void MxSvgPainter::drawLine( const MxVector2F &posA, const MxVector2F &posB, float thickness, const MxVector4UC &color)
{

    // \TODO scale the thickness as well

    pVectorDraw.stroke( posA * pScale, posB * pScale, thickness, color );
}

void MxSvgPainter::drawCustomTriangle( const MxVector2F &posA, const MxVector4UC &colorA,
                                       const MxVector2F &posB, const MxVector4UC &colorB,
                                       const MxVector2F &posC, const MxVector4UC &colorC, MxVectorDraw::TriangleFill fill )
{

    pVectorDraw.triangle(
                posA * pScale, colorA,
                posB * pScale, colorB,
                posC * pScale, colorC,  fill
                );
}

