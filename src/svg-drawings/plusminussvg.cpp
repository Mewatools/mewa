/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "plusminussvg.h"
#include "mxsvgpainter.h"
#include "mxthemecolors.h"


PlusMinusSvg::PlusMinusSvg( Symbol sym )
{
    pSymbol = sym;
}



MxVector2F PlusMinusSvg::canvasSize() const
{
    return MxVector2F(16.0f, 16.0f);
}


void PlusMinusSvg::draw( MxSvgPainter &painter )
{

    MxVector2F size = canvasSize();
    MxRectF area( 0.0f, size.width(), 0.0f, size.height());

    const float halfThickness = 2.0f;
    float middleX = size.width() / 2.0f;
    float middleY = size.height() / 2.0f;
    MxRectF horzLine(0.0f, size.width(), middleY - halfThickness, middleY + halfThickness );

    painter.drawFilledRectangle( horzLine, MxThemeColors::whiteText );
    if( PlusSymbol == pSymbol ) {
        MxRectF vertLine(middleX - halfThickness, middleX + halfThickness, 0.0f, size.height());
        painter.drawFilledRectangle( vertLine, MxThemeColors::whiteText );
    }

}

