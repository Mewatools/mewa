/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxspacer.h"
#include "mxfillbackgroundinterface.h"
#include "mxpainter.h"


MxSpacer::MxSpacer()
{
    pOrientation = -1;
}

MxSpacer::~MxSpacer()
{

}

void MxSpacer::setOrientation( int orientation )
{
    pOrientation = orientation;
}

MxVector2F MxSpacer::sizePolicy()
{
    Q_ASSERT( pOrientation == 0 || pOrientation == 1 );

    MxVector2F policy;
    if(pOrientation == 0) { // horizontal
        policy = MxVector2F(0.0f, -1.0f);
    } else { // vertical
        policy = MxVector2F(-1.0f, 0.0f);
    }

    return policy;
}




void MxSpacer::paint( MxPainter &painter )
{
    // fill background color
    MxRectF windowArea(size());
    MxWidget *parentWidget = this;
    if( ! (parentWidget->pAttributes & MxWidget::HasBackground) )
    {
        parentWidget = firstParentWithAttribute( MxWidget::HasBackground );
        Q_ASSERT( NULL != parentWidget );
        Q_ASSERT( parentWidget->pAttributes & MxWidget::HasBackground );
    }
    MxFillBackgroundInterface *backFiller = dynamic_cast<MxFillBackgroundInterface*>(parentWidget);
    const MxVector4UC& backColor = backFiller->backgroundColor();
    painter.vectorDraw().fillRect(windowArea, backColor);
}
