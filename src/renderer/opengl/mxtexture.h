/****************************************************************************
** Copyright (C) 2021-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXTEXTURE_H
#define MXTEXTURE_H

#include "mxvector.h"
#include "mxopengl.h"


class MxRenderer;


/*! MxTexture abstracts a Gpu texture.
 
 \sa MxRenderer
*/
class MxTexture
{
public:
	enum PixelFormat
	{
        Uninitialized = 0,
        RGBA8, //! 8 bit per pixel, RGBA
        RGBAFloat,
	};

    // texture parameters
    enum FilterParameter {
        NoFilter = 0x1, // default (nearest filter)
        LinearFilter = 0x02,
       // MipmapFilter = 0x04,
        MipmapLinearFilter = 0x08,

        FilterMask = 0xF // reserved for BINARY-AND with filter options
    };

    enum WrapParameter {
        ClampWrap = 0x10,
        RepeatWrap = 0x20,

        WrapMask = 0x30 // reserved for BINARY-AND
    };



    MxTexture();
    virtual ~MxTexture();

    //! \a size must be pot
    void create(MxRenderer* renderer, const MxVector2I &size, PixelFormat format, const void *bits );

    void discardGLResources();
    bool isNull() const;

    //! Set the given pixel data to this texture
    void setData(MxRenderer *renderer, const unsigned char *pixels, const MxVector2I& size, MxTexture::PixelFormat format);


    //! Parameters are the bits defined in FilterParameter and WrapParameter. Its safe to call before every draw
    void setParameters(MxRenderer *renderer, unsigned char newParameters );

public:
    void setGLInterpolation( MxRenderer *renderer, int filter);
    void setGLWrap( MxRenderer *renderer, int wrap );
    void updateMipmaps( MxRenderer *renderer ); // call it everytime the texture content changes
    GLenum glFormat(PixelFormat format ); // format can be RGB, RGBA, and A.
    GLenum glType(PixelFormat format );



    int mWidth;
    int mHeight;
    int mTextureWidth;
    int mTextureHeight;
    int mChannels;
    GLuint mTextureId;
    unsigned char pCurrParameters;
};

#endif

