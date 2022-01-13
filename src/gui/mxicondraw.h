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
    MxIconDraw( MxBuffer *buffer, const MxVector2F *translation = 0 );
    virtual ~MxIconDraw();

    void clear();
    
    void drawImageRect( const MxRectF &source, const MxRectF &dest );





    const MxVector2F *pTranslation;





};

#endif // MXICONDRAW_H
