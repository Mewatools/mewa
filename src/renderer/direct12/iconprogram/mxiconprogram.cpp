/****************************************************************************
** Copyright (C) 2021-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxiconprogram.h"
#include "mxdebug.h"
#include "mxrenderer.h"
#include "mxtexture.h"
#include "mxmatrix.h"
#include "mxicondraw.h"
#include "mxbuffer.h"

#include<d3dcompiler.h>


#include<vector>
#include<string>



MxIconProgram::MxIconProgram()
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


MxIconProgram::~MxIconProgram()
{
}


void MxIconProgram::setViewMatrix(const MxMatrix* matrix)
{

}

void MxIconProgram::draw( const MxIconDraw& rectsArray, const MxMatrix* matrix )
{
	

	Q_ASSERT(pRenderer->pBoundTextures[0].texture != NULL );

	pRenderer->prepareToDraw();

	pRenderer->connectInputTextures(1);



	// 1 matrix = 16 floats
	pRenderer->pCmdList->SetGraphicsRoot32BitConstants(0, 16, matrix->data(), 0);




#if 0
	
	float x0 = 50.0f;
	float x1 = 500.0f;
	float y0 = 50.0f;
	float y1 = 500.0f;
	const unsigned char color = 128;
	const unsigned char alpha = 255;

	Vertex vertices[] = {
		{{x0, y0}, {255,color,color,alpha}  ,{0.0f,1.0f} },
		{{x0, y1},{255,color,color,alpha}  ,{0.0f,0.0f}},
		{{x1, y0},{255,color,color,alpha} ,{1.0f,1.0f}},

		{{x1, y1},{255,color,color,alpha}  ,{1.0f,0.0f}},
		{{x0, y1},{255,color,color,alpha}  ,{0.0f,0.0f}},
		{{x1, y0},{255,color,color,alpha}  ,{1.0f,1.0f}},
	};
	
	UINT arrayLength = 6 * sizeof(Vertex);
	MxGpuArray* vertArray = pRenderer->getBuffer(arrayLength);
	vertArray->setVertexData((char*)vertices, arrayLength, sizeof(Vertex));

#else
	UINT arrayLength = rectsArray.pArray->size();

	MxGpuArray* vertArray = pRenderer->getBuffer(arrayLength);
	//vertArray->setVertexData((char*)vertices, arrayLength, sizeof(Vertex));
	vertArray->setVertexData((char*)rectsArray.pArray->data(), arrayLength, sizeof(Vertex));
#endif


	//MxGpuArray* idxArray = pRenderer->getBuffer(idxLength);
	//idxArray->setIndexData((char*)idx, idxLength);

	pRenderer->pCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pRenderer->pCmdList->IASetVertexBuffers(0, 1, &(vertArray->pView.vertexBufferView));
	//pRenderer->pCmdList->IASetIndexBuffer(&(idxArray->pView.indexBufferView));





	//pRenderer->pCmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);
	UINT vertexCountPerInstance = 6;
	UINT instanceCount = 1;
		UINT startVertexLocation = 0;
	UINT startInstanceLocation = 0;
	pRenderer->pCmdList->DrawInstanced(vertexCountPerInstance, 
		instanceCount, 
		startVertexLocation, 
		startInstanceLocation);
}




void MxIconProgram::enable(D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipeline)
{
	pipeline->VS.pShaderBytecode = pVertexShader->GetBufferPointer();
	pipeline->VS.BytecodeLength = pVertexShader->GetBufferSize();
	pipeline->PS.pShaderBytecode = pPixelShader->GetBufferPointer();
	pipeline->PS.BytecodeLength = pPixelShader->GetBufferSize();

	pipeline->InputLayout.pInputElementDescs = pInputLayout;
	Q_ASSERT(3 == _countof(pInputLayout));
	pRenderer->pPipeline.InputLayout.NumElements = _countof(pInputLayout);


	
}





bool MxIconProgram::compile()
{
	Q_ASSERT(NULL != pRenderer);



	const wchar_t* vtxShaderFilename = L"hlsl\\IconVertexShader.hlsl";
	HRESULT result = D3DCompileFromFile(vtxShaderFilename,
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"IconVS", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &pVertexShader, &(pRenderer->pErrorBlob));

	if (FAILED(result)) {
		if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)) {
			//::OutputDebugStringA("MxIconProgram VS shader file not found");
			qDebug("MxIconProgram VS shader file not found");
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
	result = D3DCompileFromFile(L"hlsl\\IconPixelShader.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"IconPS", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &pPixelShader, &(pRenderer->pErrorBlob));
	if (FAILED(result)) {
		if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)) {
			//::OutputDebugStringA("MxIconProgram PS shader file not found");
			qDebug("MxIconProgram PS shader file not found");
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


