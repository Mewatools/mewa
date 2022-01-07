/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXICONPROGRAM_H
#define MXICONPROGRAM_H


#include "mxgpuprogram.h"


class MxMatrix;
class MxIconDraw;
class MxRenderer;


// \TODO
class MxIconProgram : public MxGpuProgram
{
public:

    enum ColorFilter {
        IdentityFilter,
        LightGrayFilter,
        BlueFilter,
        DarkGrayFilter,

    };


    MxIconProgram();
    virtual ~MxIconProgram();

   
    virtual void setToPipeline(D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipeline);

    void setModelViewMatrix(const MxMatrix* matrix );
    void setColorFilter(const ColorFilter filter );
    void draw(MxIconDraw& rectsArray );



};


#endif
