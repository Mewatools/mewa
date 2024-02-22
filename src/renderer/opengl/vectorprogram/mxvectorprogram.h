/****************************************************************************
** Copyright (C) 2020-2024 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXVECTORPROGRAM_H
#define MXVECTORPROGRAM_H

#include "mxgpuprogram.h"
#include "mxmatrix.h"
#include "mxvectordraw.h"


class MxVectorProgram : public MxGpuProgram
{
public:

    MxVectorProgram();
    virtual ~MxVectorProgram();

    virtual bool initialize( MxRenderer *renderer );
    void compile();

    void draw( MxVectorDraw &stream, const MxMatrix *matrix );

    virtual MxGpuProgram::VaoFormat vaoFormat();
    virtual void enableAttributes();

private:
    MxMatrix pModelview;

    GLint mVertexAttrib;
    GLint mUvAttrib;
    GLint mColorAttrib;
    GLint matrixUniform1;
};


#endif
