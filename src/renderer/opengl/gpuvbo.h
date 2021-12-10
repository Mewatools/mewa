/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef GPUVBO_H
#define GPUVBO_H

#include "mxopengl.h"
#include "mxshaderprogram.h"


// \TODO rename to GLBufferObject or GpuBufferObject ?
class GpuVbo
{
public:
    GpuVbo();

    void uploadToVbo(MxRenderer *renderer, char *data, int size );
    void upload( MxRenderer *renderer, char *data, int size, GLenum target = GL_ARRAY_BUFFER, GLenum usage = GL_DYNAMIC_DRAW );
    void deleteGL(MxRenderer *renderer);


    // data members
    unsigned int pVbo;
    unsigned int pVboSize;
    unsigned int pVaoObject;


    // needed to be able for the renderer to reuse buffers
    MxShaderProgram::VaoFormat pFormat;
};



#endif

