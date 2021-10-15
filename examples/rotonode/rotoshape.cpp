/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "rotoshape.h"
#include "mxorthotransform.h"
#include "rototransform.h"
#include "mxpainter.h"
#include "shapelockedsvg.h"
#include "mxaggregation.h"
#include "mxabstractatlas.h"
#include "mxgeometryutils.h"


#define ARROW_COLUMN_WIDTH 12.0f

RotoShape::RotoShape( RotoTransform *parent )
    : RotoTreeItem(parent)
{
    pOperation = OR;

    pProperties = RotoShape::OpenStroke;
    pCurveInterface = RotoShape::BSplineInterface;

    pBlendMode = RotoShape::Add;

    pSelectionCount = 0;

    Q_ASSERT( NULL != parent );
    parent->pChildren.append(this);
}


RotoShape::~RotoShape()
{}


void RotoShape::toggleInvert()
{
    pProperties ^= RotoShape::Invert;
}

bool RotoShape::isInverted() const
{
    return pProperties & RotoShape::Invert;
}

void RotoShape::toggleLock()
{
    pProperties ^= RotoShape::Locked;
}

bool RotoShape::isLocked() const
{
    return pProperties & RotoShape::Locked;
}

void RotoShape::toggleVisibility()
{
    pProperties ^= RotoShape::Visible;
}

bool RotoShape::isVisible() const
{
    return pProperties & RotoShape::Visible;
}


RotoTreeItem::ItemType RotoShape::itemType() const
{
    return RotoTreeItem::ShapeType;
}

RotoTreeItem* RotoShape::siblingAtRow(int row) const
{
    return NULL;
}

void RotoShape::paint(MxPainter &painter, DrawData &drawData ) const
{
    MxVectorDraw &lbTriangles = painter.vectorDraw();
    const float buttonW = drawData.rowH;
    fillRow( lbTriangles, drawData );

    // draw  text


    // draw buttons
    const float buttonMargin = 2.0f;
    const MxVector4UC highlightColor(120, 120, 120, 255);
    MxVector4UC restColor(60, 60, 160, 255);
    // lock button
    ShapeLockedSvg lockSvg;
    MxRectF buttonRect = drawData.rowRect;
    float buttonLeft = buttonRect.right() - buttonW;
    buttonRect.setLeft( buttonLeft );
    // qDebug("isLocked(): %d", (int)isLocked());
    if( isLocked() ) {
        restColor = MxVector4UC(60, 60, 160, 255);
    } else {
        restColor = MxVector4UC(80, 80, 80, 255);
    }

    if( drawData.isUnderMouse() && drawData.pButton == 2 ) { // hovering button

        lockSvg.pColor = highlightColor;
    } else {
        lockSvg.pColor = restColor;
    }
    painter.drawSvg( &lockSvg , buttonRect);

    // show/hide button
    buttonRect.setRight(buttonLeft);
    buttonLeft -= buttonW;
    buttonRect.setLeft( buttonLeft );
    if( drawData.isUnderMouse() && drawData.pButton == 1 ) {
        lbTriangles.fillRect( buttonRect, MxVector4UC(100, 220, 30, 255) );
    } else {
        lbTriangles.fillRect( buttonRect, MxVector4UC(111, 20, 30, 255) );
    }
    MxIconDraw &iconPainter = painter.iconDraw( MxPainter::OriginalColor );
    const MxAbstractAtlas *atlas = MxAggregation::instance()->iconAtlas();
    const MxRectF &visIconRect = atlas->iconRect( MxThemeIcons::NotVisible );
    iconPainter.drawImageRect( visIconRect, buttonRect );

    // invert button
    buttonRect.setRight(buttonLeft);
    buttonLeft -= buttonW;
    buttonRect.setLeft( buttonLeft );
    /*if( drawData.isUnderMouse() && drawData.pButton == 0 ) {
        lbTriangles.fillRect( buttonRect, MxVector4UC(10, 220, 230, 255) );
    } else{
        lbTriangles.fillRect( buttonRect, MxVector4UC(110, 220, 130, 255) );
    }*/
    int invertIconName;
    if( isInverted() ) {
        invertIconName = MxThemeIcons::Inverted;
    } else {
        invertIconName = MxThemeIcons::NotInverted;
    }
    const MxRectF &invTextureRect = atlas->iconRect( invertIconName );
    // because the icon is smaller than the buttonRect, we center the icon inside buttonRect
    MxRectF centeredIconRect = MxGeometryUtils::centeredRect( buttonRect, atlas->iconSize( invertIconName ) );
    iconPainter.drawImageRect( invTextureRect, centeredIconRect );


    drawData.rowIndex++;

}

bool RotoShape::isSelected() const
{
    return pSelected;

}

RotoTreeItem* RotoShape::itemAtRow( int *rowIndex, int *transformCount )
{
    Q_ASSERT( (*rowIndex) >= 0 );

    if( (*rowIndex) == 0 ) {
        return this;
    }
    return NULL;
}

void RotoShape::clearSelection()
{
    pSelected = false;
}

void RotoShape::selectRow()
{
    pSelected = true;
}
