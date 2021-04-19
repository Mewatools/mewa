/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "gpuvbo.h"
#include "mxdebug.h"
#include "mxopengl.h"
#include "mxrenderer.h"

/*!
 * \brief Holds an OpenGL VBO, nothing else
 */
GpuVbo::GpuVbo()
{
    pVbo = 0;
    pVboSize = 0;
#ifndef QX_OPENGL_ES_2_0
    pVaoObject = 0;
#endif
    pFormat = MxShaderProgram::Unknown;
}


void GpuVbo::uploadToVbo( MxRenderer *renderer, char *data, int size )
{

    if ( pVbo == 0 )
    {
        renderer->glGenBuffers( 1,&(pVbo) );
        Q_ASSERT(pVbo);
    }

    // make sure the buffer is bound, don't perform any checks because size may be zero
    renderer->glBindBuffer(GL_ARRAY_BUFFER, pVbo);

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

void GpuVbo::upload( MxRenderer *renderer, char *data, int size, GLenum target, GLenum usage )
{

    if ( pVbo == 0 )
    {
        renderer->glGenBuffers( 1,&(pVbo) );
        Q_ASSERT(pVbo);
    }

    // make sure the buffer is bound, don't perform any checks because size may be zero
    renderer->glBindBuffer(target, pVbo);

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

    renderer->checkGLError(__FILE__, __LINE__);
}

void GpuVbo::deleteGL( MxRenderer *renderer )
{
    // delete VBOs first
    renderer->glDeleteBuffers(1, &pVbo);

    // delete VAO
    renderer->glDeleteVertexArrays(1, &pVaoObject);

    pVbo = 0;
    pVboSize = 0;
    pVaoObject = 0;
    pFormat = MxShaderProgram::Unknown;
}
