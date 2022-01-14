/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXICONDRAW_H
#define MXICONDRAW_H


#include "mxthemeicons.h"
#include "mxrect.h"

class QxRenderer;
class MxAbstractAtlas;
class MxBuffer;


/*!
  \brief The MxIconDraw class draws icons from a atlas
*/
class MxIconDraw
{
public:
    MxIconDraw();
    MxIconDraw( MxBuffer *buffer, const MxVector2F *translation );
    virtual ~MxIconDraw();

    void clear();
    int vertexCount() const;
    void drawImageRectColored( const MxRectF &source, const MxRectF &dest, const MxVector4UC &color );
    void drawImageRect( const MxRectF &source, const MxRectF &dest );




   struct Vertex
   {
       float x;
       float y;

      MxVector4UC color;


       float u;
       float v;
   };


    const MxVector2F *pTranslation;
    MxBuffer *pArray;

};

#endif // MXICONDRAW_H
