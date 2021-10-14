/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
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

unsigned int MxAbstractAtlas::texture() const
{
    return pTexture;
}

 bool MxAbstractAtlas::isLoaded() const
 {
     return pTexture > 0;
 }

void MxAbstractAtlas::discardGLResources()
{
    pTexture = 0;
}
