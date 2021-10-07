/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef SHAPELOCKEDSVG_H
#define SHAPELOCKEDSVG_H

#include "mxabstractsvg.h"


class ShapeLockedSvg : public MxAbstractSvg
{
public:
    ShapeLockedSvg();

    virtual MxVector2F canvasSize() const;
    virtual void draw( MxSvgPainter &painter );

    MxVector4UC pColor;


};

#endif // SHAPELOCKEDSVG_H
