/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxlinearlayout.h"
#include "mxpainter.h"



MxLinearLayout::MxLinearLayout( MxLinearLayout::Orientation orientation )
{
    pOrientation = orientation;

    pLeftMargin = 0.0;
    pTopMargin = 0.0;
    pRightMargin = 0.0;
    pBottomMargin = 0.0;
    pSpacing = 0.0;
}

MxLinearLayout::~MxLinearLayout()
{}



/*! Sets the orientation of the layout, vertical or horizontal. */
void MxLinearLayout::setOrientation( MxLinearLayout::Orientation orientation )
{
    pOrientation = orientation;
}

/*! Returns the orientation of the layout.
\sa MxLinearLayout::setOrientation(). */
MxLinearLayout::Orientation MxLinearLayout::orientation() const
{
    return pOrientation;
}



/*!
    The margin is the space around the layout.
*/
void MxLinearLayout::setContentsMargins(float bottom, float left, float top, float right)
{
    if (pLeftMargin == left && pTopMargin == top && pRightMargin == right && pBottomMargin == bottom)
        return;

    pLeftMargin = left;
    pRightMargin = right;
    pTopMargin = top;
    pBottomMargin = bottom;
}


void MxLinearLayout::getContentsMargins(float *bottom, float *left, float *top, float *right) const
{
    if (left)
        *left = pLeftMargin;
    if (top)
        *top = pTopMargin;
    if (right)
        *right = pRightMargin;
    if (bottom)
        *bottom = pBottomMargin;
}

void MxLinearLayout::setSpacing( float spacing )
{
    pSpacing = spacing;
}


MxVector2F MxLinearLayout::sizePolicy()
{
    int listSize = count();
    Q_ASSERT( listSize > 0 );

    // check children, priorities are: 0, negative, positive
    MxWidget *child = widgetAt(0);
    MxVector2F layoutHint = child->sizePolicy(); // need to start with initialized data
    bool foundNegValue[2];
   foundNegValue[0] = layoutHint[0] < 0.0f;
   foundNegValue[1] = layoutHint[1] < 0.0f;
    layoutHint[0] = abs(layoutHint[0]);
    layoutHint[1] = abs(layoutHint[1]);
    bool foundZero[2];
    foundZero[0] = (layoutHint[0] == 0.0f);
    foundZero[1] = (layoutHint[1] == 0.0f);
    for(int i=1; i < listSize; ++i)
    {
        child = widgetAt(i);
        MxVector2F widgetSize = child->sizePolicy();
        if( widgetSize[0] < 0.0f ) {
            foundNegValue[0] = true;
        } else if( widgetSize[0] == 0.0f ) {
            foundZero[0] = true;
        }

        if( widgetSize[1] < 0.0f ) {
            foundNegValue[1] = true;
        } else if( widgetSize[1] == 0.0f ) {
            foundZero[1] = true;
        }

        float absX = abs(widgetSize[0]);
        float absY = abs(widgetSize[1]);
        if(pOrientation == Vertical)
        {
            if( absX > layoutHint[0] ) {
                layoutHint[0] = absX;
            }
            layoutHint[1] += absY;
        }
        else
        {
            if( absY > layoutHint[1] ) {
                layoutHint[1] = absY;
            }
            layoutHint[0] += absX;
        }


    }



    // add margins
    // at least 1 needs to be different from 0
    Q_ASSERT( layoutHint[0] != 0.0f || layoutHint[1] != 0.0f );
    layoutHint[0] += pLeftMargin + pRightMargin;
    layoutHint[1] += pBottomMargin + pTopMargin;


    if( foundZero[0] ) {
        layoutHint[0] = 0.0f;
    } else if( foundNegValue[0] ) {
        layoutHint[0] *= -1.0f;
    }

    if( foundZero[1] ) {
        layoutHint[1] = 0.0f;
    } else if( foundNegValue[1] ) {
        layoutHint[1] *= -1.0f;
    }

    return layoutHint;
}


