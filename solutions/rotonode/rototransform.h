/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef ROTOTRANSFORM_H
#define ROTOTRANSFORM_H

#include "rototreeitem.h"
#include "shapelist.h"


class RotoTransform : public RotoTreeItem
{
public:
    RotoTransform(RotoTransform *parent = NULL );

    virtual RotoTreeItem::ItemType itemType() const;
virtual RotoTreeItem* siblingAtRow(int row) const;
virtual void paint( MxPainter &painter, DrawData &drawData ) const;
virtual bool isSelected() const;
virtual RotoTreeItem* itemAtRow(int *rowIndex , int *transformCount);
 virtual void clearSelection();
 virtual void selectRow();

    void toggleExpand();
   static float expandButtonWidth();

    ShapeList pChildren;
    bool pExpanded;

};

#endif // ROTOTRANSFORM_H
