/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXTEXTURE_H
#define MXTEXTURE_H

#include "mxvector.h"

#include<d3d12.h>

class MxRenderer;


/*! MxTexture abstracts a Gpu texture.
 
 \sa MxRenderer
*/
class MxTexture
{
public:


	// texture parameters
	enum Filter {
		NoFilter = 0x1, // default (nearest filter)
		LinearFilter = 0x02,
		MipmapFilter = 0x04,
		MipmapLinearFilter = 0x08
	};

	enum Wrap {
		ClampWrap = 0x10,
		RepeatWrap = 0x20
	};



	enum PixelFormat
	{
		UChar4, //! 8 bit per pixel, RGBA
		Float4,
	};

	MxTexture();
	~MxTexture();

	void init( MxRenderer* renderer, int imgWidth, int imgHeight );

	//! Set the given pixel data to this texture
	void setPixelData( const unsigned char* pixels, const MxVector2I& size, PixelFormat format);





	D3D12_HEAP_PROPERTIES pHeapProp;
	D3D12_RESOURCE_DESC pResDesc;
	ID3D12Resource* pTexBuffer;
	ID3D12DescriptorHeap* pTexDescHeap;
};

#endif

