/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "gpubuffer.h"
#include "mxdebug.h"
#include "mxopengl.h"
#include "mxrenderer.h"

#include <stdlib.h>
#include <string.h>


GpuBuffer::GpuBuffer()
{
    pData = NULL;
    pSize = 0;
    pAlloc = 0;
    pVbo = 0;
    pVboSize = 0;
    pVaoObject = 0;
}

GpuBuffer::~GpuBuffer()
{
    if(pData) {
        free(pData);
    }
}

void GpuBuffer::reserveForAppend( int size )
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

char* GpuBuffer::alloc( int size, int growBy )
{
    if( pSize + size > pAlloc ) {
        pAlloc = (pSize + growBy);
        pData = (char*)realloc(pData, pAlloc);
    }

    pSize += size;
    return pData + pSize;
}

void GpuBuffer::clear()
{
    pSize = 0;

}

void GpuBuffer::append( char *data, int size )
{
    reserveForAppend(size);
    memcpy(pData + pSize, data, size);
    pSize += size;
}

char* GpuBuffer::lastDataAndIncrement( int size )
{
    reserveForAppend( size );
    char *ptr = pData + pSize;
    pSize += size;
    Q_ASSERT( pAlloc >= pSize );
    return ptr;
}

int GpuBuffer::size() const
{
    return pSize;
}

void GpuBuffer::addToSize( int amount )
{
    Q_ASSERT( (pSize + amount) < pAlloc );
    pSize += amount;
}

const char* GpuBuffer::data() const
{
    return pData;
}

char* GpuBuffer::end()
{
    char *ptr = pData + pSize;
    return ptr;
}


void GpuBuffer::uploadGL( MxRenderer *renderer )
{
    uploadToVbo( renderer, pData, pSize );
}
