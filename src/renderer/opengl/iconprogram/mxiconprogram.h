/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXICONPROGRAM_H
#define MXICONPROGRAM_H

#include "mxdebug.h"
#include "mxmatrix.h"
#include "mxvector.h"
#include "mxopengl.h"
#include "mxgpuprogram.h"
#include "mxrect.h"


class MxIconDraw;
//class GpuBuffer;



class MxIconProgram : public MxGpuProgram
{
public:
    MxIconProgram();
    virtual ~MxIconProgram();

    void init( MxRenderer *renderer );
    void compile(); // \TODO make it private




    void draw(MxIconDraw &rectsArray, const MxMatrix *matrix );

    virtual VaoFormat vaoFormat();
    virtual void enableAttributes();


private:
    // gl
    GLint vertexAttr2;
    GLint texCoordAttr2;
    GLint matrixUniform2;
    GLint mColorAttrib;
    GLint textureUniform2;

};


#endif
