/****************************************************************************
** Copyright (C) 2020-2023 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXRENDERER_H
#define MXRENDERER_H

#include "mxlist.h"
#include "mxgpuarray.h"
#include "mxmatrix.h"
#include "mxopenglfunctions.h"
#include "mxvector.h"
#include "mxmatrix.h"


class MxGpuProgram;
class MxAbstractAtlas;
class MxTexture;


class MxRenderer : public MxOpenGLFunctions
{
public:

    enum Blending {
        NoBlending, // default
        BlendingPremultiplied,
        BlendingImages,
        CustomBlending
    };

    MxRenderer();

    virtual void discardGLResources();

    virtual void initialize();

    virtual void setWindowSize( int width, int height );
    const MxVector2I& windowSize() const;
    const MxMatrix* windowMatrix();


    void checkGLError(const char *fileName, int lineNumber);

    void setProgram( MxGpuProgram *effect );


    // \TODO
    void setViewport(int x, int y, unsigned int width, unsigned int height);
    void setScissor( int x, int y, unsigned int width, unsigned int height );
    void setTexturesParameters( unsigned int flags );

    void setBlending( Blending blend );
     void enableDepthTest( bool enable );
     void bindTextureGL(GLuint textureId , GLuint activeSlot = 0 );
     void bindTexture( MxTexture *texture, unsigned char parameters = 0xFF, int inputIndex = 0 );


    MxGpuArray * uploadToGpu(MxGpuProgram::VaoFormat format, const char *data , unsigned int size);
    // \TODO make it private
    MxGpuArray *newGpuArray( MxGpuProgram::VaoFormat format, unsigned int size );
    void renderBegin();
virtual void renderEnd();





    struct ReusableVbo
       {
           MxGpuArray gpuArray;
           bool inUse;
       };
    MxList<ReusableVbo, MxClassInitializer<ReusableVbo>, 32> pReusableVbos;



protected:
 //! ortho matrix that maps to application window
    MxMatrix pScreenProjectionMatrix;

protected:

    struct Viewport
    {
        int x;
        int y;
        unsigned int width;
        unsigned int height;
    };
    Viewport pViewport;
    
    unsigned int pCurrShaderProgram; // \TODO change to MxGpuProgram*
    MxVector2I pScreenSize;
    Blending pCurrBlend;
    bool pDepthTestEnabled;
    GLuint pCurrentTexture;
    GLuint pCurrentActiveTextureSlot;
};

#endif
