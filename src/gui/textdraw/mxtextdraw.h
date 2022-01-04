/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXTEXTDRAW_H
#define MXTEXTDRAW_H

#include "mxvector.h"
#include "mxrect.h"


class MxBuffer;



class MxTextDraw
{
public:
    MxTextDraw();


    const MxVector2F *pTranslation;
    MxBuffer *pArray;

    struct Vertex
    {
        float x;
        float y;

        float u;
        float v;

        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };



};

#endif