void MxLinearLayout::sizeChanged()
{
    // resize child items
    const MxVector2F newSize = size();

    if( newSize.width() < 1.0f || newSize.height() < 1.0f )
        return;

    // to recalculate, go through child policies and set size to each child

    int expandItemsX = 0;
    int expandItemsY = 0;
    float fixedSizeX = 0.0;
    float fixedSizeY = 0.0;
    int canGrowItemsX = 0;
    int canGrowItemsY = 0;
    int listSize = count();
    for(int i=0; i < listSize; ++i)
    {
        MxWidget *child = widgetAt(i);
        MxVector2F widgetSize = child->sizePolicy();

        if( widgetSize.width() == 0 )
        {
            ++expandItemsX;
        }
        else if( widgetSize.width() < 0 )
        {
            fixedSizeX += -1*widgetSize.width();
            ++canGrowItemsX;
        }
        else
        {
            fixedSizeX += widgetSize.width(); // for fixed size and canGrow items
        }



        if( widgetSize.height() == 0 )
        {
            ++expandItemsY;
        }
        else if( widgetSize.height() < 0 )
        {
            fixedSizeY += -1*widgetSize.height();
            ++canGrowItemsY;
        }
        else
        {
            fixedSizeY += widgetSize.height(); // for fixed size and canGrow items

        }


    }

    // set sizes
    int freeSize[2];
    freeSize[0] = newSize.width() - fixedSizeX;
    freeSize[1] = newSize.height() - fixedSizeY;
    freeSize[Horizontal] -= (pLeftMargin + pRightMargin);
    freeSize[Vertical] -= (pBottomMargin + pTopMargin);

    // \TODO use Orientation flags?
    freeSize[pOrientation] -= (count() - 1) * pSpacing;

    int newExpandSize[2];
    int canGrowSize[2];
    int leftOver;

    float varPos;
    if(pOrientation == Horizontal)
    {
        if( expandItemsX )
        {
            // split freeSize between all expand items
            newExpandSize[Horizontal] = freeSize[Horizontal] / expandItemsX;
            canGrowSize[0] = 0;
            leftOver = freeSize[Horizontal] % expandItemsX;
        }
        else if( canGrowItemsX )
        {
            // split freeSize between all canGrow items (add as extra space)
            newExpandSize[Horizontal] = 0;
            canGrowSize[Horizontal] = freeSize[Horizontal] / canGrowItemsX;
            leftOver = freeSize[Horizontal] % canGrowItemsX;
        }
        else
        {
             canGrowSize[0] = 0;
            newExpandSize[Horizontal] = 0;
            leftOver = 0;
        }


        varPos = pLeftMargin;
    }
    else
    {
        if( expandItemsY )
        {
            // split freeSize between all expand items
            newExpandSize[Vertical] = freeSize[Vertical] / expandItemsY;
            canGrowSize[Vertical] = 0;
            leftOver = freeSize[Vertical] % expandItemsY;
        }
        else if( canGrowItemsY )
        {
            // split freeSize between all canGrow items (add as extra space)
            newExpandSize[Vertical] = 0;
            canGrowSize[Vertical] = freeSize[Vertical] / canGrowItemsY;
            leftOver = freeSize[Vertical] % canGrowItemsY;
        }
        else
        {
             canGrowSize[Vertical] = 0;
            newExpandSize[Vertical] = 0;
            leftOver = 0;
        }

        varPos = pBottomMargin;
    }

    Q_ASSERT( leftOver <= 1 ); // if assert here add a spacer to the layout

    MxVector2F iSize;
    for(int i=0; i < listSize; ++i)
    {
        MxWidget *child = widgetAt(i);
        MxVector2F widgetSize = child->sizePolicy();

        if(pOrientation == Horizontal)
        {
            // set sizes
            if( widgetSize.width() == 0 )
            {
                if( leftOver )
                {
                    iSize.setWidth(newExpandSize[0] + leftOver);
                    leftOver = 0;
                } else {
                    iSize.setWidth(newExpandSize[0]);
                }
            }
            else if( widgetSize.width() < 0 )
            {
                if( leftOver )
                {
                    iSize.setWidth( -1*widgetSize.width() + canGrowSize[0] + leftOver);
                    leftOver = 0;
                } else {
                iSize.setWidth( -1*widgetSize.width() + canGrowSize[0]);
                }
            }
            else
            {
                iSize.setWidth( widgetSize.width() );
            }

            if( widgetSize.height() > 0 )
                iSize.setHeight(widgetSize.height());
            else
                iSize.setHeight( newSize.height() - (pBottomMargin + pTopMargin));


            child->setSize( iSize );
            child->setPos( MxVector2F(varPos, pBottomMargin) );
            varPos += iSize.width() + pSpacing;
        }
        else
        {

            if( widgetSize.height() == 0.0f )
            {
                if( leftOver )
                {
                    iSize.setHeight(newExpandSize[1] + leftOver);
                    leftOver = 0;
                } else {
                    iSize.setHeight(newExpandSize[1]);
                }

            }
            else if( widgetSize.height() < 0.0f )
            {
                if( leftOver )
                {
                    iSize.setHeight( -1*widgetSize.height() + canGrowSize[1] + leftOver);
                    leftOver = 0;
                } else {
                   iSize.setHeight( -1*widgetSize.height() + canGrowSize[1]);
                }
            }
            else
            {
                iSize.setHeight( widgetSize.height() );
            }


            if( widgetSize.width() > 0 )
                iSize.setWidth(widgetSize.width());
            else // set full layout width
                iSize.setWidth(newSize.width() - (pLeftMargin + pRightMargin));

            child->setSize( iSize );

            child->setPos( MxVector2F(pLeftMargin, varPos) );
            varPos += iSize.height() + pSpacing;

        }
    }



}



