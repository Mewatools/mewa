/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXVECTORPROGRAM_H
#define MXVECTORPROGRAM_H

#include "mxgpuprogram.h"


#include<d3d12.h>

class MxMatrix;
class MxVectorDraw;
class MxRenderer;



class MxVectorProgram : public MxGpuProgram
{
public:

    MxVectorProgram();

  
    void enable(D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipeline);
    void draw( const MxVectorDraw& stream, const MxMatrix* matrix);


protected:
    virtual bool compile();

    ID3DBlob* pVertexShader;
    ID3DBlob* pPixelShader;
    D3D12_INPUT_ELEMENT_DESC pInputLayout[3];
};


#endif
