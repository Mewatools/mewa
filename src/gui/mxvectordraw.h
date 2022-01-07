/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXVECTORDRAW_H
#define MXVECTORDRAW_H

#include "mxrect.h"



class MxRenderer;
class MxBuffer;


class MxVectorDraw
{
public:


    struct Vertex
    {
        float x;
        float y;

        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;

        float u;
        float v;
    };


    MxVectorDraw();
    MxVectorDraw( MxBuffer *buffer, MxVector2F *translation );



    void clear();
    int pointCount() const;

    //! Draws rotating triangle. Used to show when something is loading
    void loadingSymbol(const MxVector2F &center, float radius );

    //! Draw a filled rectangle with rounded corners
    void roundedRect( const MxRectF &area, float radius, const MxVector4UC &color );

    void addVGradientLeftTopRounded(const MxRectF &area, const MxVector4UC &color );
    void addVGradientLeftBottomRounded(const MxRectF &area, const MxVector4UC &color );
    void addVGradientRightTopRounded(const MxRectF &area, const MxVector4UC &color );
    void addVGradientRightBottomRounded(const MxRectF &area, const MxVector4UC &color );

    // draws a circle using 4 triangles
    void addSquaredCircle(MxVector2F center, const MxVector2F &radius, const MxVector4UC &color );

    //! Draws a stroked line with given \a thickness
    void stroke(const MxVector2F &p1, const MxVector2F &p2, float thickness, const MxVector4UC &color );
    void strokeRect(MxRectF outerRect, float innerThickness, const MxVector4UC &color );

    // draws a circle using 8 triangles
    void circle(MxVector2F center, float radius, const MxVector4UC &color );

    enum TriangleFill
    {
        InnerCurveFill //! Draws the fill of the inner-side of the curve, anti-alliased
        , OutterCurveFill //! Draws the fill of the outer-side of the curve, anti-alliased
        , FullFill //! Draws an filled triangle, alliased
        , FillCount //! reserved for counting purposes
    };
    //! InnerCurveFill and OutterCurveFill options draw a triangle where 1 side is drawn as a quadratic curve
    void triangle(const MxVector2F &posA, const MxVector4UC &colorA,
                              const MxVector2F &posB, const MxVector4UC &colorB,
                              const MxVector2F &posC, const MxVector4UC &colorC, TriangleFill fill );

    //! Draws a filled \a rect with \a color
    void fillRect(const MxRectF &rect, const MxVector4UC &color );

    void gradientRect(const MxRectF &rect, const MxVector4UC &bottomLeftColor, const MxVector4UC &bottomRightColor , const MxVector4UC &topLeftlColor, const MxVector4UC &topRightColor);
    void verticalGradientRect(const MxRectF &rect, const MxVector4UC &bottomColor, const MxVector4UC &topColor );

    // vertical gradient using 4 colors on rounded corner rect. Takes an array of 4 colors, the 4 gradient colors.
    void roundedRectV4Gradient(const MxRectF &rect, float radius, const MxVector4UC colors[] );

    // looks like half curved rectangle. Curvature reaches full height
    // negative height draws the hat downwards
    void appendChaplinHat(float left, float right,
                          float bottom, float top, float radius, const MxVector4UC &color );

    void appendTopRoundedRect( const MxRectF &area, float radius, const MxVector4UC &color );

private:
    // no translation added, should be only used internally by this class
    void appendFillTriangle_p(const MxVector2F &a, const MxVector2F &b, const MxVector2F &c, const unsigned char *pathColor);
    void appendColoredTriangle_p( const MxVector2F &a, const unsigned char *colorA, const MxVector2F &b, const unsigned char *colorB, const MxVector2F &c, const unsigned char *colorC );
    void appendTriangle_p(const MxVector2F &a, const MxVector2F &b, const MxVector2F &c, unsigned char direction, const unsigned char *pathColor );
    void appendRect_p(const MxVector2F &leftBottom, const MxVector2F &rightBottom, const MxVector2F &topLeft, const MxVector2F &topRight, const MxVector4UC &color );


    // draw a quarter of a circle. Only draws the rounded area
    void addCubicArc_p(MxVector2F center, const MxVector2F inPoints[4], const MxVector4UC &color );
    MxVector2F bezierPoint( const MxVector2F inPoints[4], double inT );


public:
    MxBuffer *pArray;
    MxVector2F *pTranslation;


};

#endif
