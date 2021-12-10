/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXSHADERPROGRAM_H
#define MXSHADERPROGRAM_H

#include "mxdebug.h"
#include "mxopengl.h"


class MxRenderer;
class MxGpuArray;


class MxShaderProgram
{
public:

    // name VAOs so that they can be reused
    enum VaoFormat
    {
        Unknown = 0,
        Float_2,
        Float_2_2,
        Float_3,
        Float_3_2,
        Float_4,
        Float2_UChar4_Float2
    };


    MxShaderProgram( MxRenderer *renderer = NULL );
    virtual ~MxShaderProgram();

    virtual void discardGLResources();

    virtual void enable();

    virtual VaoFormat vaoFormat() { return Unknown; } // \TODO delete this member

    virtual void enableAttributes(){}
    virtual void disableAttributes(){}


    void enableVao(MxGpuArray *buffer );
    void disableVao();


    GLuint programId() const { return mProgramId; }

protected:
    GLuint mProgramId;
    MxRenderer *pRenderer;
};

#endif
