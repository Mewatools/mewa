/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXBUFFER_H
#define MXBUFFER_H


/*!
  Holds data to be uploaded to GPU.
 Buffers created from MxRenderer::getReusableBuffer are valid for one frame render
 */
class MxBuffer
{
public:
    MxBuffer();
    virtual ~MxBuffer();

    void reserveForAppend( int size );
    char* alloc( int size, int growBy );
    void clear();
    void append( char *data, int size );
    char* lastDataAndIncrement( int size );
    int size() const;
    void addToSize( int amount );
    const char* data() const;
    char* end();



    char *pData;
    int pSize;
    int pAlloc;
};


#endif

