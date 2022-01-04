/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXGPUARRAY_H
#define MXGPUARRAY_H

//#include "mxopengl.h"
#include "mxshaderprogram.h"


/*!
 * \brief The MxGpuArray class holds a graphics vertex buffer
 */
class MxGpuArray
{
public:
    MxGpuArray();

    // \deprecated
    void uploadToVbo(MxRenderer *renderer, const char *data, unsigned int size );
    void upload( MxRenderer *renderer, char *data, int size, GLenum target = GL_ARRAY_BUFFER, GLenum usage = GL_DYNAMIC_DRAW );
    void deleteGL(MxRenderer *renderer);


    // data members
    unsigned int pVboId;
    unsigned int pVboSize;
    unsigned int pVaoObject;


    // needed to be able for the renderer to reuse buffers
    MxShaderProgram::VaoFormat pFormat;
};



#endif

