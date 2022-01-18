/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxabstractatlas.h"


MxAbstractAtlas::MxAbstractAtlas()
{
}

MxAbstractAtlas::~MxAbstractAtlas()
{}


void MxAbstractAtlas::getPixelSize( MxVector2F *pixelSize ) const
{
    MxVector2I texSize = textureSize();
    float w = 1.0f/texSize[0];
    float h = 1.0f/texSize[1];
    (*pixelSize) = MxVector2I(w, h);
}

MxTexture* MxAbstractAtlas::texture()
{
    return &pTexture;
}

 bool MxAbstractAtlas::isLoaded() const
 {
     return (! pTexture.isNull());
 }

