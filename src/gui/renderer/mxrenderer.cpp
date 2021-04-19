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

GpuBuffer *MxRenderer::auxBuffer( MxShaderProgram::VaoFormat format )
{
    // \TODO manage list of vbos for reuse
    pGpuBuffer.pFormat = format;
    pGpuBuffer.pSize = 0;
    return &pGpuBuffer;
}
