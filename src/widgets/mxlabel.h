/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXLABEL_H
#define MXLABEL_H

#include "mxdebug.h"
#include "mxwidget.h"
#include "mxpainter.h"


class MxLabel : public MxWidget
{
public:

    enum Alignment {
        AlignLeft =	0x0001, //	Aligns with the left edge.
        AlignRight = 0x0002, // Aligns with the right edge.
        AlignHCenter = 0x0004, // Centers horizontally in the available space.
        AlignTop = 0x0020, // Aligns with the top.
        AlignBottom = 0x0040, // Aligns with the bottom.
        AlignVCenter = 0x0080 // Centers vertically in the available space.
    };



    MxLabel();
    virtual ~MxLabel();

 void setText( const MxString &text );
    void setAlignment( int alignmentFlags );


    virtual MxVector2F sizePolicy();

    virtual void paint( MxPainter &painter );


protected:
    int pAlignmentFlags;
    MxString pText;

};

#endif
