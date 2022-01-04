/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXICONDRAW_H
#define MXICONDRAW_H

#include "mxquaddraw.h"
#include "mxthemeicons.h"


class QxRenderer;
class MxAbstractAtlas;



/*!
  \brief The MxIconDraw class draws icons from a atlas
*/
class MxIconDraw : public MxQuadDraw
{
public:
    MxIconDraw();
    MxIconDraw( MxBuffer *buffer, const MxVector2F *translation );
    virtual ~MxIconDraw();

    void clear();

    void drawImageRect( const MxRectF &source, const MxRectF &dest );
    void drawImageRectRotated( const MxRectF &source, const MxRectF &dest, float angle );
    //void drawIconCentered( const QxRectF buttonRect, QxStyle::ImageName name );



   /*!
   This function is for drawing a frame which, not stretching the borders.
    The  margins of a rectangle are the border values.
   Draws the binding texture into the given target rectangle.
   The binding texture will be split into nine segments and drawn according to the margins structure.
   Margins are fixed size, inner rect is stretched.

   This is used e.g. popups.
   */
   void draw3x3Frame(const MxRectF &target, const MxRectI &margins, const MxAbstractAtlas *style, MxThemeIcons::IconName name, bool flipVertically = false );
   void draw3x1Frame( const MxRectF &target, const MxVector2I &margins, const MxAbstractAtlas *style, MxThemeIcons::IconName name, bool flipVertically );




    const MxVector2F *pTranslation;





};

#endif // MXICONDRAW_H
