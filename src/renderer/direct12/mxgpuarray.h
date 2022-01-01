/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXGPUARRAY_H
#define MXGPUARRAY_H

#include<d3d12.h>


class MxRenderer;


/**
 * DirectX 12 implementation holds a resource.
 * This class is internal to the framework and not available to the application.
 */
class MxGpuArray
{
public:
    enum ArrayType
    {
        Undefined = 0, // reserved for error detection
        IndexUInt16,
        VertexFloat
    };



    MxGpuArray();
    unsigned int size() const;
    ArrayType type() const;
    bool isTaken() const;
    void setIndexData(char* bytes, UINT length );
    void setVertexData(char* bytes, UINT length, int stride);


private:
    void setData( char *bytes, UINT64 length );



public:
    union BufferView
    {
        D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
        D3D12_INDEX_BUFFER_VIEW indexBufferView;
    } pView;


    ID3D12Resource* pBuffer;
    ArrayType pType;
    bool pTaken;

};



#endif

