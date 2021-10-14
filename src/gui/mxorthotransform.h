/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXORTHOTRANSFORM_H
#define MXORTHOTRANSFORM_H

#include "mxvector.h"
#include "mxrect.h"



/*!
 * \brief The MxOrthoTransform is used to map mouse coordinates to view coordinates
  and vice-versa.
\code
    pMousePressedPos = event.pos();
    pMousePressedPos = pOutputViewTransform.map(pMousePressedPos);
\endcode

  To set the projection matrix:
\code
    MxMatrix matrix;
    matrix.setToIdentity();
    matrix.ortho(0.0f, viewSize.width(), 0.0f, viewSize.height(), -1.0, 1.0);
    matrix.scale( pOutputViewTransform.pScale.x(), pOutputViewTransform.pScale.y() );
    matrix.translate( -pOutputViewTransform.pTranslation.x(), -pOutputViewTransform.pTranslation.y() );
\endcode

  To translate view objects, get the mouse offset and scale it:
\code
     MxVector2F mousePosOffset = event.globalPos() - pLastMousePos;
     mousePosOffset = mousePosOffset / pOutputViewTransform.pScale;
     pOutputViewTransform.pTranslation -= mousePosOffset;
\endcode
 */
class MxOrthoTransform
{
public:

    MxOrthoTransform()
    {
        pTranslation = MxVector2F(0.0f, 0.0f);
        pScale = MxVector2F(1.0f, 1.0f);
    }

    MxVector2F viewToScene( const MxVector2F &point ) const
    {
        MxVector2F r = point * 1.0f / pScale;
        r += pTranslation;
        return r;
    }

    MxRectF viewToSceneRect( const MxRectF &rect ) const
    {
        MxVector2F lb = viewToScene( rect.bottomLeft() );
        MxVector2F tr = viewToScene( rect.topRight() );
        MxRectF r(lb, tr);
        return r;
    }

    float viewToSceneX( float x )
    {
        float r = x / pScale[0] + pTranslation[0];
        return r;
    }

    float viewToSceneY( float y )
    {
        float r = y / pScale[1] + pTranslation[1];
        return r;
    }

    MxVector2F fromSceneToView( const MxVector2F &point ) const
    {
        MxVector2F r = point - pTranslation;
        r *= pScale;
        return r;
    }

    MxRectF fromSceneToViewRect( const MxRectF &rect ) const
    {
        MxVector2F lb = fromSceneToView( rect.bottomLeft() );
        MxVector2F tr = fromSceneToView( rect.topRight() );
        MxRectF r(lb, tr);
        return r;
    }

    float fromSceneToViewY( float y ) const
    {
        float r = y;
        r -= pTranslation[1];
        r *= pScale[1];
        return r;
    }


    ////////////// DATA MEMBERS /////////////
    MxVector2F pTranslation;
    MxVector2F pScale;

};

#endif

