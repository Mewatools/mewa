/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXRECT_H
#define MXRECT_H

#include "mxdebug.h"
#include "mxvector.h"


template<typename T>
class MxRect
{
public:
    /*! Creates a empty QxRect.
        For performance reasons, no initialization is performed. */
  MxRect();
  MxRect(T left, T right, T bottom, T top);

  inline T left() const { return x1; }
  inline T top() const { return y2; }
  inline T right() const { return x2; }
  inline T bottom() const { return y1; }


  inline MxVector<2,T> bottomLeft() const { return MxVector<2,T>(x1,y1); }
  inline MxVector<2,T> topRight() const { return MxVector<2,T>(x2,y2); }
  inline MxVector<2,T> topLeft() const { return MxVector<2,T>(x1,y2); }
  inline MxVector<2,T> bottomRight() const { return MxVector<2,T>(x2,y1); }

  inline T width() const;
  inline T height() const;

  inline MxVector<2,T> size() const { return MxVector<2,T>(width(), height()); }

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
inline T MxRect<T>::width() const
{
    return (right() - left());
}

template<typename T>
inline T MxRect<T>::height() const
{
    return (top() - bottom());
}

#endif
