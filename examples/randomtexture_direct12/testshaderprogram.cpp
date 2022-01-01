/****************************************************************************
** Copyright (C) 2021-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "testshaderprogram.h"
#include "mxdebug.h"
#include "mxrenderer.h"
#include "mxtexture.h"

#include<d3dcompiler.h>


#include<vector>
#include<string>



TestShaderProgram::TestShaderProgram()
{
    pVertexShader = nullptr;
    pPixelShader = nullptr;

	pInputLayout[0] =
	{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
   
	pInputLayout[1] = 
	{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
    
}


TestShaderProgram::~TestShaderProgram()
{
}



bool TestShaderProgram::compile()
{
    Q_ASSERT( NULL != pRenderer );
  


	
	HRESULT result = D3DCompileFromFile(L"BasicVertexShader.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"BasicVS", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &pVertexShader, &(pRenderer->pErrorBlob) );

	if (FAILED(result)) {
		if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)) {
			//::OutputDebugStringA("TestShaderProgram VS shader file not found");
			qDebug("TestShaderProgram VS shader file not found");
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
	result = D3DCompileFromFile(L"BasicPixelShader.hlsl",
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"BasicPS", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &pPixelShader, &(pRenderer->pErrorBlob));
	if (FAILED(result)) {
		if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND)) {
			//::OutputDebugStringA("TestShaderProgram PS shader file not found");
			qDebug("TestShaderProgram PS shader file not found");
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


void TestShaderProgram::setToPipeline( D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipeline )
{
	

	pipeline->VS.pShaderBytecode = pVertexShader->GetBufferPointer();
	pipeline->VS.BytecodeLength = pVertexShader->GetBufferSize();
	pipeline->PS.pShaderBytecode = pPixelShader->GetBufferPointer();
	pipeline->PS.BytecodeLength = pPixelShader->GetBufferSize();

	pipeline->InputLayout.pInputElementDescs = pInputLayout;
	Q_ASSERT(2 == _countof(pInputLayout));
	pRenderer->pPipeline.InputLayout.NumElements = _countof(pInputLayout);
}


void TestShaderProgram::setInputTexture(MxTexture* texture)
{
	// changing descriptor heaps is a heavy operation 
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;
	descHeapDesc.NumDescriptors = 1;
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	Q_ASSERT( NULL != pRenderer );
	HRESULT result = pRenderer->pDevice->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&(pRenderer->pTexDescHeap)));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	// set texture at t0
	pRenderer->pDevice->CreateShaderResourceView(texture->pTexBuffer,
		&srvDesc,
		pRenderer->pTexDescHeap->GetCPUDescriptorHandleForHeapStart()
	);
}

void TestShaderProgram::draw(TestShaderProgram::Vertex *vtx, UINT vtxLength,
	unsigned short *idx, UINT idxLength )
{
	pRenderer->prepareToDraw();

	Vertex vertices[] = {
		{{-0.5f,-0.9f,0.0f},{0.0f,1.0f} },
		{{-0.5f,0.9f,0.0f} ,{0.0f,0.0f}},
		{{0.5f,-0.9f,0.0f} ,{1.0f,1.0f}},
		{{0.5f,0.9f,0.0f} ,{1.0f,0.0f}},
	};

	MxGpuArray * vertArray = pRenderer->getBuffer(vtxLength);
	vertArray->setVertexData((char*)vtx, vtxLength, sizeof(Vertex));

	MxGpuArray* idxArray = pRenderer->getBuffer(idxLength);
	idxArray->setIndexData((char*)idx, idxLength);

	pRenderer->pCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pRenderer->pCmdList->IASetVertexBuffers(0, 1, &(vertArray->pView.vertexBufferView));
	pRenderer->pCmdList->IASetIndexBuffer(&(idxArray->pView.indexBufferView));

	// descriptorHeaps are called after allocating all textures
	pRenderer->pCmdList->SetDescriptorHeaps(1, &(pRenderer->pTexDescHeap));
	pRenderer->pCmdList->SetGraphicsRootDescriptorTable(0, pRenderer->pTexDescHeap->GetGPUDescriptorHandleForHeapStart());


	pRenderer->pCmdList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}


