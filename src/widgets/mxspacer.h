/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXSPACER_H
#define MXSPACER_H

#include "mxwidget.h"


/*! The MxSpacer class stretchs out a blank space in a layout. */
class MxSpacer : public MxWidget
{
public:
    MxSpacer();
    virtual ~MxSpacer();

    void setOrientation( int orientation );
    virtual MxVector2F sizePolicy();

    virtual void paint(MxPainter &painter );

protected:
    // 0 = horizontal, 1 = vertical
    int pOrientation;

};

#endif
