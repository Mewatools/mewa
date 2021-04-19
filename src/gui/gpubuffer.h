/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef GPUBUFFER_H
#define GPUBUFFER_H

#include "gpuvbo.h"


class GpuBuffer : public GpuVbo
{
public:
    GpuBuffer();
    virtual ~GpuBuffer();

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

