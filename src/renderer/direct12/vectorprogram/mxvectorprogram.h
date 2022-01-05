/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXVECTORPROGRAM_H
#define MXVECTORPROGRAM_H

#include "mxgpuprogram.h"


class MxMatrix;
class MxVectorDraw;
class MxRenderer;


// \TODO
class MxVectorProgram : public MxGpuProgram
{
public:

    MxVectorProgram();



  
    void setToPipeline(D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipeline);

    void setMatrix(const MxMatrix& matrix);
    void draw(MxVectorDraw& stream);

};


#endif
