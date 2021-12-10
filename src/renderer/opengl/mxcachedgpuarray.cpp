/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxcachedgpuarray.h"
#include "mxdebug.h"
#include "mxopengl.h"
#include "mxrenderer.h"

#include <stdlib.h>
#include <string.h>


MxCachedGpuArray::MxCachedGpuArray()
    :MxGpuArray()
{
    pData = NULL;
    pSize = 0;
    pAlloc = 0;
    pVboSize = 0;
    pVaoObject = 0;
}

MxCachedGpuArray::~MxCachedGpuArray()
{
    if(pData) {
        free(pData);
    }
}

void MxCachedGpuArray::reserveForAppend( int size )
{
    int neededSize = pSize + size;
    if( neededSize > pAlloc ) {

        // reallocate in chunks (stairs function)
        const int chunk = 4096;
        float tmp = ( (float)neededSize / (float)(chunk) );
        int nextLimit = (int)tmp;
        nextLimit += 1;
        int newSize = nextLimit * chunk;
        Q_ASSERT(newSize >= neededSize);
        pAlloc = newSize;
        pData = (char*)realloc(pData, pAlloc);
    }
}

char* MxCachedGpuArray::alloc( int size, int growBy )
{
    if( pSize + size > pAlloc ) {
        pAlloc = (pSize + growBy);
        pData = (char*)realloc(pData, pAlloc);
    }

    pSize += size;
    return pData + pSize;
}

void MxCachedGpuArray::clear()
{
    pSize = 0;

}

void MxCachedGpuArray::append( char *data, int size )
{
    reserveForAppend(size);
    memcpy(pData + pSize, data, size);
    pSize += size;
}

char* MxCachedGpuArray::lastDataAndIncrement( int size )
{
    reserveForAppend( size );
    char *ptr = pData + pSize;
    pSize += size;
    Q_ASSERT( pAlloc >= pSize );
    return ptr;
}

int MxCachedGpuArray::size() const
{
    return pSize;
}

void MxCachedGpuArray::addToSize( int amount )
{
    Q_ASSERT( (pSize + amount) < pAlloc );
    pSize += amount;
}

const char* MxCachedGpuArray::data() const
{
    return pData;
}

char* MxCachedGpuArray::end()
{
    char *ptr = pData + pSize;
    return ptr;
}


void MxCachedGpuArray::uploadGL( MxRenderer *renderer )
{
    uploadToVbo( renderer, pData, pSize );
}

