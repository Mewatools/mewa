/****************************************************************************
** Copyright (C) 2021-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxtexture.h"
#include "mxrenderer.h"
#include "mxdebug.h"

#include<d3d12.h>



MxTexture::MxTexture()
{
	pTexBuffer = NULL;
}

MxTexture::~MxTexture()
{}


void MxTexture::init( MxRenderer *renderer, int imgWidth, int imgHeight)
{
	// init D3D12_HEAP_PROPERTIES
	pHeapProp = {};
	pHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	pHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	pHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	pHeapProp.CreationNodeMask = 0;
	pHeapProp.VisibleNodeMask = 0;

	// init D3D12_RESOURCE_DESC
	pResDesc = {};
	pResDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	pResDesc.Width = imgWidth;
	pResDesc.Height = imgHeight;
	pResDesc.DepthOrArraySize = 1;
	pResDesc.SampleDesc.Count = 1;
	pResDesc.SampleDesc.Quality = 0;
	pResDesc.MipLevels = 1;
	pResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	pResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	pResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	// alloc texture memory
	HRESULT result = renderer->pDevice->CreateCommittedResource(
		&pHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&pResDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&pTexBuffer)
	);

	Q_ASSERT(NULL != pTexBuffer);
}


void MxTexture::setPixelData(const char* pixels, const MxVector2I& size, MxTexture::PixelFormat format )
{
	Q_ASSERT( MxTexture::UChar4 == format );
	int bpp = 4 * sizeof(unsigned char);

	Q_ASSERT( NULL != pTexBuffer);
	HRESULT  result = pTexBuffer->WriteToSubresource(0,
		nullptr,
		pixels,
		bpp * size.width(),
		bpp * size.width() * size.height()
	);
}

