/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "rototransform.h"
#include "mxpainter.h"


RotoTransform::RotoTransform(RotoTransform *parent)
    : RotoTreeItem(parent)
{
    pExpanded = true;
    if(parent) {
        parent->pChildren.append( this );
    }
}

RotoTreeItem::ItemType RotoTransform::itemType() const
{
    return RotoTreeItem::TransformType;
}

RotoTreeItem* RotoTransform::siblingAtRow(int row) const
{
    RotoTreeItem* item = NULL;
    int childCount = pChildren.size();
    if( row > childCount ) {
        item = pChildren[childCount-1]->siblingAtRow( row - childCount-1 );
    }
    return item;
}

void RotoTransform::paint(MxPainter &painter, DrawData &drawData ) const
{
    MxVectorDraw &lbTriangles = painter.vectorDraw();
    float yy = drawData.rowRect.bottom();
    fillRow( lbTriangles, drawData );

    // draw expand button
    float expandW = expandButtonWidth();
    MxRectF expandButtonRect = drawData.rowRect;
    expandButtonRect.setLeft( drawData.textX );
    expandButtonRect.setRight( drawData.textX + expandW );
    const float buttonMargin = 5.0f;
    expandButtonRect.adjust(buttonMargin, -buttonMargin, buttonMargin, -buttonMargin);
    const MxVector4UC expandColor(180, 180, 180, 255);
   // lbTriangles.fillRect( expandButtonRect, expandColor );
    const float halfTriangle = 5.0f;
    if( pExpanded ) {
        float middleX = expandButtonRect.left() + ((expandButtonRect.right() - expandButtonRect.left()) / 2.0f);
        MxVector2F midCenter( middleX, expandButtonRect.bottom() - halfTriangle );
        lbTriangles.triangle(expandButtonRect.topLeft(), expandColor,
                            midCenter , expandColor,
                            expandButtonRect.topRight() , expandColor, MxVectorDraw::InnerCurveFill );
    } else {
        float middleY = expandButtonRect.bottom() + ((expandButtonRect.top() - expandButtonRect.bottom()) / 2.0f);
        MxVector2F midCenter( expandButtonRect.right() + halfTriangle, middleY );
        lbTriangles.triangle(expandButtonRect.bottomLeft(), expandColor,
                           midCenter , expandColor,
                             expandButtonRect.topLeft() , expandColor, MxVectorDraw::InnerCurveFill );
    }


    drawBottomSeparator( lbTriangles, drawData.rowRect );

    // draw name

    drawData.rowIndex++;

    if( pExpanded )
    {
        // daw children

        drawData.textX += expandW;
        int count = pChildren.size();
        for(int i=0; i<count; ++i) {


            yy -= drawData.rowH;
            drawData.rowRect.setBottom(yy);
            drawData.rowRect.setTop(yy +  drawData.rowH);

            pChildren[i]->paint( painter, drawData );
        }
        drawData.textX -= expandW;
    }
}

bool RotoTransform::isSelected() const
{
   return pSelected;

}

RotoTreeItem* RotoTransform::itemAtRow( int *rowIndex, int *transformCount )
{
    (*transformCount)++;

    Q_ASSERT( (*rowIndex) >= 0 );
    if( (*rowIndex) == 0 ) {
        return this;
    }

    int count = pChildren.size();
    for(int i=0; i<count; ++i) {

        RotoTreeItem *item = pChildren[i];
        (*rowIndex)--;
        RotoTreeItem *foundItem = item->itemAtRow( rowIndex, transformCount );
        if(foundItem) {
            return foundItem;
        }

    }
    return NULL;
}

void RotoTransform::clearSelection()
{
    pSelected = false;
    int count = pChildren.size();
    for(int i=0; i<count; ++i) {

        RotoTreeItem *item = pChildren[i];
        item->clearSelection();

    }


}

void RotoTransform::selectRow()
{
    pSelected = true;
}

void RotoTransform::toggleExpand()
{
    pExpanded = !pExpanded;
}

float RotoTransform::expandButtonWidth()
{
    return 20.0f;
}
