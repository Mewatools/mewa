/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxrenderer.h"
#include "mxgpuprogram.h"
#include "mxdebug.h"

#include <d3d12.h>

// \TODO remove std dependencies
#include <vector>
#include <string>
#include <stdlib.h>





MxRenderer::MxRenderer()
{
	pEnableSRGB = false;
	pFirstTime = true;
	pPipelineChanged = true;
	pCurrProgram = NULL;
	pRootSignatureChanged = true;
    pCurrInputTextureFlags = 0;


	pDevice = nullptr;
	pDxgiFactory = nullptr;
	pCmdAllocator = nullptr;
	pCmdList = nullptr;
	pCmdQueue = nullptr;
	pSwapchain = nullptr;
	pRtvHeaps = nullptr;

	pErrorBlob = nullptr;
	
	pPipeline = {};
	pPipeline.pRootSignature = nullptr;

	pRootSignature = nullptr;

	pPipelinestate = nullptr;
}

MxRenderer::~MxRenderer()
{}

void MxRenderer::initialize()
{

}

void MxRenderer::init(HWND hwnd, unsigned int windowWidth, unsigned int windowHeight )
{
	Q_ASSERT( NULL == pDevice );

	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	HRESULT result = CreateDXGIFactory1(IID_PPV_ARGS(&pDxgiFactory));
	// \TODO avoid heap alloc and use a stack array (MxStack)
	std::vector <IDXGIAdapter*> adapters;
	IDXGIAdapter* tmpAdapter = nullptr;
	for (int i = 0; pDxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; ++i) {
		adapters.push_back(tmpAdapter);
	}
	for (auto adpt : adapters) {
		DXGI_ADAPTER_DESC adesc = {};
		adpt->GetDesc(&adesc);
		std::wstring strDesc = adesc.Description;
		if (strDesc.find(L"NVIDIA") != std::string::npos) {
			tmpAdapter = adpt;
			break;
		}
	}

	D3D_FEATURE_LEVEL featureLevel;
	for (auto l : levels) {
		if (D3D12CreateDevice(tmpAdapter, l, IID_PPV_ARGS(&pDevice)) == S_OK) {
			featureLevel = l;
			break;
		}
	}





    result = pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pCmdAllocator));
	result = pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pCmdAllocator, nullptr, IID_PPV_ARGS(&pCmdList));

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	result = pDevice->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&pCmdQueue));

	DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
	swapchainDesc.Width = windowWidth;
	swapchainDesc.Height = windowHeight;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.Stereo = false;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.SampleDesc.Quality = 0;
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapchainDesc.BufferCount = 2;
	swapchainDesc.Scaling = DXGI_SCALING_NONE;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	result = pDxgiFactory->CreateSwapChainForHwnd(pCmdQueue,
		hwnd,
		&swapchainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)&pSwapchain);

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = 2;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	//ID3D12DescriptorHeap* rtvHeaps = nullptr;
	result = pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&pRtvHeaps));
	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = pSwapchain->GetDesc(&swcDesc);
	//std::vector<ID3D12Resource*> _backBuffers(swcDesc.BufferCount);
	Q_ASSERT(swcDesc.BufferCount == 2);
	

	D3D12_CPU_DESCRIPTOR_HANDLE handle = pRtvHeaps->GetCPUDescriptorHandleForHeapStart();

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	if (pEnableSRGB) {
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	}
	else {
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	}
	
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;


	for (size_t i = 0; i < swcDesc.BufferCount; ++i) {
		result = pSwapchain->GetBuffer(static_cast<UINT>(i), IID_PPV_ARGS(&pBackBuffers[i]));
		pDevice->CreateRenderTargetView(pBackBuffers[i], &rtvDesc, handle);
		handle.ptr += pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}


}


void MxRenderer::setViewport( int x, int y, unsigned int width, unsigned int height )
{
	D3D12_VIEWPORT viewport = {};
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.TopLeftX = (float)x;
	viewport.TopLeftY = (float)y;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;

	pCmdList->RSSetViewports(1, &viewport);
}

void MxRenderer::setScissor(const MxVector2I& pos, const MxVector2I& size)
{
	D3D12_RECT scissorrect = {};
	scissorrect.top = pos.x();
	scissorrect.left = pos.y();
	scissorrect.right = scissorrect.left + size.width();
	scissorrect.bottom = scissorrect.top + size.height();
	
	pCmdList->RSSetScissorRects(1, &scissorrect);
}

void MxRenderer::setBlending(MxRenderer::Blending blend)
{

}

void MxRenderer::enableDepthTest(bool enable)
{

}

/*MxTexture* MxRenderer::newTexture(const MxVector2I& size, MxTexture::PixelFormat format)
{
	Q_ASSERT(NULL == pTextures[0].pTexBuffer);

	pTextures[0].init(this, size.width(), size.height());
	Q_ASSERT(NULL != pTextures[0].pTexBuffer);
	return &(pTextures[0]);
}*/

MxGpuArray* MxRenderer::getBuffer( UINT64 length)
{
	// reuse buffers, is this correct??

	for (int i = 0; i < pBufferViews.size(); ++i)
	{
	    MxGpuArray& reuseArray = pBufferViews[i];
		if ( (reuseArray.isTaken() == false)
			&& (reuseArray.size() >= length) )
		{
			reuseArray.pTaken = true;
			return &reuseArray;
		}
	}

    // create new
	MxGpuArray *newArray = pBufferViews.appendAndGet();
	allocResource(newArray,  length);
	newArray->pTaken = true;
	return newArray;
}

