/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXQUADDRAW_H
#define MXQUADDRAW_H

#include "gpubuffer.h"
#include "mxrect.h"


class MxQuadDraw
{
public:
    MxQuadDraw( GpuBuffer *buffer );

    int vertexCount() const;

    // used by MotionVectors
    void drawCustomRect(const MxRectF &worldRect, const MxRectF &indexRect );


    GpuBuffer *pArray;
};

#endif // MXQUADDRAW_H
