/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxtextbutton.h"
#include "mxpainter.h"
#include "mxfont.h"
#include "mxmouseevent.h"
#include "mxeventhandler.h"
#include "mxguiaggregation.h"
#include "mxthemecolors.h"



MxTextButton::MxTextButton()
{
}

MxTextButton::~MxTextButton()
{
}

void MxTextButton::setText( const MxString &text )
{
    pText = text;
}

MxVector2F MxTextButton::sizePolicy()
{
    const MxFont *f = MxGuiAggregation::instance()->font();

    float w = f->width( pText );
    // height of the buttons depend on the size of the text
    float h = f->metric(MxFont::Height) + 6.0f;
    const float margins = 16.0f;
    MxVector2F policy( -(w+margins), -h);
    return policy;
}

void MxTextButton::paint( MxPainter &painter )
{
    MxRectF drawArea(size());

    MxRectF countourRect = drawArea;
    const float border = 1.0f;
    const float outerRadius = 6.0f;
    const float innerRadius = 4.0f;
    MxVectorDraw &lbTriangles = painter.vectorDraw();

    if( isEnabled() )
    {
        if( state() == MxButton::UnderMouseState )
        {
            MxVector4UC contourColor(127, 130, 137, 255);
            MxVector4UC fillColor(107, 111, 116, 255);
            lbTriangles.roundedRect( countourRect, outerRadius, contourColor);
            countourRect.adjust(border, -border, border, -border);
            lbTriangles.roundedRect( countourRect, innerRadius, fillColor);

        }
        else if( state() == MxButton::PressedState )
        {
            MxVector4UC contourColor(91, 94, 99, 255);
            MxVector4UC fillColor(69, 70, 78, 255);
            lbTriangles.roundedRect( countourRect, outerRadius, contourColor);
            countourRect.adjust(border, -border, border, -border);
            lbTriangles.roundedRect( countourRect, innerRadius, fillColor);

        }
        else
        {
            MxVector4UC contourColor(109, 112, 118, 255);
            MxVector4UC fillColor(91, 94, 99, 255);
            lbTriangles.roundedRect( countourRect, outerRadius, contourColor);
            countourRect.adjust(border, -border, border, -border);
            lbTriangles.roundedRect( countourRect, innerRadius, fillColor);
        }
        MxTextDraw &whiteTextArray = painter.textDraw();
        whiteTextArray.drawTextCentered( pText, drawArea, MxThemeColors::whiteText );
    }
    else
    {
        MxVector4UC contourColor(91, 94, 99, 255);
        MxVector4UC fillColor(52, 53, 58, 255);
        lbTriangles.roundedRect( countourRect, outerRadius, contourColor);
        countourRect.adjust(border, -border, border, -border);
        lbTriangles.roundedRect( countourRect, innerRadius, fillColor);
        MxTextDraw &whiteTextArray = painter.textDraw();
        whiteTextArray.drawTextCentered( pText, drawArea, MxThemeColors::disabledText );
    }
}
