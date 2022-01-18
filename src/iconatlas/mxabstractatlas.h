/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXABSTRACTATLAS_H
#define MXABSTRACTATLAS_H

#include "mxrect.h"
#include "mxtexture.h"


class MxAbstractAtlas
{
public:
    MxAbstractAtlas();
    virtual ~MxAbstractAtlas();


    //! Icon rectangle in texture coordinates
    virtual const MxRectF& iconRect( int imageName ) const = 0;
    //! icon size in pixel coordinates
    virtual const MxVector2I & iconSize( int imageName ) const = 0;
    //! The size of the atlas image
    virtual const MxVector2I& textureSize() const = 0;


    void getPixelSize( MxVector2F *pixelSize ) const;
    
   MxTexture* texture();
     bool isLoaded() const;


protected:
    MxTexture pTexture;


};

#endif
