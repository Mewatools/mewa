/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxiconatlas.h"
#include "mxmathutils.h"
#include "lodepng.h"

#ifdef MX_DIRECTX12_RENDERER
#include "mxtexture.h"
#endif

MxIconAtlas::MxIconAtlas()
{
    for(int i=0; i<MxThemeIcons::ImageCount; ++i) {
        pImageList[i].iconSize = MxVector2I(0,0);
        pImageList[i].rgba = NULL;
    }
}

const MxRectF& MxIconAtlas::iconRect( int name ) const
{
    const ImageDetails &details = pImageList[name];
    return details.textureRect;
}

const MxVector2I & MxIconAtlas::iconSize( int name ) const
{
    const ImageDetails &details = pImageList[name];
    return details.iconSize;
}

void MxIconAtlas::loadGL( MxRenderer& renderer )
{
    if(  build() )
    {
        MxVector2I texSize = textureSize();
        if( texSize.width() > 0 && texSize.height() > 0 )
        {
            const unsigned char* pixelData = pAtlasImage.bits();
            Q_ASSERT(NULL != pixelData);

/*
            Q_ASSERT( NULL == pTexture);
            pTexture = renderer.newTexture(texSize, MxTexture::UChar4);
            pTexture->setPixelData(pixelData, texSize, MxTexture::UChar4);

            // \TODO allow RepeatWrap
            //renderer.bindTexture(pTexture, (MxTexture::NoFilter | MxTexture::ClampWrap), 0);
            */





            pTexture.create( &renderer, texSize, MxTexture::RGBA8, pixelData );


            renderer.checkGLError(__FILE__, __LINE__);

            // delete icons
            for(int i=0; i<MxThemeIcons::ImageCount; ++i) {
               free( pImageList[i].rgba );
            }

        }
        else
        {
            qDebug( "No icons set, don't load icon atlas" );
        }
    }
}

void MxIconAtlas::setIcon( MxThemeIcons::IconName name, const char *imageFileName )
{
    Q_ASSERT( name >= 0 && name < MxThemeIcons::ImageCount );

    ImageDetails &details = pImageList[name];
    unsigned int outWidth, outHeight;
    unsigned error = lodepng_decode32_file( &(details.rgba), &outWidth, &outHeight, imageFileName);
    if(error) {
        qDebug("decoder error %u: %s", error, lodepng_error_text(error));
    }

    details.iconSize = MxVector2I( outWidth, outHeight );
}

bool MxIconAtlas::build()
{
    // \TODO use a better atlas packing algorithm

    int biggestH = 0;
    int sumW = 0;
    for(int i=0; i<MxThemeIcons::ImageCount; ++i) {
        const MxVector2I &imgSize = pImageList[i].iconSize;
        if( imgSize.height() > biggestH ) {
            biggestH = imgSize.height();
        }

        sumW += imgSize.width();
    }

    if( biggestH == 0 || sumW == 0 ) {
        return false;
    }

    MxVector2I textureSize(
                MxMathUtils::nextPowerOfTwo(sumW)
                , MxMathUtils::nextPowerOfTwo(biggestH) );
    pAtlasImage.alloc(textureSize);
    pAtlasImage.fillWithBlack();

    MxVector2I pastePos(0, 0);
    for(int i=0; i < MxThemeIcons::ImageCount; ++i) {
        ImageDetails &details = pImageList[i];
        unsigned char* iconImg = details.rgba;
        if( iconImg ) {
            const MxVector2I &imgSize = details.iconSize;

            pAtlasImage.pasteImageAtPos( pastePos, iconImg, imgSize.width(), imgSize.height() );

            float texX = (float)pastePos.x() / (float)textureSize.width();
            float texY = (float)pastePos.y() / (float)textureSize.height();
            float texW = (float)imgSize.width() / (float)textureSize.width();
            float texH = (float)imgSize.height() / (float)textureSize.height();
            details.textureRect = MxRectF( texX, texX + texW, texY, texY + texH);

            pastePos[0] += imgSize.width();

        } else {
            details.textureRect = MxRectF(0.0f, 0.0f, 0.0f, 0.0f);
        }
    }

    return true;
}



const MxVector2I& MxIconAtlas::textureSize() const
{
    return pAtlasImage.pImageData.size;
}

