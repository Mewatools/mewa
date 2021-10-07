/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxgenericstring.h"

#include <string.h> // strlen, strcpy
#include <stdlib.h> // atoi


template <>
void MxGenericString<char>::operator=( const char *str )
{
    Q_ASSERT( str != NULL );
    int s = strlen(str);

    if( NULL == d )
    {
        allocDynamicString( s + 1 );
    }

    DynamicData *dd = static_cast<DynamicData*>(d);
    char * dst = reinterpret_cast<char *>(dd) + dd->offset;


    Q_ASSERT( dd->alloc >= (s+1) );

    memcpy(dst, str, s+1);
    dd->size = s;
}



template <>
bool MxGenericString<char>::operator==( const char *str ) const
{
    Q_ASSERT( NULL != d );
    Q_ASSERT( str != NULL );

    int s = strlen(str);
    if( d->size != s ) {
        return false;
    }

    if( memcmp(str, data(), s) == 0 ) {
        return true;
    }

    return false;
}




template <>
void MxGenericString<char>::append( const char *str )
{
    Q_ASSERT( NULL != d );
    Q_ASSERT( d->refcount >= 0 );

    Q_ASSERT( str != NULL );
    int s = strlen(str);


    DynamicData *dd = static_cast<DynamicData*>(d);
    Q_ASSERT( dd->alloc >= (dd->size + s) );
    char * dst = reinterpret_cast<char *>(dd) + dd->offset;
    dst += dd->size;
    strcpy( dst, str );
    dd->size += s;
}

template <>
void MxGenericString<char>::prepend( const char *str )
{
    Q_ASSERT( NULL != d );
    Q_ASSERT( d->refcount >= 0 );

    Q_ASSERT( str != NULL );
    int s = strlen(str);


    DynamicData *dd = static_cast<DynamicData*>(d);
    Q_ASSERT( dd->alloc >= (dd->size + s) );
    char * startPtr = reinterpret_cast<char *>(dd) + dd->offset;
    memmove ( startPtr+s, startPtr, s );
    memcpy( startPtr, str, s );
    dd->size += s;
}

template <>
int MxGenericString<char>::toInt() const
{
    DynamicData *dd = static_cast<DynamicData*>(d);
    char * strData = reinterpret_cast<char *>(dd) + dd->offset;

    Q_ASSERT( dd->alloc >= (dd->size + 1) );
    strData[dd->size] = '\0';
    int i = atoi(strData);
    return i;
}

template <>
MxGenericString<char>::MxGenericString( const MxByteArrayRef &ref )
{
    int s = ref.size();
    allocDynamicString( s );
    DynamicData *dd = static_cast<DynamicData*>(d);
    char * dst = reinterpret_cast<char *>(dd) + dd->offset;
    memcpy(dst, ref.data(), s);
    dd->size += s;
}
