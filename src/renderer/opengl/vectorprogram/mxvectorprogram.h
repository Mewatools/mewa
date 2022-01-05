/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
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

    void init( MxRenderer *renderer );
    void compile();

    virtual void setMatrix( const MxMatrix &matrix );

    void draw( MxVectorDraw &stream );


    static MxGpuProgram::VaoFormat getVaoFormat();
    virtual MxGpuProgram::VaoFormat vaoFormat();
    virtual void enableAttributes();
    virtual void disableAttributes();

private:
    MxMatrix pModelview;

    // gl
    GLint mVertexAttrib;
    GLint mUvAttrib;
    GLint mColorAttrib;
    GLint matrixUniform1;
};


#endif
