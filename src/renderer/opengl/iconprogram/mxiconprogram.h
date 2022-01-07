/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
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

    void setModelViewMatrix( const MxMatrix *matrix );


    enum ColorFilter {
        IdentityFilter,
        LightGrayFilter,
        BlueFilter,
        DarkGrayFilter,

    };

    void setColorFilter(const ColorFilter filter );
    void draw(MxIconDraw &rectsArray );

    virtual VaoFormat vaoFormat();
    virtual void enableAttributes();



public:
    void updateUniformValues();



    enum Update
    {
        UpdateMatrix = 0x00000001,
        UpdateAlpha = 0x00000002,
        UpdateAll = 0x7FFFFFFF
    };


    ColorFilter pColorFilter;

    int pUpdates;
public:
    // gl
    GLint vertexAttr2;
    GLint texCoordAttr2;
    GLint matrixUniform2;
    GLint pColorUniform;
    GLint textureUniform2;

};


#endif
