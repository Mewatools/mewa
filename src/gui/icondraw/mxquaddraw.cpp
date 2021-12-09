/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxquaddraw.h"



MxQuadDraw::MxQuadDraw( GpuBuffer *buffer )
{
    pArray = buffer;
}

int MxQuadDraw::vertexCount() const
{
    Q_ASSERT( NULL != pArray );


    int r = pArray->size() / (4 * sizeof(float));
    return r;
}

void MxQuadDraw::drawCustomRect( const MxRectF &worldRect, const MxRectF &indexRect )
{
    Q_ASSERT( NULL != pArray );

    float *dst = (float*) pArray->lastDataAndIncrement( 24 * sizeof(float) );

    const float texBottom = indexRect.bottom();
    const float texTop = indexRect.top();
    const float texLeft = indexRect.left();
    const float texRight = indexRect.right();

    const float left = worldRect.left();
    const float bottom = worldRect.bottom();
    const float right  = worldRect.right();
    const float top = worldRect.top();

    // left bottom
    *dst = left;
    dst++;
    *dst = bottom;
    dst++;
    *dst = texLeft;
    dst++;
    *dst = texBottom;
    dst++;
    // right bottom
    *dst = right;
    dst++;
    *dst = bottom;
    dst++;
    *dst = texRight;
    dst++;
    *dst = texBottom;
    dst++;
    // left top
    *dst = left;
    dst++;
    *dst = top;
    dst++;
    *dst = texLeft;
    dst++;
    *dst = texTop;
    dst++;
    // right top
    *dst = right;
    dst++;
    *dst = top;
    dst++;
    *dst = texRight;
    dst++;
    *dst = texTop;
    dst++;
    // left top
    *dst = left;
    dst++;
    *dst = top;
    dst++;
    *dst = texLeft;
    dst++;
    *dst = texTop;
    dst++;
    // right bottom
    *dst = right;
    dst++;
    *dst = bottom;
    dst++;
    *dst = texRight;
    dst++;
    *dst = texBottom;

}


