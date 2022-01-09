/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxabstractatlas.h"


MxAbstractAtlas::MxAbstractAtlas()
{
    pTexture = 0;
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

#ifdef MX_DIRECTX12_RENDERER
MxTexture * MxAbstractAtlas::texture()
#else
unsigned int MxAbstractAtlas::texture() const
#endif
{
    return pTexture;
}


 bool MxAbstractAtlas::isLoaded() const
 {
     return pTexture > 0;
 }

