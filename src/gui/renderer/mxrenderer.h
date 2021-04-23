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
#include "gpubuffer.h"

class MxShaderProgram;


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

    void setProgram( MxShaderProgram *effect );

    // shader programs
    MxColorWheelProgram * colorWheelProgram();
    MxVectorProgram * setVectorProgram();
    void setBlending( Blending blend );
    GpuBuffer* newGpuBuffer( MxShaderProgram::VaoFormat format );
     void clearGpuBuffers();

    typedef MxList<GpuBuffer, MxClassInitializer<GpuBuffer> > VboList;

    VboList pVboList;
    MxMatrix pScreenProjectionMatrix; // ortho view matrix

protected:
    MxColorWheelProgram pColorWheelEffect;
    MxVectorProgram pVectorProgram;

    unsigned int pCurrShaderProgram;
    MxVector2I pScreenSize;
    Blending pCurrBlend;
};

#endif
