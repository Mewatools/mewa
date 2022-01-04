/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXRENDERER_H
#define MXRENDERER_H

#include "mxopenglfunctions.h"
#include "mxcolorwheelprogram.h"
#include "mxvectorprogram.h"
#include "mxlist.h"
#include "mxiconprogram.h"
#include "mxbuffer.h"
#include "mxgpuarray.h"


class MxGpuProgram;
class MxAbstractAtlas;


class MxRenderer : public MxOpenGLFunctions
{
public:

    enum Blending {
        NoBlending, // default
        BlendingText,
        BlendingImages,
        CustomBlending
    };

    MxRenderer();

    void discardGLResources();

    virtual void initializeGL();

    void setWindowSize( int width, int height );
    MxVector2I windowSize() const;


    void checkGLError(const char *fileName, int lineNumber);

    void setProgram( MxGpuProgram *effect );

    // shader programs
    MxColorWheelProgram * colorWheelProgram();
    MxVectorProgram * setVectorProgram();
    MxIconProgram * setIconProgram();

    void setBlending( Blending blend );
     void enableDepthTest( bool enable );
     void bindTextureGL(GLuint textureId , GLuint activeSlot = 0 );

    MxGpuArray * uploadToGpu(MxGpuProgram::VaoFormat format, const char *data , unsigned int size);
    // \TODO make it private
    MxGpuArray *newGpuArray( MxGpuProgram::VaoFormat format, unsigned int size );
    //! The returned buffer is automatically deleted after render.
    MxBuffer *getTemporaryBuffer( int sizeEstimate = 1024 );
    void recycleALl();



    struct ReusableVbo
       {
           MxGpuArray gpuArray;
           bool inUse;
       };
    MxList<ReusableVbo, MxClassInitializer<ReusableVbo>, 32> pReusableVbos;

    struct ReusableBuffer
       {
           MxBuffer buffer;
           bool inUse;
       };
     // \TODO fine tune list allocation
    MxList<ReusableBuffer, MxClassInitializer<ReusableBuffer>, 32> pReusableMem;

    MxAbstractAtlas *pIconAtlas;
    MxMatrix pScreenProjectionMatrix; // ortho view matrix

protected:
    MxColorWheelProgram pColorWheelEffect;
    MxVectorProgram pVectorProgram;
    MxIconProgram pIconProgram;

    unsigned int pCurrShaderProgram;
    MxVector2I pScreenSize;
    Blending pCurrBlend;
    bool pDepthTestEnabled;
    GLuint pCurrentTexture;
    GLuint pCurrentActiveTextureSlot;
};

#endif
