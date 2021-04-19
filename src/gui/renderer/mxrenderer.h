/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXRENDERER_H
#define MXRENDERER_H

#include "mxopenglfunctions.h"
#include "mxcolorwheelprogram.h"
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

    // \TODO rename to newGpuBuffer
    GpuBuffer* auxBuffer( MxShaderProgram::VaoFormat format );



protected:
    // \TODO use a container to hold all buffers for re-use
    GpuBuffer pGpuBuffer;

    MxColorWheelProgram pColorWheelEffect;

    unsigned int pCurrShaderProgram;


};

#endif
