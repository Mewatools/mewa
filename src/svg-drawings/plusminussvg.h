/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef PLUSMINUSSVG_H
#define PLUSMINUSSVG_H

#include "mxabstractsvg.h"


class PlusMinusSvg : public MxAbstractSvg
{
public:
    enum Symbol
    {
        MinusSymbol,
        PlusSymbol
    };


    PlusMinusSvg( Symbol sym );


    virtual MxVector2F canvasSize() const;
    virtual void draw( MxSvgPainter &painter );

private:
    Symbol pSymbol;
};

#endif // PLUSMINUSSVG_H
