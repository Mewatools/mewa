/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxrenderer.h"



MxRenderer::MxRenderer()
{
}

void MxRenderer::discardGLResources()
{
    pColorWheelEffect.discardGLResources();
}

void MxRenderer::checkGLError( const char *fileName, int lineNumber )
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        qDebug("[%s line %d] GL Error: %d", fileName, lineNumber, (int)err);
    }
}

void MxRenderer::setProgram( MxShaderProgram *effect )
{
    Q_ASSERT( NULL != effect );
    if( effect->programId() != pCurrShaderProgram )
    {
        effect->enable();
        pCurrShaderProgram = effect->programId();
    }
}

MxColorWheelProgram * MxRenderer::colorWheelProgram()
{
    if( pColorWheelEffect.programId() == 0 )
    {
        pColorWheelEffect.init(this);
        pColorWheelEffect.initializeGL();
    }

    setProgram( &pColorWheelEffect );
    return &pColorWheelEffect;
}

GpuBuffer *MxRenderer::newGpuBuffer( MxShaderProgram::VaoFormat format )
{
    // find first available buffer
    int vboCount = pVboList.size();
    for(int i=0; i<vboCount; ++i)
    {
        GpuBuffer &buffer = pVboList[i];
        Q_ASSERT( buffer.pFormat != MxShaderProgram::Unknown );
        if( buffer.size() == -1 && buffer.pFormat == format )
        {
            buffer.pSize = 0; // set as taken
            return &buffer;
        }
    }

    GpuBuffer *buffer = pVboList.appendAndGet();
    buffer->pFormat = format;
    Q_ASSERT( buffer->size() == 0 );
    return buffer;
}


void MxRenderer::clearGpuBuffers()
{
    int vboCount = pVboList.size();
    for(int i=0; i<vboCount; ++i)
    {
        GpuBuffer &buffer = pVboList[i];
        buffer.pSize = -1;
    }
}

