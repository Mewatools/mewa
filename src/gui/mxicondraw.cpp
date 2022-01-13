/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxicondraw.h"
#include "mxgeometryutils.h"
#include "mxrenderer.h"
#include "mxabstractatlas.h"
#include "mxbuffer.h"


MxIconDraw::MxIconDraw()
    : MxQuadDraw(NULL)
{
    pTranslation = NULL;
}


MxIconDraw::MxIconDraw(MxBuffer *buffer, const MxVector2F *translation )
    : MxQuadDraw(buffer)
{
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


void MxIconDraw::drawImageRect( const MxRectF &source, const MxRectF &dest )
{
    Q_ASSERT( NULL != pArray );

    float *dst = (float*) pArray->lastDataAndIncrement( 24 * sizeof(float) );

    const float texBottom = source.bottom();
    const float texTop = source.top();
    const float texLeft = source.left();
    const float texRight = source.right();

    float left, bottom, right, top;
    left = dest.left();
    bottom = dest.bottom();
    right = dest.right();
    top = dest.top();
    if (pTranslation) {
        left += (*pTranslation)[0];
        bottom += (*pTranslation)[1];
        right += (*pTranslation)[0];
        top += (*pTranslation)[1];
    }


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

    //pVertices += 6;
}



