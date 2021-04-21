/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXABSTRACTSVG_H
#define MXABSTRACTSVG_H

#include "mxvector.h"


class MxSvgPainter;



/*! 
 Holds an SVG draw
*/
class MxAbstractSvg
{
public:
    MxAbstractSvg(){}

    virtual MxVector2F canvasSize() const = 0;

    virtual void draw( MxSvgPainter &painter ) = 0;

};

#endif

