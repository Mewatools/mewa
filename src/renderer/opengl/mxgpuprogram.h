/****************************************************************************
** Copyright (C) 2020-2024 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXGPUPROGRAM_H
#define MXGPUPROGRAM_H

#include "mxopengl.h"


class MxRenderer;
class MxGpuArray;


class MxGpuProgram
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
        Float_2_4_2,
        Float2_UChar4_Float2,
        Custom
    };


    MxGpuProgram( MxRenderer *renderer = nullptr );
    virtual ~MxGpuProgram();

    /*!
     * \brief initialize compiles the shader
     *
     * Compiles the shaders. This function is called by MxRenderer::setProgram() when the program is set.
     * This function can be called by the application to compile the shader in advance.
     * In the implementation the data member mProgramId should be set with the OpenGL program id.
     */
    virtual bool initialize( MxRenderer * ){ return false; }

    /*! Check if initialize() has been called. */
    bool isInitialized() const;

    virtual void enable();

    virtual VaoFormat vaoFormat() { return Unknown; } // \TODO delete this member

    virtual void enableAttributes(){}


    void enableVao(MxGpuArray *buffer );



    GLuint programId() const { return mProgramId; }

protected:
    GLuint mProgramId;
    MxRenderer *pRenderer;
};

#endif
