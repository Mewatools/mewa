/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "rototreeview.h"
#include "mxpainter.h"
#include "mxmouseevent.h"


#define MARGIN 1.0f
#define LINE_SEPARATOR_THICKNESS 2.0f


RotoTreeView::RotoTreeView()
{
    pDrawData.pButton = -1;
    pDrawData.pHighlightRow = -1;
    pRowCount = 1;
}

RotoTreeView::~RotoTreeView()
{}

RotoShape* RotoTreeView::newRotoShape()
{
    RotoShape *newShape = new RotoShape( &pRootTransform );

    pRowCount++;
    sizeChanged();
    return newShape;
}

void RotoTreeView::removeShape(  RotoShape *shape )
{
    RotoTreeItem *p = shape->pParent;
    Q_ASSERT( p->itemType() == RotoTreeItem::TransformType );
    RotoTransform *trf = static_cast<RotoTransform*>(p);
    bool done = trf->pChildren.removeOne(shape);
    Q_ASSERT( done == true );

    pRowCount--;
    sizeChanged();

}

MxVector2F RotoTreeView::sizePolicy()
{
    float h = pRowCount * RotoTreeItem::rowHeight();
    return MxVector2F(0.0f, h + (2.0f * MARGIN) );
}

void RotoTreeView::sizeChanged()
{

}

void RotoTreeView::mousePressEvent( MxMouseEvent &event )
{
    const MxVector2F &mousePos = event.pos();
    const MxVector2F &areaSize = size();
    int rowH = rowHeight();

    float yy = areaSize.height() - (mousePos.y() + MARGIN);
    pDrawData.pHighlightRow = ((int)yy) / rowH;
    pDrawData.pButtonPressed = true;
    selectRow( pDrawData.pHighlightRow, mousePos.x() );



    event.accept(this); // accept after getting mouse pos

}

void RotoTreeView::mouseMoveEvent( MxMouseEvent &event )
{

    if( event.isAccepted() ) // dragging
    {

    }
    else
    {
        const MxVector2F &mousePos = event.pos();
        const MxVector2F &areaSize = size();
        int rowH = rowHeight();

        float yy = areaSize.height() - (mousePos.y() + MARGIN);
        int hoveringRow = ((int)yy) / rowH;
        //qDebug( "yy %f row: %d",yy, hoveringRow);
        setHoveringRow( hoveringRow );

        if( pDrawData.pHighlightRow >= 0 )
        {
            // find column
            const float buttonW = buttonWidth();
            const float allButtonsW = (ButtonCount * buttonW);
            float xx = mousePos.x();
            float allButtonsRight = areaSize.width() - MARGIN;
            float allButtonsLeft = allButtonsRight - allButtonsW;

            if( xx >= allButtonsLeft && xx <= allButtonsRight ) {
                // highlight button

                xx -= allButtonsLeft;

                int buttonIndex = (int)(xx / buttonW);
                setHoveringButton( buttonIndex );
                //  qDebug("button %d, xx %f", button, xx);

            } else {
                //qDebug("button NO");
                setHoveringButton( -1 );
                //setHoveringRow( -1 );
            }
        }

    }

}

void RotoTreeView::mouseReleaseEvent( MxMouseEvent &event )
{
    pDrawData.pButtonPressed = false;
}

void RotoTreeView::paint( MxPainter &painter )
{

    const MxVector2F &areaSize = size();
    MxRectF areaRect(areaSize);
    MxVectorDraw &lbTriangles = painter.vectorDraw();

    MxVector4UC contourColor(30, 30, 30, 255);
    MxVector4UC fillColor(60, 60, 60, 255);

    lbTriangles.fillRect( areaRect, contourColor);
    areaRect.adjust( MARGIN, -MARGIN, MARGIN, -MARGIN );
    lbTriangles.fillRect( areaRect, fillColor);

    // const float buttonW = buttonWidth();


    RotoTransform &root = rootTransform();
    //ShapeList &list = root.pChildren;



    pDrawData.rowH = rowHeight();

    pDrawData.rowRect = areaRect;
    float yy = areaSize.height() - MARGIN;
    yy -=  pDrawData.rowH;
    pDrawData.rowRect.setBottom(yy);
    pDrawData.rowRect.setTop(yy +  pDrawData.rowH);

    pDrawData.textX = MARGIN;

    pDrawData.rowIndex = 0;

    root.paint(painter, pDrawData);
}

void RotoTreeView::clearAllSelections()
{
    pRootTransform.clearSelection();
}

RotoTransform & RotoTreeView::rootTransform()
{
    return pRootTransform;
}

int RotoTreeView::rowHeight() const
{
    return 22;
}

float RotoTreeView::buttonWidth()
{
    return 22.0f;
}

void RotoTreeView::selectRow( int row, float mouseX )
{
    //pDrawData.pSelectedRows.clear();


    // find item at row

    int transformCount = 0;
    RotoTreeItem *item =pRootTransform.itemAtRow( &row, &transformCount );
    // qDebug("item %p", item);
    bool doSelect = true;
    if( item ) {
        if( item->itemType() == RotoTreeItem::TransformType ) {
            // check if hit expand button
            mouseX -= MARGIN;
            float buttonW = RotoTransform::expandButtonWidth();
            float buttonX = ((transformCount-1) * buttonW);
            if( mouseX > buttonX && mouseX < (buttonX+buttonW)) {
                RotoTransform *trf = dynamic_cast<RotoTransform*>(item);
                trf->toggleExpand();
                update();
                doSelect = false;

            }
        } else {

            // check if hit button

            RotoShape *shape = dynamic_cast<RotoShape*>(item);
            int button = buttonAtPos( mouseX );
            if( button >= 0 ) {
                doSelect = false;

                //qDebug("button pressed %d", button);
                switch( button )
                {
                case InvertButton:
                    shape->toggleInvert();
                    break;
                case LockButton:
                    shape->toggleLock();
                    break;
                case ViewButton:
                    shape->toggleVisibility();
                    break;
                default:
                    qFatal("Missing Tree view button");
                    break;
                }
            }



        }

        if( doSelect ) {
            pRootTransform.clearSelection();
            item->selectRow();
        }
    } else {
        // no row hit
        pRootTransform.clearSelection();
    }



    /* if( row >= 0 && row < rootTransform().size() )
    {
    pSelectedRows.append(row);
    }*/

    update();
}

void RotoTreeView::setHoveringRow( int row )
{
    if( pDrawData.pHighlightRow != row ) {
        pDrawData.pHighlightRow = row;
        update();
    }
}

void RotoTreeView::setHoveringButton( int buttonIndex )
{
    if( pDrawData.pButton != buttonIndex ) {
        pDrawData.pButton = buttonIndex;
        update();
    }
}

int RotoTreeView::buttonAtPos( float mouseX )
{
    const float buttonW = buttonWidth();
    const float allButtonsW = (ButtonCount * buttonW);
    float allButtonsRight = size().width() - MARGIN;
    float allButtonsLeft = allButtonsRight - allButtonsW;
    int button = -1;
    if( mouseX >= allButtonsLeft && mouseX <= allButtonsRight ) {
        // highlight button

        mouseX -= allButtonsLeft;

        button = (int)(mouseX / buttonW);

        //  qDebug("button %d, xx %f", button, xx);

    }

    return button;
}

