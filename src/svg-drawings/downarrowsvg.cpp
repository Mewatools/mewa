/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "downarrowsvg.h"
#include "mxsvgpainter.h"
#include "mxthemecolors.h"

DownArrowSvg::DownArrowSvg()
{
}



MxVector2F DownArrowSvg::canvasSize() const
{
    return MxVector2F(64.0f, 64.0f);
}


void DownArrowSvg::draw( MxSvgPainter &painter )
{

    MxVector2F size = canvasSize();
    const float halfH = size.height() / 2.0f;
    const float halfW = size.width() / 2.0f;

    const float rectMargin = 0.20 * size.width();
    MxRectF rect(rectMargin, size.width()-rectMargin, halfH, size.height());
    painter.drawFilledRectangle( rect, MxThemeColors::whiteText );


    MxVector2F posA( 0.0f, halfH );
    MxVector2F posB( halfW, 0.0f);
    MxVector2F posC( size.width(), halfH );

    painter.drawCustomTriangle( posA, MxThemeColors::whiteText,
                                posB, MxThemeColors::whiteText,
                                posC, MxThemeColors::whiteText, MxVectorDraw::FullFill );


}
