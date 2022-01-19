/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxvectorprogram.h"
#include "mxdebug.h"
#include "mxrenderer.h"

#include<d3dcompiler.h>



MxVectorProgram::MxVectorProgram()
{
	pVertexShader = nullptr;
	pPixelShader = nullptr;

	pInputLayout[0] =
	{ "POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	pInputLayout[1] =
	{ "COLOR",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };

	pInputLayout[2] =
	{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
	
}



void MxVectorProgram::enable(D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipeline)
{
	pipeline->VS.pShaderBytecode = pVertexShader->GetBufferPointer();
	pipeline->VS.BytecodeLength = pVertexShader->GetBufferSize();
	pipeline->PS.pShaderBytecode = pPixelShader->GetBufferPointer();
	pipeline->PS.BytecodeLength = pPixelShader->GetBufferSize();

	pipeline->InputLayout.pInputElementDescs = pInputLayout;
	Q_ASSERT(3 == _countof(pInputLayout));
	pRenderer->pPipeline.InputLayout.NumElements = _countof(pInputLayout);
}

void MxVectorProgram::draw( const MxVectorDraw& stream , const MxMatrix* matrix)
{

}



bool MxVectorProgram::compile()
{
	Q_ASSERT(NULL != pRenderer);

	const wchar_t* vtxShaderFilename = L"VectorVertexShader.hlsl";
	HRESULT result = D3DCompileFromFile(vtxShaderFilename,
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"BasicVS", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &pVertexShader, &(pRenderer->pErrorBlob));

	if (FAILED(result)) {
		if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)) {
			//::OutputDebugStringA("MxIconProgram VS shader file not found");
			qDebug("MxVectorProgram VS shader file not found");
		}
		else {
			std::string errstr;
			errstr.resize(pRenderer->pErrorBlob->GetBufferSize());
			std::copy_n((char*)pRenderer->pErrorBlob->GetBufferPointer(), pRenderer->pErrorBlob->GetBufferSize(), errstr.begin());
			errstr += "\n";
			qDebug(errstr.c_str());
		}
		return false;
	}
	result = D3DCompileFromFile(L"VectorPixelShader.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"BasicPS", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &pPixelShader, &(pRenderer->pErrorBlob));
	if (FAILED(result)) {
		if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)) {
			//::OutputDebugStringA("MxIconProgram PS shader file not found");
			qDebug("MxVectorProgram PS shader file not found");
		}
		else {
			std::string errstr;
			errstr.resize(pRenderer->pErrorBlob->GetBufferSize());
			std::copy_n((char*)pRenderer->pErrorBlob->GetBufferPointer(), pRenderer->pErrorBlob->GetBufferSize(), errstr.begin());
			errstr += "\n";
			qDebug(errstr.c_str());
		}
		return false;
	}
	return true;
}
