/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "shapelockedsvg.h"
#include "mxsvgpainter.h"



ShapeLockedSvg::ShapeLockedSvg()
{

}

MxVector2F ShapeLockedSvg::canvasSize() const
{
    return MxVector2F(16.0f, 16.0f);
}


void ShapeLockedSvg::draw( MxSvgPainter &painter )
{

    MxVector2F size = canvasSize();
    MxRectF area( 0.0f, size.width(), 0.0f, size.height());
   // MxVector4UC lockColor(91, 91, 112, 255);
    float right = size.width();
    float top = size.height();
    float middleY = top / 2.0f;
    float middleX = right / 2.0f;

    const float ringMargin = 2.0f;
    const float halfRingMargin = ringMargin / 2.0f;

    MxRectF lockerBody( halfRingMargin,
                        right - halfRingMargin,
                        halfRingMargin,
                        middleY
                        );
    painter.drawFilledRectangle( lockerBody, pColor );


    MxVector2F posA( ringMargin, middleY); // left
    MxVector2F posB( middleX, top ); // top center
    MxVector2F posC( right - ringMargin, middleY );
    painter.drawCustomTriangle( posA, pColor,
                                posB, pColor,
                                posC, pColor, MxVectorDraw::InnerCurveFill );

    // draw background on top to create the ring stroke
   /* const float ringThickness = 3.0f;
    posA[0] += ringThickness;
    posB[1] -= ringThickness;
    posC[0] -= ringThickness;
    painter.drawCustomTriangle( posA, pColor,
                                posB, pColor,
                                posC, pColor, MxVectorDraw::InnerCurveFill );*/



    /*
    MxVector2F size = canvasSize();
    MxRectF area( 0.0f, size.width(), 0.0f, size.height());
    MxVector4UC borderColor(220, 220, 220, 255);
    float radius = 4.0f;
    painter.drawRoundedRect( area, radius, pBackgroundColor );

    float right = size.width();
    float top = size.height();
    float thickness = 2.0f;
    float halfThickness = thickness/2.0f;
    float middleY = size.height() / 2.0f;



    MxVector4UC crossColor(41, 171, 226, 255);
    MxRectF horLine( 0.0f,
                     right,
                     middleY - halfThickness,
                     middleY + halfThickness
                     );
    painter.drawFilledRectangle( horLine, crossColor );





    MxVector4UC curveColor(140, 198, 63, 255);
    MxVector2F posA( right, top - thickness );
    MxVector2F posB( thickness, middleY + halfThickness);
    MxVector2F posC( right, middleY + halfThickness );
    painter.drawCustomTriangle( posA, curveColor,
                                posB, curveColor,
                                posC, curveColor, MxVectorDraw::InnerCurveFill );

    // draw background on top to create stroke

    posA = MxVector2F( right, middleY - halfThickness );
    posB = MxVector2F( thickness, middleY - halfThickness );
    posC = MxVector2F( right, thickness );
    painter.drawCustomTriangle( posA, pBackgroundColor,
                                posB, pBackgroundColor,
                                posC, pBackgroundColor, MxVectorDraw::InnerCurveFill );
*/

}
