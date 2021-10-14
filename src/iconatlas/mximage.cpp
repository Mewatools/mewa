/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mximage.h"



MxImage::MxImage()
{
    pImageData.data = NULL;
}

MxImage::~MxImage()
{
    if( pImageData.data ) {
        free( pImageData.data );
    }
}

void MxImage::alloc( const MxVector2I &size )
{
    Q_ASSERT( NULL == pImageData.data );

    int numOfBytes = size.width() * size.height() * 4;
    pImageData.data = (unsigned char*)malloc(numOfBytes);
    pImageData.size = size;
}

void MxImage::fillWithBlack()
{
    Q_ASSERT( pImageData.data != NULL );
    int numOfBytes = byteCount();
    memset(pImageData.data, 0x0, numOfBytes);
}

/*!
Returns a pointer to the first pixel data. This is equivalent to
scanLine(0).
*/
unsigned char *MxImage::bits()
{
    return pImageData.data;
}


 const MxVector2I& MxImage::size() const
 {
return pImageData.size;
 }

/*!
    Returns the number of bytes occupied by the image data.

    \sa bits()
*/
int MxImage::byteCount() const
{
    Q_ASSERT( pImageData.data != NULL );
    Q_ASSERT( pImageData.size.width() > 0 );
    Q_ASSERT( pImageData.size.height() > 0 );
    return ( pImageData.size.width() * pImageData.size.height() * 4 );
}

unsigned char* MxImage::scanLine( int i ) const
{
    const int bpp = 4;
    return pImageData.data + (i * pImageData.size.width() * bpp);
}


void MxImage::pasteBitmapAtPos( const MxVector2I &pos, const unsigned char *imageBits, int imageWidth, int imageHeight )
{
    Q_ASSERT( pImageData.data != NULL );
    Q_ASSERT( pImageData.size.width() >= (pos.x() + imageWidth) );
    Q_ASSERT( pImageData.size.height() >= (pos.y() + imageHeight) );

    const int bytesPerPixel = 4;

    int bytesPerRow = imageWidth * bytesPerPixel;
    int startY = pos.y();
    int endY = startY + imageHeight;
    int srcY = imageHeight-1; // flip vertically because of OpenGL
    for( int y = startY; y < endY; ++y, srcY--)
    {
        const unsigned char *src = imageBits + (srcY * imageWidth * bytesPerPixel);//image.scanLine(srcY);
        unsigned char *dst = scanLine(y);
        dst += pos.x();
        memcpy(dst, src, bytesPerRow);
    }
}



/*!
\class MxImage
\brief Holds an image in a format supported by OpenGL

  Currently, its API only supports pasting images into other images at specific positions.
  Used to create atlases.

  MxImage pixel data is stored in the OpenGL friendly format RGBA unsigned char.

*/
