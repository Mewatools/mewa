/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXABSTRACTSVG_H
#define MXABSTRACTSVG_H

#include "mxvector.h"


class MxSvgPainter;



/*! 
 Like an SVG drawing, a Mewa SVG is a collection of Mewa triangles
 that are renderered together as a single draw.

 It makes an efficient use of icon drawing, as it doesn't use any 
 texture memory and can be rendered at any size without any performance
 penalty.
  
 A Mewa SVG is derived from MxAbstractSvg.
  
 \sa DownArrowSvg, HeartSvg and PlusMinusSvg
*/
class MxAbstractSvg
{
public:
    MxAbstractSvg(){}

/*! Returns the size of the area being drawn. It can be any size 
 and is used to select the area of the draw that will be rendered */ 
    virtual MxVector2F canvasSize() const = 0;

/*! This method makes all the draw calls to render the SVG. Note that
 the target area of the draw is set by the canvasSize() method.
 */
    virtual void draw( MxSvgPainter &painter ) = 0;

};

#endif

