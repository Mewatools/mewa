/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXRECT_H
#define MXRECT_H

#include "mxdebug.h"
#include "mxvector.h"

#include <algorithm> // min, max

template<typename T>
class MxRect
{
public:
    /*! Creates a empty MxRect.
        For performance reasons, no initialization is performed. */
    MxRect();
    MxRect(T left, T right, T bottom, T top);
    MxRect( const MxVector<2,T> &size );
    MxRect( const MxVector<2,T> &p1, const MxVector<2,T> &p2 );

    /*! Returns the left x position. */
    inline T left() const { return x1; }
    inline T top() const { return y2; }
    inline T right() const { return x2; }
    inline T bottom() const { return y1; }

    inline void setLeft(T pos) { x1 = pos; }
    inline void setTop(T pos) { y2 = pos; }
    inline void setRight(T pos) { x2 = pos; }
    inline void setBottom(T pos) { y1 = pos; }

    inline MxVector<2,T> bottomLeft() const { return MxVector<2,T>(x1,y1); }
    inline MxVector<2,T> topRight() const { return MxVector<2,T>(x2,y2); }
    inline MxVector<2,T> topLeft() const { return MxVector<2,T>(x1,y2); }
    inline MxVector<2,T> bottomRight() const { return MxVector<2,T>(x2,y1); }

    inline T width() const;
    inline T height() const;

    inline MxVector<2,T> size() const { return MxVector<2,T>(width(), height()); }

    bool isNull() const
    {
        return ( y1 == 0 && y2 == 0 && x1 == 0 && x2 == 0 );
    }

    void setToNull()
    {
        y1 = 0;
        y2 = 0;
        x1 = 0;
        x2 = 0;
    }

    bool isNormalized() const
    {
        if( y1 <= y2 && x1 <= x2 )
            return true;

        return false;
    }

    void flipVertically()
    {
        T tmp = y2;
        y2 = y1;
        y1 = tmp;
    }

    void flipHorizontally()
    {
        T tmp = x2;
        x2 = x1;
        x1 = tmp;
    }

    void setRectFromPointAndSize( const MxVector<2,T> &position, const MxVector<2,T> &size )
    {
        setLeft(position.x());
        setRight(position.x() + size.width());
        setBottom(position.y());
        setTop(position.y() + size.height());
    }

    // returns the center not centerOfMass
    MxVector2F center() const
    {
        return MxVector2F( x1 + ((x2-x1)/2.0f), y1 + ((y2-y1)/2.0f));
    }

  /*! Returns true if the given \a point is inside or on the edge of the QxRect; otherwise returns false.
              \sa intersects(). */
  bool contains( const MxVector<2,T> &point ) const;

    /*! Returns true if this MxRect intersects with the given \a MxRect (i.e. there
              is a non-empty area of overlap between them), otherwise returns false.
              The intersection MxRect can be retrieved using the intersected() function.
              \sa contains().*/
    bool intersects( const MxRect<T> &rect ) const
    {
        // check if is normalized
        Q_ASSERT( isNormalized() );
        Q_ASSERT( rect.isNormalized() );

        return (std::max((T)x1, rect.left()) <= std::min((T)x2, rect.right()) &&
                std::max((T)y1, rect.bottom()) <= std::min((T)y2, rect.top()));
    }




    MxRect<T> intersected ( const MxRect<T> & other ) const
    {
        return *this & other;
    }


    void normalize()
    {
        T tmp;
        if( y1 > y2 )
        {
            tmp = y1;
            y1 = y2;
            y2 = tmp;
        }

        if( x1 > x2 )
        {
            tmp = x1;
            x1 = x2;
            x2 = tmp;
        }
    }

    void roundToInt()
    {
        x1 = (int)x1;
        y1 = (int)y1;
        x2 = (int)x2;
        y2 = (int)y2;
    }

    /*! Values are added to the reespective sides. */
    void adjust( T left, T right, T bottom, T top )
    {
        x1 += left; // left
        y1 += bottom; // bottom
        x2 += right; // right
        y2 += top; // top
    }

    /*! Translates this rect by dx amount. */
    void translateX( const T dx );
    void translateY( const T dy );
    void translate( const T dx, const T dy );
    void translate( const MxVector<2,T> &d  );


private:
    T x1; // left
    T x2; // right
    T y1; // bottom
    T y2; // top
};


//////////// Convenience type definitions //////////
typedef MxRect<float>  MxRectF;
typedef MxRect<int>  MxRectI;



////// inline functions //////////
template<typename T>
inline MxRect<T>::MxRect()
{}

template<typename T>
inline MxRect<T>::MxRect(T left, T right, T bottom, T top)
{
    x1 = left;
    y1 = bottom;
    x2 = right;
    y2 = top;
}

template<typename T>
inline MxRect<T>::MxRect( const MxVector<2,T> &size )
{
    x1 = (T)0.0;
    y1 = (T)0.0;
    x2 = size.x();
    y2 = size.y();
}

template<typename T>
inline MxRect<T>::MxRect( const MxVector<2,T> &p1, const MxVector<2,T> &p2 )
{
    x1 = p1.x();
    y1 = p1.y();
    x2 = p2.x();
    y2 = p2.y();
}

template<typename T>
inline T MxRect<T>::width() const
{
    return (right() - left());
}

template<typename T>
inline T MxRect<T>::height() const
{
    return (top() - bottom());
}

template<typename T>
bool MxRect<T>::contains( const MxVector<2,T> &point ) const
{
    if( point.x() >= x1 && point.x() <= x2 && point.y() >= y1 && point.y() <= y2 )
        return true;

    return false;
}

template<typename T>
void MxRect<T>::translateX( const T dx )
{
    x1 += dx;
    x2 += dx;
}

template<typename T>
void MxRect<T>::translateY( const T dy )
{
    y1 += dy;
    y2 += dy;
}

template<typename T>
void MxRect<T>::translate( const T dx, const T dy )
{
    x1 += dx;
    x2 += dx;
    y1 += dy;
    y2 += dy;
}

template<typename T>
void MxRect<T>::translate( const MxVector<2,T> &d  )
{
    x1 += d[0];
    x2 += d[0];
    y1 += d[1];
    y2 += d[1];
}

#endif
