/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXICONPROGRAM_H
#define MXICONPROGRAM_H

#include "mxgpuprogram.h"


#include<d3d12.h>


class MxIconDraw;
class MxMatrix;



class MxIconProgram : public MxGpuProgram
{
public:

	struct Vertex {
		float pos[2];
		unsigned char color[4];
		float uv[2];
	};

	MxIconProgram();
	virtual ~MxIconProgram();


	bool compile();
	virtual void setToPipeline(D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipeline);

	void draw( const MxIconDraw& rectsArray, const MxMatrix* matrix );



private:
	ID3DBlob* pVertexShader;
	ID3DBlob* pPixelShader;
	D3D12_INPUT_ELEMENT_DESC pInputLayout[3]; // inputs POSITION , COLOR and TEXTURE
};

#endif
