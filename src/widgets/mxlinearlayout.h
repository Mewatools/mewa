/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXLINEARLAYOUT_H
#define MXLINEARLAYOUT_H

#include "mxdebug.h"
#include "mxlayout.h"


/*!
\class MxLinearLayout
\ingroup gui
\brief Vertical/Horizontal layout

Use MxLinearLayout to lay widgets in vertically or horizontally.
*/
class MxLinearLayout : public MxLayout
{
public:
    enum Orientation{ Horizontal = 0, Vertical = 1 };


    MxLinearLayout( Orientation orientation = Horizontal );
    virtual ~MxLinearLayout();


    void setOrientation( Orientation orientation );
    Orientation orientation() const;

    void setContentsMargins( float bottom, float left, float top, float right );
    void getContentsMargins( float *bottom, float *left, float *top, float *right ) const;

    void setSpacing( float spacing );

    virtual MxVector2F sizePolicy();

    /*! Calculates and set pos/size for layout items. */
    virtual void sizeChanged();

    virtual void paint(MxPainter &painter );


    float pLeftMargin, pTopMargin, pRightMargin, pBottomMargin;
    float pSpacing;


private:
    virtual void dirtyCollidingWidgets(const MxRectF &collidingRect);
    inline void calcBiggestSizeHint( float widgetSize, float &resSize ) const;
    inline void calcSmallestSizeHint( float widgetSize, float &resSize ) const;

    Orientation pOrientation;


};

#endif

