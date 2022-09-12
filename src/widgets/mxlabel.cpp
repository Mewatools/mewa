/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxlabel.h"
#include "mxguiaggregation.h"
#include "mxfillbackgroundinterface.h"

MxLabel::MxLabel()
{
    pAlignmentFlags = AlignLeft | AlignVCenter;
}

MxLabel::~MxLabel()
{
}

void MxLabel::setText( const MxString &text )
{
    pText = text;
    update();
}

void MxLabel::setAlignment( int alignmentFlags )
{
    pAlignmentFlags = alignmentFlags;
}



MxVector2F MxLabel::sizePolicy()
{
    const MxFont *f = MxGuiAggregation::instance()->font();
    float h = f->metric(MxFont::Height);
    float w = 1.0f;
    if( ! pText.isNull() ) {
        w = f->width( pText );
    }
    MxVector2F policy(-w, -h);
    return policy;
}



void MxLabel::paint( MxPainter &painter )
{
    // fill background color
    MxWidget *parentBackg = firstParentWithAttribute( MxWidget::HasBackground );
    // if asserts here add if-condition  if( parentBackg != NULL )
    Q_ASSERT( NULL != parentBackg );
    MxFillBackgroundInterface *widgetWithBgd = dynamic_cast<MxFillBackgroundInterface*>(parentBackg);
    const MxVector4UC& backColor = widgetWithBgd->backgroundColor();
    MxRectF windowArea(size());
    painter.vectorDraw().fillRect( windowArea, backColor );



    if( ! pText.isNull() )
    {
        MxVector2F textPos;

        // horizontal flag
        if( pAlignmentFlags & AlignLeft )
        {
            textPos[0] = 0.0f;
        }
        else if( pAlignmentFlags & AlignRight )
        {

        }
        else
        {

        }

        // vertical flag
        if( pAlignmentFlags & AlignBottom )
        {
            textPos[1] = 0.0f;
        }
        else if( pAlignmentFlags & AlignTop )
        {
            float h = painter.font()->metric( MxStaticFont::Height );
            textPos[1] = (int)(size().height() - h);
        }
        else
        {
            Q_ASSERT( pAlignmentFlags & AlignVCenter );
            float fontHeight = painter.font()->metric( MxStaticFont::Ascender ) + painter.font()->metric( MxStaticFont::Descender );
            float widgetHeight = size().height();
            textPos[1] = (int)((widgetHeight - fontHeight) / 2.0f);
        }

    }

}


