/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxicondraw.h"
#include "mxgeometryutils.h"
#include "mxrenderer.h"
#include "mxbuffer.h"
#include "mxabstractatlas.h"


MxIconDraw::MxIconDraw()
{
    pArray = NULL;
    pTranslation = NULL;
}


MxIconDraw::MxIconDraw(MxBuffer *buffer, const MxVector2F *translation )
{
    pArray = buffer;
    pTranslation = translation;
    Q_ASSERT( pArray->size() == 0 );
}

MxIconDraw::~MxIconDraw()
{}

void MxIconDraw::clear()
{
    Q_ASSERT( (NULL != pArray) && (NULL != pTranslation) );

    //pVertices = 0;
    pArray->clear();
}

int MxIconDraw::vertexCount() const
{
    Q_ASSERT( NULL != pArray );
    int r = pArray->size() / sizeof(Vertex);
    return r;
}

void MxIconDraw::drawImageRectColored( const MxRectF &source, const MxRectF &dest, const MxVector4UC &color )
{
    Q_ASSERT( (NULL != pArray) && (NULL != pTranslation) );

    Vertex *dst = (Vertex*) pArray->lastDataAndIncrement( 6 * sizeof(Vertex) );

    const float texBottom = source.bottom();
    const float texTop = source.top();
    const float texLeft = source.left();
    const float texRight = source.right();

    const float left = dest.left() + (*pTranslation)[0];
    const float bottom = dest.bottom() + (*pTranslation)[1];
    const float right  = dest.right() + (*pTranslation)[0];
    const float top = dest.top() + (*pTranslation)[1];

    // left bottom
    dst->x = left;
    dst->y = bottom;
    dst->color = color;
    dst->u = texLeft;
    dst->v = texBottom;
    dst++;

    // right bottom
    dst->x = right;
    dst->y = bottom;
    dst->color = color;
    dst->u = texRight;
    dst->v = texBottom;
    dst++;

    // left top
    dst->x = left;
    dst->y = top;
    dst->color = color;
    dst->u = texLeft;
    dst->v = texTop;
    dst++;

    // right top
    dst->x = right;
    dst->y = top;
    dst->color = color;
    dst->u = texRight;
    dst->v = texTop;
    dst++;

    // left top
    dst->x = left;
    dst->y = top;
    dst->color = color;
    dst->u = texLeft;
    dst->v = texTop;
    dst++;

    // right bottom
    dst->x = right;
    dst->y = bottom;
    dst->color = color;
    dst->u = texRight;
    dst->v = texBottom;
}

void MxIconDraw::drawImageRect( const MxRectF &source, const MxRectF &dest )
{
    MxVector4UC whiteColor(255, 255, 255, 255);
    drawImageRectColored( source, dest, whiteColor );
}
