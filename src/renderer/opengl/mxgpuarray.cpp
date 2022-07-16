/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxgpuarray.h"
#include "mxdebug.h"
#include "mxopengl.h"
#include "mxrenderer.h"

/*!
 * \brief Holds an OpenGL VBO, nothing else
 */
MxGpuArray::MxGpuArray()
{
    pVboId = 0;
    pVboSize = 0;
    pVaoObject = 0;
    pFormat = MxGpuProgram::Unknown;
}


void MxGpuArray::uploadToVbo(MxRenderer *renderer, const char *data, unsigned int size )
{

    if ( pVboId == 0 )
    {
        renderer->glGenBuffers( 1,&(pVboId) );
        Q_ASSERT(pVboId);
    }

    // make sure the buffer is bound, don't perform any checks because size may be zero
    renderer->glBindBuffer(GL_ARRAY_BUFFER, pVboId);

    // if the buffer has already been created, just update the data providing it fits
    if ( pVboSize > 0 )
    {
        // if the data will fit in the existing buffer, just update it
        if( size <= pVboSize )
        {
            renderer->glBufferSubData(GL_ARRAY_BUFFER,0, size, data );
        }
        else
        {
            // create a new buffer of the larger size,
            // gl should automatically deallocate the old buffer
            renderer->glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW); // \TODO use GL_STREAM_DRAW
            pVboSize = size;
        }
    }
    else
    {
        // create the buffer
        renderer->glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW); // GL_DYNAMIC_DRAW
        pVboSize = size;
    }
}

void MxGpuArray::upload( MxRenderer *renderer, char *data, int size, GLenum target, GLenum usage )
{

    if ( pVboId == 0 )
    {
        renderer->glGenBuffers( 1,&(pVboId) );
        Q_ASSERT(pVboId);
    }

    // make sure the buffer is bound, don't perform any checks because size may be zero
    renderer->glBindBuffer(target, pVboId);

    // if the buffer has already been created, just update the data providing it fits
    if ( pVboSize > 0 )
    {
        // if the data will fit in the existing buffer, just update it
        if( size <= pVboSize )
        {
            renderer->glBufferSubData(target,0, size, data );
        }
        else
        {
            // create a new buffer of the larger size,
            // gl should automatically deallocate the old buffer
            renderer->glBufferData(target, size, data, usage); // \TODO use GL_STREAM_DRAW
            pVboSize = size;
        }
    }
    else
    {
        // create the buffer
        renderer->glBufferData(target, size, data, usage); // GL_DYNAMIC_DRAW
        pVboSize = size;
    }
}

void MxGpuArray::deleteGL( MxRenderer *renderer )
{
    // delete VBOs first
    renderer->glDeleteBuffers(1, &pVboId);

    // delete VAO
    renderer->glDeleteVertexArrays(1, &pVaoObject);

    pVboId = 0;
    pVboSize = 0;
    pVaoObject = 0;
    pFormat = MxGpuProgram::Unknown;
}
