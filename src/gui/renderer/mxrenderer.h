/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXRENDERER_H
#define MXRENDERER_H

#include "mxopenglfunctions.h"
#include "mxcolorwheelprogram.h"
#include "mxlist.h"
#include "gpubuffer.h"

class MxShaderProgram;


class MxRenderer : public MxOpenGLFunctions
{
public:

    MxRenderer();

    void discardGLResources();

    void checkGLError(const char *fileName, int lineNumber);

    void setProgram( MxShaderProgram *effect );

    // shader programs
    MxColorWheelProgram * colorWheelProgram();

    GpuBuffer* newGpuBuffer( MxShaderProgram::VaoFormat format );
     void clearGpuBuffers();

    typedef MxList<GpuBuffer, MxClassInitializer<GpuBuffer> > VboList;





    VboList pVboList;

protected:
    MxColorWheelProgram pColorWheelEffect;

    unsigned int pCurrShaderProgram;


};

#endif
