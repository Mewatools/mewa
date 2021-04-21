/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef HEARTSVG_H
#define HEARTSVG_H

#include "mxabstractsvg.h"



class HeartSvg : public MxAbstractSvg
{
public:
    HeartSvg( const MxVector4UC &fillColor );

    virtual MxVector2F canvasSize() const;
    virtual void draw( MxSvgPainter &painter );


   const MxVector4UC &pFillColor;
};

#endif // HEARTSVG_H
