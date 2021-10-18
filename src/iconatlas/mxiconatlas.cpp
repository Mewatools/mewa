/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxiconatlas.h"
#include "mxmathutils.h"

MxIconAtlas::MxIconAtlas()
{
    pTexture = 0;

    for(int i=0; i<MxThemeIcons::ImageCount; ++i) {
        pImageList[i].iconSize = MxVector2I(0,0);
        pImageList[i].image = QImage();
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
            Q_ASSERT( 0 == pTexture );
            renderer.glGenTextures(1, &pTexture );
            renderer.glBindTexture( GL_TEXTURE_2D, pTexture );

            renderer.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            renderer.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            renderer.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            renderer.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            const unsigned char *pixelData = pAtlasImage.bits();
            Q_ASSERT( NULL != pixelData );
            renderer.glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texSize[0], texSize[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData );

            renderer.checkGLError(__FILE__, __LINE__);
        }
        else
        {
            qDebug() << "No icons set, don't load icon atlas";
        }
    }
}

bool MxIconAtlas::isLoaded() const
{
    return pTexture > 0;
}

void MxIconAtlas::setIcon( MxThemeIcons::IconName name, const char *imageFileName )
{
    Q_ASSERT( name >= 0 && name < MxThemeIcons::ImageCount );

    QImage iconImg(imageFileName);
    iconImg = iconImg.convertToFormat(QImage::Format_RGBA8888);

    ImageDetails &details = pImageList[name];
    details.image = iconImg;
    details.iconSize = MxVector2I( iconImg.width(), iconImg.height() );
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

        const QImage &iconImg = details.image;
        if( ! iconImg.isNull() ) {
            const QSize &imgSize = iconImg.size();
            Q_ASSERT( iconImg.format() == QImage::Format_RGBA8888 );
            pAtlasImage.pasteBitmapAtPos( pastePos, iconImg.constBits(), iconImg.width(), iconImg.height() );

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

unsigned int MxIconAtlas::texture() const
{
    return pTexture;
}

const MxVector2I& MxIconAtlas::textureSize() const
{
    return pAtlasImage.pImageData.size;
}

