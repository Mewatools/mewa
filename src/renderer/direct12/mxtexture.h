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
	enum PixelFormat
	{
		Uninitialized = 0,
		RGBA8, //! 8 bit per pixel, RGBA
		RGBAFloat,
	};

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




	MxTexture();
	~MxTexture();

	void create( MxRenderer* renderer, const MxVector2I& size, PixelFormat format, const void* pixels);

	bool isNull() const;

	//! Set the given pixel data to this texture
	void setData( const void* pixels, const MxVector2I& size, PixelFormat format);





	D3D12_HEAP_PROPERTIES pHeapProp;
	D3D12_RESOURCE_DESC pResDesc;
	ID3D12Resource* pTexBuffer;
	ID3D12DescriptorHeap* pTexDescHeap;
};

#endif

