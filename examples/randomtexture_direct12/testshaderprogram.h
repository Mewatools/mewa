/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef TESTSHADERPROGRAM_H
#define TESTSHADERPROGRAM_H

#include "mxgpuprogram.h"

#include<d3d12.h>



class MxTexture;
class MxMatrix;



class TestShaderProgram : public MxGpuProgram
{
public:

	struct Vertex {
		float pos[3];
		float uv[2];
	};

	TestShaderProgram();
	virtual ~TestShaderProgram();


	bool compile();
	virtual void setToPipeline(D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipeline);


	// sets texture to the correct shader input
	void setInputTexture(MxTexture* texture);
	void draw(const MxMatrix* matrix );



private:
	ID3DBlob* pVertexShader;
	ID3DBlob* pPixelShader;
	D3D12_INPUT_ELEMENT_DESC pInputLayout[2]; // inputs POSITION and TEXTURE
};

#endif
