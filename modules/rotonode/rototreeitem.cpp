/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "rototreeitem.h"
#include "mxvectordraw.h"


RotoTreeItem::RotoTreeItem(RotoTreeItem *parent)
    :pParent(parent)
{
pSelected = false;
}

RotoTreeItem::~RotoTreeItem()
{
}


bool RotoTreeItem::DrawData::isUnderMouse() const
{
return rowIndex == pHighlightRow;
}


void RotoTreeItem::fillRow( MxVectorDraw &lbTriangles, DrawData &rowData ) const
{
    float pc = isSelected();
    if( pc > 0.0f ) {
        const MxVector4UC selectedColor(90, 90, 160, 255);
        lbTriangles.fillRect( rowData.rowRect, selectedColor);
    } /*else if( rowData.isUnderMouse() ) {
        const MxVector4UC highlightRowColor(120, 120, 190, 255);
        lbTriangles.fillRect( rowData.rowRect, highlightRowColor);
    }*/
}


float RotoTreeItem::rowHeight()
{
     return 22;
}


void RotoTreeItem::drawBottomSeparator( MxVectorDraw &lbTriangles, const MxRectF &rowRect ) const
{
    MxRectF separatorLine = rowRect;
    separatorLine.setTop( rowRect.bottom() + 2.0f );
     const MxVector4UC separatorColor(120, 120, 90, 255);
    lbTriangles.fillRect( separatorLine, separatorColor);
}
