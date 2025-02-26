/****************************************************************************
** Copyright (C) 2020-2024 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef TRIANGLEPROGRAM_H
#define TRIANGLEPROGRAM_H

#include "mxrenderer.h"



class TriangleProgram : public MxGpuProgram
{
public:
    TriangleProgram();
    virtual ~TriangleProgram();

    virtual bool initialize( MxRenderer *renderer );
    virtual VaoFormat vaoFormat();
    virtual void enableAttributes();

private:
    GLuint loadShader( MxRenderer &renderer, GLenum shaderType, const char* pSource);
    GLuint createProgram( MxRenderer &renderer, const char* pVertexSource, const char* pFragmentSource);


    GLuint mPositionAttrib;

};

#endif // TRIANGLEPROGRAM_H
