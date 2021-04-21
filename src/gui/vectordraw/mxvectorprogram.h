/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXVECTORPROGRAM_H
#define MXVECTORPROGRAM_H

#include "mxshaderprogram.h"
#include "mxmatrix.h"
#include "mxvectordraw.h"


class MxVectorProgram : public MxShaderProgram
{
public:

    MxVectorProgram();
    virtual ~MxVectorProgram();

    void init( MxRenderer *renderer );
    void initializeGL();

    virtual void setMatrix( const MxMatrix &matrix );

    void draw( MxVectorDraw &stream );


    static MxShaderProgram::VaoFormat getVaoFormat();
    virtual MxShaderProgram::VaoFormat vaoFormat();
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
