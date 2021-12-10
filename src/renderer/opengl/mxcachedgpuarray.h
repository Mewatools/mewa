/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXCACHEDGPUARRAY_H
#define MXCACHEDGPUARRAY_H

#include "mxgpuarray.h"


/*! Keeps in RAM a copy of the data stored in vbo (MxGpuArray)
 */
class MxCachedGpuArray : public MxGpuArray
{
public:
    MxCachedGpuArray();
    virtual ~MxCachedGpuArray();

    void reserveForAppend( int size );
    char* alloc( int size, int growBy );
    void clear();
    void append( char *data, int size );
    char* lastDataAndIncrement( int size );
    int size() const;
    void addToSize( int amount );
    const char* data() const;
    char* end();

    void uploadGL(MxRenderer *renderer);



    char *pData;
    int pSize;
    int pAlloc;

};


#endif

