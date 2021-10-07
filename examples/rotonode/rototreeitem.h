/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef ROTOTREEITEM_H
#define ROTOTREEITEM_H

#include "mxstring.h"
#include "mxrect.h"
#include "mxlist.h"


class MxPainter;
class MxVectorDraw;


class RotoTreeItem
{
public:
    RotoTreeItem( RotoTreeItem *parent );
    virtual ~RotoTreeItem();


    enum ItemType
    {
        ShapeType,
        TransformType
    };

    virtual ItemType itemType() const = 0;
    virtual RotoTreeItem* siblingAtRow(int row) const = 0;

    // \TODO rename to RowData
    struct DrawData {
        int rowIndex;
        float textX;
        MxRectF rowRect;
        float rowH;


        bool pButtonPressed;
        int pButton; // -1 if no button pressed
        int pHighlightRow;
        //MxList<int, MxPodInitializer<int> > pSelectedRows;

        //bool isSelected() const;
        bool isUnderMouse() const;
    };

    virtual void paint( MxPainter &painter, DrawData &drawData ) const = 0;
    // returns percentage of points/shapes selected
    virtual bool isSelected() const = 0;
    virtual RotoTreeItem* itemAtRow( int *rowIndex, int *transformCount ) = 0;
     virtual void clearSelection() = 0;
    virtual void selectRow() = 0;


    void fillRow(MxVectorDraw &lbTriangles, DrawData &rowData ) const;
    static float rowHeight();
    void drawBottomSeparator( MxVectorDraw &lbTriangles, const MxRectF &rowRect ) const;

    RotoTreeItem *pParent;
    MxString pName;
        bool pSelected;
};

#endif // ROTOTREEITEM_H
