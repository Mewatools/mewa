/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef DOWNARROW_H
#define DOWNARROW_H

#include "mxabstractsvg.h"



class DownArrowSvg : public MxAbstractSvg
{
public:
    DownArrowSvg();

    virtual MxVector2F canvasSize() const;
    virtual void draw( MxSvgPainter &painter );

};

#endif // DOWNARROW_H
