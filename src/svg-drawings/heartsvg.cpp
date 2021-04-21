/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "heartsvg.h"
#include "mxsvgpainter.h"


HeartSvg::HeartSvg(const MxVector4UC &fillColor)
    :pFillColor(fillColor)
{
}


MxVector2F HeartSvg::canvasSize() const
{
    return MxVector2F(64.0f, 64.0f);
}


void HeartSvg::draw( MxSvgPainter &painter )
{
    MxVector2F size = canvasSize();
    const float halfW = size.width() / 2.0f;
    MxVector2F middleUp( halfW, 0.78 * size.height() );
    MxVector2F middleDown( halfW, 0.0f);

    const float adjustX = 20.0f;
    const float adjustY = 10.0f;
    MxVector2F tl( 0.0f - adjustX, 64.0f + adjustY);
    MxVector2F tr( 64.0f + adjustX, 64.0f + adjustY );

    painter.drawCustomTriangle( middleDown, pFillColor,
                                tl, pFillColor,
                                middleUp, pFillColor, MxVectorDraw::InnerCurveFill );

    painter.drawCustomTriangle( middleDown, pFillColor,
                                tr, pFillColor,
                                middleUp, pFillColor, MxVectorDraw::InnerCurveFill );

}
