/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "rotoshape.h"
#include "mxorthotransform.h"
#include "rototransform.h"
#include "mxpainter.h"
#include "shapelockedsvg.h"


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
    qDebug("isLocked(): %d", (int)isLocked());
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

    // invert button
    buttonRect.setRight(buttonLeft);
    buttonLeft -= buttonW;
    buttonRect.setLeft( buttonLeft );
    if( drawData.isUnderMouse() && drawData.pButton == 0 ) {
        lbTriangles.fillRect( buttonRect, MxVector4UC(10, 220, 230, 255) );
    } else{
        lbTriangles.fillRect( buttonRect, MxVector4UC(110, 220, 130, 255) );
    }


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

#ifndef TEST_ROTOTREEVIEW
void RotoShape::fillPolygon( MxCubicBezierLineEffect::Polygon &polygon, int frame, const MxOrthoTransform &transform )
{

    if( RotoShape::QuadraticInterface == pCurveInterface )
    {

        RotoPoint *previousHandler = &(pPointList[0]);
        polygon.setInitialPoint( transform.fromSceneToView(previousHandler->pos(frame)) );

        MxVector2F b,c;

        int handlerCount = pPointList.size();
        Q_ASSERT( handlerCount > 1 );
        RotoPoint *iHandler;
        for(int i=1; i<handlerCount; ++i)
        {
            iHandler = &(pPointList[i]);
            //Q_ASSERT( previousHandler->pInterpolation == RotoPoint::Quadratic );
            b = transform.fromSceneToView( /*previousHandler->pos(frame) +*/ previousHandler->pFrontHandler.pos(frame) );
            /*} else {
            b = transform.fromSceneToView( previousHandler->pos(frame) );
        }*/

            /*if( iHandler->pInterpolation == RotoPoint::Quadratic ) {
            c = transform.fromSceneToView( iHandler->pos(frame) + iHandler->backHandler.pos(frame) );
        } else {
            c = transform.fromSceneToView( iHandler->pos(frame) );
        }*/
            polygon.bezierTo( b, b , transform.fromSceneToView(iHandler->pos(frame)) );
            previousHandler = iHandler;
        }

        if( isClosed() )
        {
            previousHandler = &(pPointList[0]);

            // Q_ASSERT( iHandler->pInterpolation == RotoPoint::Quadratic );
            b = transform.fromSceneToView(/*iHandler->pos(frame) +*/ iHandler->pFrontHandler.pos(frame));
            /*} else {
            b = transform.fromSceneToView(iHandler->pos(frame));
        }*/

            /*if( previousHandler->pInterpolation == RotoPoint::Quadratic ) {
            c = transform.fromSceneToView(previousHandler->pos(frame) + previousHandler->backHandler.pos(frame));
        } else {
            c = transform.fromSceneToView(previousHandler->pos(frame));
        }*/

            polygon.bezierTo( b, b, transform.fromSceneToView(previousHandler->pos(frame)) );
        }

    }
    else if( RotoShape::CubicInterface == pCurveInterface )
    {
        int handlerCount = pPointList.size();
        if(handlerCount > 2)
        {
            RotoPoint *previousHandler = &(pPointList[0]);
            MxVector2F prevPos;
            previousHandler->getPos(&prevPos, frame);

            /*RotoPoint *nextHandler = &(pPointList[1]);
            MxVector2F nextPos;
            nextHandler->getPos(&nextPos, frame);*/

            MxVector2F aScnPos = transform.fromSceneToView(prevPos);
            //MxVector2F firstPos = aScnPos;


            polygon.setInitialPoint( aScnPos );

            Q_ASSERT( handlerCount > 1 );
            for(int i=1; i<handlerCount; ++i)
            {
                RotoPoint *nextHandler = &(pPointList[i]);
                MxVector2F nextPos;
                nextHandler->getPos(&nextPos, frame);
                MxVector2F dScnPos = transform.fromSceneToView( nextPos );

                MxVector2F frontCtrl;
                previousHandler->pFrontHandler.getPos( &frontCtrl, frame );

                MxVector2F backCtrl;
                nextHandler->pBackHandler.getPos( &backCtrl, frame );

                MxVector2F b = aScnPos + frontCtrl;
                MxVector2F c = dScnPos + backCtrl;
                MxVector2F mid = b + ((c - b) / 2.0f);


                polygon.bezierTo( b, b , mid );
                polygon.bezierTo( c, c , dScnPos );

                previousHandler = nextHandler;
                aScnPos = dScnPos;
            }

            if( isClosed() )
            {
                RotoPoint *nextHandler = &(pPointList[0]);
                MxVector2F nextPos;
                nextHandler->getPos(&nextPos, frame);
                MxVector2F dScnPos = transform.fromSceneToView( nextPos );

                MxVector2F frontCtrl;
                previousHandler->pFrontHandler.getPos( &frontCtrl, frame );

                MxVector2F backCtrl;
                nextHandler->pBackHandler.getPos( &backCtrl, frame );

                MxVector2F b = aScnPos + frontCtrl;
                MxVector2F c = dScnPos + backCtrl;
                MxVector2F mid = b + ((c - b) / 2.0f);


                polygon.bezierTo( b, b , mid );
                polygon.bezierTo( c, c , dScnPos );


            }
        }


    }
    else
    {

        Q_ASSERT( RotoShape::BSplineInterface == pCurveInterface );
        int handlerCount = pPointList.size();
        if(handlerCount > 2)
        {

            RotoPoint *previousHandler = &(pPointList.last());
            MxVector2F prevPos;
            previousHandler->getPos(&prevPos, frame);
            MxVector2F prevScnPos = transform.fromSceneToView( prevPos );
            MxVector2F lastScnPos = prevScnPos;

            RotoPoint *iHandler = &(pPointList[0]);
            MxVector2F iPos;
            iHandler->getPos(&iPos, frame);
            MxVector2F iScnPos = transform.fromSceneToView( iPos );

            MxVector2F prevMiddleScnPos = iScnPos + ((prevScnPos - iScnPos) / 2.0f);
            polygon.setInitialPoint( prevMiddleScnPos );

            MxVector2F startPos = prevMiddleScnPos;

            // iScnPos = prevScnPos;

            Q_ASSERT( handlerCount > 1 );
            for(int i=1; i<handlerCount; ++i)
            {
                RotoPoint *nextHandler = &(pPointList[i]);
                MxVector2F nextPos;
                nextHandler->getPos(&nextPos, frame);
                MxVector2F nextScenePos = transform.fromSceneToView( nextPos );

                MxVector2F nextMiddleScnPos = iScnPos + (( nextScenePos - iScnPos) / 2.0f);


                polygon.bezierTo( iScnPos, iScnPos , nextMiddleScnPos );

                iScnPos = nextScenePos;
                // previousHandler = iHandler;
            }

            if( isClosed() )
            {
                polygon.bezierTo( lastScnPos, lastScnPos , startPos );
            }
        } else if( handlerCount == 2 ) {
            // draw line

            RotoPoint *iHandler = &(pPointList[0]);
            MxVector2F iPos;
            iHandler->getPos(&iPos, frame);
            MxVector2F startPos = transform.fromSceneToView( iPos );

            polygon.setInitialPoint( startPos );

            iHandler = &(pPointList[1]);
            iHandler->getPos(&iPos, frame);
            MxVector2F endPos = transform.fromSceneToView( iPos );
            polygon.bezierTo( startPos, endPos , endPos );


        }


    }

}
#endif