void MxRenderer::setProgram(MxGpuProgram* program)
{
	if (pCurrProgram != program) {
		pCurrProgram = program;
		pPipelineChanged = true;
	}
}

void MxRenderer::setTexturesParameters(unsigned int flags)
{
	if (pCurrInputTextureFlags != flags) {
		pCurrInputTextureFlags = flags;
		pRootSignatureChanged = true;
		// changing the root signature is expensive, it triggers pipeline change
		pPipelineChanged = true;
	}
}




void MxRenderer::bindTexture(MxTexture* texture, unsigned char parameters, int inputIndex)
{
	Q_ASSERT(inputIndex == 0);
	pBoundTextures[inputIndex] = texture;
}

void MxRenderer::checkGLError(const char* fileName, int line)
{

}

void MxRenderer::renderBegin()
{

}

void MxRenderer::renderEnd()
{
	for (int i = 0; i < MxRenderer::MaxBoundTextures; ++i) {
		pBoundTextures[i] = NULL;
	}
}




void MxRenderer::setupRoot()
{
	Q_ASSERT(NULL != pDevice);

	if (pRootSignatureChanged)
	{

		pRootSignatureChanged = false;

		
		Q_ASSERT( pPipelineChanged == true );

		// \TODO can pRootSignature be overwritten or needs to be deleted ??
		Q_ASSERT(pRootSignature == NULL);

		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;









		D3D12_DESCRIPTOR_RANGE descTblRange = {};
		descTblRange.NumDescriptors = 1;
		descTblRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descTblRange.BaseShaderRegister = 0;
		descTblRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


		// Matrix uniform
		D3D12_ROOT_PARAMETER rootparam[2]; // = {};
		rootparam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		rootparam[0].Constants.Num32BitValues = 16; // matrix
		rootparam[0].Constants.ShaderRegister = 0; //b0
		rootparam[0].Constants.RegisterSpace = 0;
		rootparam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

		rootparam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootparam[1].DescriptorTable.pDescriptorRanges = &descTblRange;
		rootparam[1].DescriptorTable.NumDescriptorRanges = 1;
		rootparam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		rootSignatureDesc.pParameters = rootparam;
		rootSignatureDesc.NumParameters = 2;



		D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		samplerDesc.MinLOD = 0.0f;
		samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		rootSignatureDesc.pStaticSamplers = &samplerDesc;
		rootSignatureDesc.NumStaticSamplers = 1;

		ID3DBlob* rootSigBlob = nullptr;
		HRESULT result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &pErrorBlob);
		result = pDevice->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pRootSignature));
		rootSigBlob->Release();
	}
}



void MxRenderer::prepareToDraw()
{
	setupRoot();

	bool changed = setupPipeline();
	if (changed) {
		HRESULT result = pDevice->CreateGraphicsPipelineState(&pPipeline, IID_PPV_ARGS(&pPipelinestate));
	}

	pCmdList->SetGraphicsRootSignature(pRootSignature);
	pCmdList->SetPipelineState(pPipelinestate);


	// descriptorHeaps are called after allocating all textures
	pCmdList->SetDescriptorHeaps(1, &(pBoundTextures[0]->pTexDescHeap));
	pCmdList->SetGraphicsRootDescriptorTable(1, pBoundTextures[0]->pTexDescHeap->GetGPUDescriptorHandleForHeapStart());


}

void MxRenderer::allocResource(MxGpuArray *newArray, UINT64 length)
{

	D3D12_HEAP_PROPERTIES heapprop = {};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resdesc = {};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = length;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.Format = DXGI_FORMAT_UNKNOWN;
	resdesc.SampleDesc.Count = 1;
	resdesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT result = pDevice->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&(newArray->pBuffer)));

	Q_ASSERT(NULL != newArray->pBuffer);
}

void MxRenderer::releaseGpuArrays()
{
	for (int i = 0; i < pBufferViews.size(); ++i)
	{
		MxGpuArray& reuseArray = pBufferViews[i];
		reuseArray.pTaken = false;
	}
}

bool MxRenderer::setupPipeline()
{
	if (pPipelineChanged)
	{
		pPipelineChanged = false;
		
		Q_ASSERT(NULL != pCurrProgram);

		pCurrProgram->enable(&pPipeline);




		pPipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		pPipeline.BlendState.AlphaToCoverageEnable = false;
		pPipeline.BlendState.IndependentBlendEnable = false;

		D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};

		renderTargetBlendDesc.BlendEnable = false;
		renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		renderTargetBlendDesc.LogicOpEnable = false;

		pPipeline.BlendState.RenderTarget[0] = renderTargetBlendDesc;

		pPipeline.RasterizerState.MultisampleEnable = false;
		pPipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		pPipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		pPipeline.RasterizerState.DepthClipEnable = true;

		pPipeline.RasterizerState.FrontCounterClockwise = false;
		pPipeline.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		pPipeline.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		pPipeline.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		pPipeline.RasterizerState.AntialiasedLineEnable = false;
		pPipeline.RasterizerState.ForcedSampleCount = 0;
		pPipeline.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;


		pPipeline.DepthStencilState.DepthEnable = false;
		pPipeline.DepthStencilState.StencilEnable = false;



		pPipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		pPipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		pPipeline.NumRenderTargets = 1;
		if (pEnableSRGB) {
			pPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		}
		else {
			pPipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		}

		pPipeline.SampleDesc.Count = 1;
		pPipeline.SampleDesc.Quality = 0;

		Q_ASSERT(pRootSignature != NULL);
		pPipeline.pRootSignature = pRootSignature;
		return true;
	}
	return false;
}

