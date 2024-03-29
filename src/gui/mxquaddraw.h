/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXQUADDRAW_H
#define MXQUADDRAW_H

#include "mxrect.h"

class MxBuffer;


class MxQuadDraw
{
public:
    MxQuadDraw();
    MxQuadDraw( MxBuffer *buffer );
    virtual ~MxQuadDraw();

    int vertexCount() const;

    // used by MotionVectors
    void drawCustomRect(const MxRectF &worldRect, const MxRectF &indexRect );


    MxBuffer *pArray;
};

#endif // MXQUADDRAW_H
