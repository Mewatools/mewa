/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXBYTEARRAYREF_H
#define MXBYTEARRAYREF_H

#include "mxdebug.h"

#include <stdlib.h> // strtod
#include <string.h> // strlen



/*
 * MxByteArrayRef holds a pointer to the string data owned by MxByteArray.
 * Use it to modify substrings.
 * For read-only acess of MxByteArray use MxSubByteArray.
 *
 * \sa MxgenericString, MxString, MxGenericSubString
 */
class MxByteArrayRef
{
public:

    MxByteArrayRef()
    {
        pStringStart = NULL;
        pSize = 0;
    }

    MxByteArrayRef( char *string, int size )
    {
        pStringStart = string;
        pSize = size;
    }

    int size() const { return pSize; }
    char * data() const { return pStringStart; }
    const char * constData() const { return pStringStart; }

    char firstChar() const { return *pStringStart; }
    void removeFirstChar(){ pStringStart++; pSize--; }

    void advance( int n ){ Q_ASSERT( n < pSize ); pStringStart+=n; pSize-=n; }

    MxByteArrayRef readPrintable()
    {
        MxByteArrayRef ret(pStringStart, 0);
        int beforeSize = pSize;
        while( *pStringStart != ' ' && *pStringStart != '\n' ) {
            pStringStart++;
            pSize--;
        }
        ret.setSize( beforeSize - pSize );
        return ret;
    }

    void readWord()
    {
        Q_ASSERT( *pStringStart != ' ' );
        if( startsWithLetter() ) {
            removeFirstChar();
            while( startsWithLetterOrNumber() ) {
                removeFirstChar();
            }
        }
    }

    bool startsWithLetter() const
    {
        char c = *pStringStart;
        bool r = ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) ;
        return r;
    }

    bool startsWithLetterOrNumber() const
    {
        char c = *pStringStart;
        bool r = ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' )
                || ( c >= '0' && c <= '9' ) ;
        return r;
    }



    void skipChars( char c1, char c2 )
    {
        while( *pStringStart == c1 || *pStringStart == c2 )
        {
            pStringStart++;
            pSize--;
        }
    }

    void goTo( char c )
    {
        while( pSize > 0 && *pStringStart != c )
        {
            pStringStart++;
            pSize--;
        }
    }

    const char & operator[]( int i ) const
    {
        return pStringStart[i];
    }

    bool operator==( const MxByteArrayRef &other ) const
    {
        if( pSize == other.pSize )
        {
            for( int i=0; i<pSize; ++i)
            {
                if( pStringStart[i] != other.pStringStart[i] ) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    // compares with null terminated string
    bool operator==( const char *str ) const
    {
        int i=0;
        for( ; i<pSize; ++i)
        {
            if( pStringStart[i] != str[i] ) {
                return false;
            }
        }
        if(str[i] == '\0') {
            return true;
        }

        return false;
    }


    // advances as long this string matches str, returns true if match. This ref is incremented
    bool forwardCheck( const char *str )
    {
        Q_ASSERT( pSize > (int)strlen(str) );
        while( *str != '\0' )
        {
            if( *str != *pStringStart ) {
                return false;
            }
            str++;
            pStringStart++;
            pSize--;
        }
        return true;
    }

    bool startsWith( const char *str )
    {
        int strSize = strlen(str) - 1;
        while( *(pStringStart+strSize) == *(str+strSize)
               && strSize > 0
               )
        {
            strSize--;
        }
        return strSize == 0;
    }


    void makeCString()
    {
        char * srcData = data();
        srcData[size()] = '\0';
    }

    void insert( int index, const char c );


private:
    void setSize( int newSize )
    {
        pSize = newSize;
    }
   
    int pSize; 
    char *pStringStart;
};




#endif
