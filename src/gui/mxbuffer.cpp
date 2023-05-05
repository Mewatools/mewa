/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxbuffer.h"
#include "mxdebug.h"


MxBuffer::MxBuffer()
{
    pData = NULL;
    pSize = 0;
    pAlloc = 0;
}

MxBuffer::~MxBuffer()
{
    if(pData) {
        free(pData);
    }
}

void MxBuffer::reserveForAppend( int size )
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

char* MxBuffer::alloc( int size, int growBy )
{
    if( pSize + size > pAlloc ) {
        pAlloc = (pSize + growBy);
        pData = (char*)realloc(pData, pAlloc);
    }

    pSize += size;
    return pData + pSize;
}

void MxBuffer::clear()
{
    pSize = 0;

}

void MxBuffer::append( char *data, int size )
{
    reserveForAppend(size);
    memcpy(pData + pSize, data, size);
    pSize += size;
}

char* MxBuffer::lastDataAndIncrement( int size )
{
    reserveForAppend( size );
    char *ptr = pData + pSize;
    pSize += size;
    Q_ASSERT( pAlloc >= pSize );
    return ptr;
}

int MxBuffer::size() const
{
    return pSize;
}

void MxBuffer::addToSize( int amount )
{
    Q_ASSERT( (pSize + amount) <= pAlloc );
    pSize += amount;
}

const char* MxBuffer::data() const
{
    return pData;
}

char* MxBuffer::end()
{
    char *ptr = pData + pSize;
    return ptr;
}
