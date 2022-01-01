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
		UChar4, //! 8 bit per pixel, RGBA
		Float4,
	};

	MxTexture();
	~MxTexture();

	void init( MxRenderer* renderer, int imgWidth, int imgHeight );

	//! Set the given pixel data to this texture
	void setPixelData( const char* pixels, const MxVector2I& size, PixelFormat format);






	D3D12_HEAP_PROPERTIES pHeapProp;
	D3D12_RESOURCE_DESC pResDesc;
	ID3D12Resource* pTexBuffer;
};

#endif