void MxLinearLayout::paint(MxPainter &painter )
{
    if( pSpacing > 0.0f
            || pLeftMargin > 0.0f
            || pTopMargin > 0.0f
            || pRightMargin > 0.0f
            || pBottomMargin > 0.0f )
    {

        MxRectF drawArea;
        drawArea.setRectFromPointAndSize( MxVector2F(0.0f, 0.0f), size() );

        const MxVector4UC& backColor = parentBackgroundColor();

        painter.vectorDraw().fillRect(drawArea, backColor);
    }

    MxLayout::paint(painter);
}


void MxLinearLayout::dirtyCollidingWidgets( const MxRectF &collidingRect/*, MxWidgetList &list*/ )
{

    // Check for margins and spacing and update accordingly

    MxRectF thisRect;
    getRect( &thisRect );

    // assume that all layouts with spacing have margins. Layouts with spacing/margins need to be redrawn
    if( collidingRect.intersects(thisRect) && (pLeftMargin > 0.0f || pTopMargin > 0.0f || pRightMargin > 0.0f || pBottomMargin > 0.0f) )
    {
        pAttributes |= DirtyFlag;
        return;
    }

    // map collidingRect to layout
    MxLayout::dirtyCollidingWidgets( collidingRect );
}

// resSize is initialized to 1.0f
void MxLinearLayout::calcBiggestSizeHint( float widgetSize, float &resSize ) const
{
    // calc biggest size hint

    bool canGrow = (widgetSize <= 0.0f) || (resSize <= 0.0f);
    resSize = abs( (int)resSize ) + abs( (int)widgetSize);
    if( canGrow ) {
        resSize *= -1.0f;
    }
}


void MxLinearLayout::calcSmallestSizeHint( float widgetSize, float &resSize ) const
{

    // resSize stores the biggest size (0.0 is the biggest)
    if(  widgetSize == 0.0f )
        return;


    if( widgetSize < 0.0f ) {
        widgetSize *= -1.0f;
    }

    resSize = std::max(widgetSize, resSize);

}

