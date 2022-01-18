/****************************************************************************
** Copyright (C) 2021-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxtexture.h"
#include "mxrenderer.h"



MxTexture::MxTexture()
{
    mWidth = 0;
    mHeight = 0;
    mTextureWidth = 0;
    mTextureHeight = 0;
    mChannels = 0;
    mTextureId = 0;
    pCurrParameters = 0;
}

MxTexture::~MxTexture()
{}

void MxTexture::create( MxRenderer* renderer, const MxVector2I &size, PixelFormat format, const void *bits )
{
    Q_ASSERT( 0 == mTextureId );
    Q_ASSERT( isNull() );
    renderer->glGenTextures(1, &mTextureId );
     Q_ASSERT( mTextureId > 0 );

    renderer->bindTextureGL(mTextureId);

    mWidth = size.width();
    mHeight = size.height();
    mChannels = 4;
    mTextureWidth = mWidth;
    mTextureHeight = mHeight;

    renderer->glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, mTextureWidth, mTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bits );
}

void MxTexture::discardGLResources()
{
    mTextureWidth = 0;
    mTextureHeight = 0;
    mTextureId = 0;
    mWidth = 0;
    mHeight = 0;
    mChannels = 0;
pCurrParameters = 0;
}

bool MxTexture::isNull() const
{
    return (mWidth == 0 && mHeight == 0);
    //return (0 == mChannels);
}


void MxTexture::setData(MxRenderer *renderer, const unsigned char* pixels, const MxVector2I& size, MxTexture::PixelFormat format )
{    
    mWidth = size.width();
    mHeight = size.height();
    Q_ASSERT( mWidth <= mTextureWidth && mHeight <= mTextureHeight );

    GLenum f = glFormat( format );
    GLenum t = glType( format );
    renderer->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.width(), size.height(), f, t, pixels);

}


void MxTexture::setParameters( MxRenderer *renderer, unsigned char newParameters )
{
    // unsigned char flags = (newParameters & pCurrParameters);
    unsigned char newFilterFlags = newParameters & MxTexture::FilterMask;
    Q_ASSERT( newFilterFlags > 0 );
    unsigned char newWrapFlags = newParameters & MxTexture::WrapMask;
    Q_ASSERT( newWrapFlags > 0 );


    unsigned char hasChanges = (newFilterFlags & (pCurrParameters & MxTexture::FilterMask));
    if( hasChanges == 0 ) { // needs setting a change
        setGLInterpolation( renderer, newFilterFlags );
    }

    hasChanges = (newWrapFlags & (pCurrParameters & MxTexture::WrapMask));
    if( hasChanges == 0 )
    {
        setGLWrap( renderer, newWrapFlags );
    }

}


void MxTexture::setGLInterpolation( MxRenderer *renderer, int filter )
{
    // \TODO glTexParameterf or glTexParameteri
    Q_ASSERT( filter > 0 );
     pCurrParameters &= ~(MxTexture::FilterMask); // reset filter flags
    if( filter & MxTexture::NoFilter )
    {
        pCurrParameters |= MxTexture::NoFilter;
        renderer->glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        renderer->glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else if( filter & MxTexture::LinearFilter )
    {
        pCurrParameters |= MxTexture::LinearFilter;
        renderer->glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        renderer->glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        Q_ASSERT( filter & MxTexture::MipmapLinearFilter );
        pCurrParameters |= MxTexture::MipmapLinearFilter;
        renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        renderer->glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        updateMipmaps( renderer );
    }
}

void MxTexture::setGLWrap( MxRenderer *renderer, int wrap )
{
    Q_ASSERT( wrap > 0 );
    pCurrParameters &= ~(MxTexture::WrapMask); // reset wrap flags
    if( wrap & MxTexture::ClampWrap ) {
        pCurrParameters |= MxTexture::ClampWrap;
        renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    } else {
        Q_ASSERT( wrap & MxTexture::RepeatWrap );
        pCurrParameters |= MxTexture::RepeatWrap;
        renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

}

void MxTexture::updateMipmaps( MxRenderer *renderer )
{
    //unsigned char filter = pCurrParameters & MxTexture::FilterMask;
    //if( filter & MxTexture::MipmapLinearFilter )
    {
        renderer->glGenerateMipmap(GL_TEXTURE_2D);
    }
}


GLenum MxTexture::glFormat( MxTexture::PixelFormat format )
{
    //Q_ASSERT( channels == 4 || channels == 1 );
   // if( channels == 4 )
        return GL_RGBA;

   // return GL_LUMINANCE;
}

GLenum MxTexture::glType( MxTexture::PixelFormat format )
{
   // Q_ASSERT( channels == 4 || channels == 1 );
    return GL_UNSIGNED_BYTE;
}
