/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXGEOMETRYUTILS_H
#define MXGEOMETRYUTILS_H


#include "mxvector.h"
#include "mxrect.h"

#include <math.h> // for cosf, sinf, sqrt


/*!
  Contains functions that only depend on MxVector and MxRect
  */
namespace MxGeometryUtils
{



inline float dot( const MxVector3F & a, const MxVector3F & b )
{
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}


inline void cross( MxVector3F &result, const MxVector3F & a, const MxVector3F & b )
{
        float x = a.y() * b.z() - a.z() * b.y();
        float y = a.z() * b.x() - a.x() * b.z();
        float z = a.x() * b.y() - a.y() * b.x();
        result[0] = x;
        result[1] = y;
        result[2] = z;
}

inline float lengthSquared( const MxVector3F & a )
{
    return dot(a,a);
}

inline float length( const MxVector3F & v )
{
    return sqrtf(lengthSquared(v));
}


inline double distance( const MxVector2F &a, const MxVector2F &b )
{
    double dx = (b.x()-a.x());
    double dy = (b.y()-a.y());
    double dist = sqrt(dx*dx + dy*dy);
    return dist;
}


inline void lineIntersection( const MxVector2F &ps1, const MxVector2F &pe1, const MxVector2F &ps2, const MxVector2F &pe2, MxVector2F &result )
{
    // Get A,B,C of first line - points : ps1 to pe1
    float A1 = pe1.y() -ps1.y();
    float B1 = ps1.x() - pe1.x();
    float C1 = A1*ps1.x() + B1*ps1.y();

    // Get A,B,C of second line - points : ps2 to pe2
    float A2 = pe2.y() - ps2.y();
    float B2 = ps2.x() - pe2.x();
    float C2 = A2*ps2.x() + B2*ps2.y();

    // Get delta and check if the lines are parallel
    float delta = A1*B2 - A2*B1;
    Q_ASSERT(delta != 0.0f); // Lines are parallel

    // now return the Vector2 intersection point
    result = MxVector2F( (B2*C1 - B1*C2)/delta, (A1*C2 - A2*C1)/delta );
}


// used to determine which side to fill
inline float signedArea( const MxVector2F &a , const MxVector2F &b )
{
    return (( a.x() * b.y() ) - (b.x() * a.y() )) * 0.5f;
}

inline float crossProduct( const MxVector2F &a, const MxVector2F &b, const MxVector2F &c )
{
    // u = b - a
    MxVector2F u = b-a;
    MxVector2F v = c-b;
    float r = (u.x() * v.y()) - (u.y() * v.x());
    return r;
}

inline MxVector2F getNormal( const MxVector2F &a, const MxVector2F &b )
{
    MxVector2F tmp = b - a;

    // normalize normal
    float length = sqrt( tmp.x() * tmp.x() + tmp.y() * tmp.y() );
    tmp /= length;

    return MxVector2F(-tmp.y(),tmp.x());
}

inline void rotatePoint( const MxVector2F &center, MxVector2F &p, float angle )
{
    float s = sin(angle);
    float c = cos(angle);


    // translate point back to origin:
    p[0] -= center.x();
    p[1] -= center.y();

    // rotate point
    float xnew = p.x() * c - p.y() * s;
    float ynew = p.x() * s + p.y() * c;

    // translate point back:
    p[0] = xnew + center.x();
    p[1] = ynew + center.y();
}

/*! Given the line p1 to p2, returns the y value at x. */
inline float solveLineY(const MxVector2F &p1, const MxVector2F &p2, float x )
{
    Q_ASSERT( (p1.x() <= x && x <= p2.x()) || (p2.x() <= x && x <= p1.x()) );
    /* This is an optimization to calculate the line equation.
     It translates one point of the line to the origin
     so it avoids calculating b. */
    const MxVector2F delta = p2 - p1;
    float m = delta.y() / delta.x();
    float yValue = m*(x - p1.x());
    yValue += p1.y();
    return yValue;
}


template<typename T>
MxRect<T> scaleSizeKeepingAspectRatio( const MxVector<2,T> &from, const MxVector<2,T> &to )
{
    float imageWidth = from.width();
    float imageHeight = from.height();
    float viewWidth = to.width();
    float viewHeight = to.height();
    float x, y, w, h;

    float scaledWidth = (imageWidth / imageHeight) * viewHeight;
    bool useHeight = (scaledWidth <= viewWidth);

    if (useHeight)
    {
        w = scaledWidth;
        h = viewHeight;
        x = (viewWidth - w) / 2.0 ;
        y = 0.0f;
    }
    else
    {
        h = imageHeight / imageWidth * viewWidth;
        w = viewWidth;
        x = 0.0f;
        y = (viewHeight - h) / 2.0f;
    }

    return MxRect<T>(x, x+w, y, y+h);
}


inline MxRectF centeredRect( const MxRectF &area, const MxVector2F &rectSize )
{
    //qDebug("area.height(): %f, rectSize.height(): %f, area.width(): %f, rectSize.width(): %f", area.height(), rectSize.height(), area.width(), rectSize.width());
    Q_ASSERT( area.height() >= rectSize.height() && area.width() >= rectSize.width() );
    MxVector2F thumbPos;
    thumbPos[0] = area.left() + (area.width() - rectSize.width())/2.0f;
    thumbPos[1] = area.bottom() + (area.height() - rectSize.height())/2.0f;
    MxRectF drawRect;
    drawRect.setRectFromPointAndSize(thumbPos, rectSize);
    return drawRect;
}

inline MxRectF centeredRect( const MxRectF &area, const MxVector2I &rectSize )
{
    float rectW = (float)rectSize.width();
    float rectH = (float)rectSize.height();
    Q_ASSERT( area.height() >= rectH && area.width() >= rectW );
    MxVector2F thumbPos;
    thumbPos[0] = area.left() + (area.width() - rectW)/2.0f;
    thumbPos[1] = area.bottom() + (area.height() - rectH)/2.0f;
    MxRectF drawRect;
    drawRect.setRectFromPointAndSize(thumbPos, MxVector2F(rectW, rectH) );
    return drawRect;
}


inline MxRectI centeredRectAtPos( const MxVector2I &pos, const MxVector2I &size )
{
    int hw = size.width() / 2;
    int hh = size.height() / 2;
    int x = pos.x();
    int y = pos.y();
    MxRectI rect( x-hw, x+hw, y-hh, y+hh);
    return rect;
}



}

#endif

