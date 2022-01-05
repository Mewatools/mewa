/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxdebug.h"
#include "mxgpuprogram.h"
#include "mxgpuarray.h"
#include "mxrenderer.h"


MxGpuProgram::MxGpuProgram( MxRenderer *renderer )
    :pRenderer(renderer)
{
    mProgramId = 0;
}

//! Destroys
MxGpuProgram::~MxGpuProgram()
{}

bool MxGpuProgram::isInitialized() const
{
    return (nullptr != pRenderer);
}


void MxGpuProgram::enable()
{
    Q_ASSERT( 0 != mProgramId );
    pRenderer->glUseProgram(mProgramId);
}


void MxGpuProgram::enableVao( MxGpuArray *buffer )
{
    if( buffer->pVaoObject == 0 )
    {
        pRenderer->glGenVertexArrays(1, &(buffer->pVaoObject) );
        pRenderer->glBindVertexArray( buffer->pVaoObject );
        Q_ASSERT( buffer->pFormat == vaoFormat() );
        enableAttributes();
    }
    else
    {
        pRenderer->glBindVertexArray( buffer->pVaoObject );
    }

    Q_ASSERT( (buffer->pFormat != MxGpuProgram::Unknown) && (buffer->pFormat == vaoFormat()) );

    pRenderer->checkGLError(__FILE__, __LINE__);
}


void MxGpuProgram::disableVao()
{

    // glBindVertexArray(0);

}

