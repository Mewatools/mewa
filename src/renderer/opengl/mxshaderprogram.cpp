/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxdebug.h"
#include "mxshaderprogram.h"
#include "gpuvbo.h"
#include "mxrenderer.h"


MxShaderProgram::MxShaderProgram( MxRenderer *renderer )
    :pRenderer(renderer)
{
    mProgramId = 0;
}

//! Destroys
MxShaderProgram::~MxShaderProgram()
{}

void MxShaderProgram::discardGLResources()
{
    mProgramId = 0;
}

void MxShaderProgram::enable()
{
    Q_ASSERT( 0 != mProgramId );
    pRenderer->glUseProgram(mProgramId);
}


void MxShaderProgram::enableVao( GpuVbo *buffer )
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

    Q_ASSERT( (buffer->pFormat != MxShaderProgram::Unknown) && (buffer->pFormat == vaoFormat()) );

    pRenderer->checkGLError(__FILE__, __LINE__);
}


void MxShaderProgram::disableVao()
{

    // glBindVertexArray(0);

}

