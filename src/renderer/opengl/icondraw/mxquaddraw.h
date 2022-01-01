/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXQUADDRAW_H
#define MXQUADDRAW_H

#include "mxcachedgpuarray.h"
#include "mxrect.h"


class MxQuadDraw
{
public:
    MxQuadDraw( MxCachedGpuArray *buffer );

    int vertexCount() const;

    // used by MotionVectors
    void drawCustomRect(const MxRectF &worldRect, const MxRectF &indexRect );


    MxCachedGpuArray *pArray;
};

#endif // MXQUADDRAW_H
