/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxgpuarray.h"
#include "mxdebug.h"


MxGpuArray::MxGpuArray()
{
	pBuffer = nullptr;
	pType = MxGpuArray::Undefined;
	pTaken = false;
}

unsigned int MxGpuArray::size() const
{
	switch (pType)
	{
	case IndexUInt16:
		return pView.indexBufferView.SizeInBytes;
	case VertexFloat:
		return pView.vertexBufferView.SizeInBytes;
	default:
		qDebug("TODO");
	}
	return 0;
}

MxGpuArray::ArrayType MxGpuArray::type() const
{
	return pType;
}

bool MxGpuArray::isTaken() const
{
	return pTaken;
}

void MxGpuArray::setIndexData(char* bytes, UINT length)
{
	Q_ASSERT(pType == Undefined || pType == IndexUInt16);
	pType = IndexUInt16;

	setData( bytes, length);

	D3D12_INDEX_BUFFER_VIEW &ibView = pView.indexBufferView;
	Q_ASSERT( NULL != pBuffer );
	ibView.BufferLocation = pBuffer->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = length;

}

void MxGpuArray::setVertexData(char* bytes, UINT length, int stride )
{
	Q_ASSERT(pType == Undefined || pType == VertexFloat);
	pType = VertexFloat;

	setData(bytes, length);

	D3D12_VERTEX_BUFFER_VIEW &vbView = pView.vertexBufferView;
	Q_ASSERT(NULL != pBuffer);
	vbView.BufferLocation = pBuffer->GetGPUVirtualAddress();
	vbView.SizeInBytes = length;
	vbView.StrideInBytes = stride;
}


void MxGpuArray::setData(char* bytes, UINT64 length)
{
	Q_ASSERT(NULL != pBuffer);
	char* writePtr = nullptr;
	HRESULT result = pBuffer->Map(0, nullptr, (void**)&writePtr);

	memcpy(writePtr, bytes, length);

	pBuffer->Unmap(0, nullptr);
}