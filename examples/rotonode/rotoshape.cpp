/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "rotoshape.h"
#include "mxorthotransform.h"
#include "rototransform.h"
#include "mxpainter.h"
#include "shapelockedsvg.h"
#include "mxguiaggregation.h"
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



    MxIconDraw &iconPainter = painter.iconDraw( MxPainter::LightColor );
    MxIconDraw &bluePainter = painter.iconDraw( MxPainter::BlueColor );
    const MxAbstractAtlas *atlas = MxGuiAggregation::instance()->iconAtlas();


    // draw buttons
    const float buttonMargin = 2.0f;
    const MxVector4UC highlightColor(120, 120, 120, 255);
    MxVector4UC restColor(60, 60, 160, 255);
    // lock button
    ShapeLockedSvg lockSvg;
    MxRectF buttonRect = drawData.rowRect;
    float buttonLeft = buttonRect.right() - buttonW;
    buttonRect.setLeft( buttonLeft );
    int lockIconName;
    if( isLocked() ) {
        lockIconName = MxThemeIcons::Locked;
    } else {
        lockIconName = MxThemeIcons::Unlocked;
    }
    if( drawData.isUnderMouse() && drawData.pButton == 2 ) { // hovering button
        lbTriangles.fillRect( buttonRect, MxVector4UC(100, 220, 30, 255) );
    }
    const MxRectF &lockIconRect = atlas->iconRect( lockIconName );
    // because the icon is smaller than the buttonRect, we center the icon inside buttonRect
    MxRectF centeredIconRect = MxGeometryUtils::centeredRect( buttonRect, atlas->iconSize( lockIconName ) );
    iconPainter.drawImageRect( lockIconRect, centeredIconRect );



    // show/hide button
    buttonRect.setRight(buttonLeft);
    buttonLeft -= buttonW;
    buttonRect.setLeft( buttonLeft );
    if( drawData.isUnderMouse() && drawData.pButton == 1 ) {
        lbTriangles.fillRect( buttonRect, MxVector4UC(100, 220, 30, 255) );
    }
    int visIconName;
    if( isVisible() ) {
        visIconName = MxThemeIcons::Visible;
    } else {
        visIconName = MxThemeIcons::NotVisible;
    }
    const MxRectF &visIconRect = atlas->iconRect( visIconName );
    centeredIconRect = MxGeometryUtils::centeredRect( buttonRect, atlas->iconSize( visIconName ) );
    bluePainter.drawImageRect( visIconRect, centeredIconRect );

    // invert button
    buttonRect.setRight(buttonLeft);
    buttonLeft -= buttonW;
    buttonRect.setLeft( buttonLeft );
    int invertIconName;
    if( isInverted() ) {
        invertIconName = MxThemeIcons::Inverted;
    } else {
        invertIconName = MxThemeIcons::NotInverted;
    }
    const MxRectF &invTextureRect = atlas->iconRect( invertIconName );
    centeredIconRect = MxGeometryUtils::centeredRect( buttonRect, atlas->iconSize( invertIconName ) );
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
