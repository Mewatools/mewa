/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXGENERICSUBSTRING_H
#define MXGENERICSUBSTRING_H

#include "mxdebug.h"

/*!
 * This class was introduced to improve performance by avoiding string copies
 * while handling substrings obtained from existing strings.
 * MxGenericSubString holds a const pointer to guarantee a read-only access to
 * string data.
 *
 * \sa MxSubString, MxSubByteArray and MxGenericString
*/
template <typename T>
class MxGenericSubString
{
public:
    MxGenericSubString(){}

    // initialized with a null terminated string
    MxGenericSubString( const T *string )
    {
        pStringStart = string;
        const T *ptr = string;
        while( *ptr != 0 )
        {
            ptr++;
        }
        pSize = ptr - string;
        Q_ASSERT( pSize > 0 );
    }

    MxGenericSubString( const T *string, int size)
    {
        pStringStart = string;
        pSize = size;
    }


    int size() const { return pSize; }
    const T * data() const { return pStringStart; }

    void setSize( int newSize )
    {
        pSize = newSize;
    }

    void advance( int n ){ Q_ASSERT( n < pSize ); pStringStart+=n; pSize-=n; }

    int firstIndexOf( T c ) const
    {
        int from = 0;
        int end = pSize;
        const T * strData = pStringStart;
        while( from < end ) {
            if(strData[from] == c ) {
                return from;
            }
            ++from;
        }
        return -1;
    }

    bool endsWith( const T *match ) const
    {
        Q_ASSERT( NULL != pStringStart );
        int matchSize = strlen(match);

        int strSize = pSize;
        const char *strEnd = pStringStart + (strSize - 1);
        const char *matchEnd = match + (matchSize - 1);
        for(int i=0; i<matchSize; ++i)
        {
            if( *matchEnd != *strEnd )
            {
                return false;
            }
            matchEnd--;
            strEnd--;
        }

        return true;
    }

    void removeLastChars( int n )
    {
        pSize -= n;
        Q_ASSERT( pSize >= 0 );
    }

    bool equals( const T *other, int otherSize ) const
    {
        if( pSize != otherSize ) {
            return false;
        }

        for( int i=0; i<pSize; ++i)
        {
            if( other[i] != pStringStart[i] ) {
                return false;
            }
        }

        return true;
    }

    bool isEqual( const char32_t *other, int otherSize ) const
    {
        if( pSize != otherSize ) {
            return false;
        }

        for( int i=0; i<pSize; ++i)
        {
            unsigned int u = other[i];
            unsigned int a = pStringStart[i];
            if( u != a ) {
                return false;
            }
        }

        return true;
    }

    bool forwardCheck( const T c )
    {
        Q_ASSERT( pSize > 0 );
        if( pStringStart[0] == c ){
            pStringStart++; pSize--;
            return true;
        }
        return false;
    }

    bool isSame( const MxGenericSubString &other ) const
    {
        return (this->pStringStart == other.pStringStart);
    }

    bool startsWith( const T *other ) const
    {
        const char *ptr = pStringStart;
        int i=0;
        while( i < pSize && other[i] != '\0' ) {
            if( ptr[i] != other[i] ) {
                return false;
            }
            i++;
        }
        return true;
    }

    const T & operator[]( int i ) const
    {
        return pStringStart[i];
    }

    // increments pointer for as long its a number
    unsigned int getInt()
    {
        int charCount;
        unsigned int total = intFromString( pStringStart, &charCount );
        pStringStart += charCount;
        pSize -= charCount;
        return total;
    }

    //static unsigned int intFromString( const char *ptr, int *charCount );
    static unsigned int intFromString( const char *ptr, int *charCount )
    {
        int l = 0;
        while( *ptr >= '0' && *ptr <= '9') {
            l++;
            ptr++;
        }

        unsigned int dec = 1;
        unsigned int total = 0;
        unsigned int i = l;
        ptr--;
        while( i > 0 ) {
            unsigned int v = (*ptr - 48);
            Q_ASSERT( v <= 9 );
            total += v * dec;
            dec *= 10;
            i--;
            ptr--;
        }

        *charCount = l;
        return total;
    }


    //////////// data members //////////
    const T *pStringStart;
    int pSize;
};


//////////// type definitions ////////////
typedef MxGenericSubString<char>  MxSubByteArray;
typedef MxGenericSubString<char32_t> MxSubString;


#endif
