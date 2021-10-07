/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXGENERICSTRING_H
#define MXGENERICSTRING_H

#include "mxdebug.h"
#include "mxbytearrayref.h"
#include "mxgenericsubstring.h"

#include <stdlib.h> // malloc
#include <string.h> // strlen

#define MXGENERICSTRING_PREALLOC 3



template <typename T, int N>
struct MxGenericStaticData {
    int refcount;
    int size;
    int offset;
    T array[N];
};


template <typename T>
class MxGenericString
{
public:
    struct Data
    {
        int refcount; // -1 for static strings
        int size;
        int offset;
    };

    struct DynamicData : Data
    {
        int alloc;
    };

    MxGenericString()
    {
        d = 0;
    }

    ~MxGenericString()
    {
        if(d && ! deref())
        {
            free(d);
        }
    }

    MxGenericString( int size )
    {
        allocDynamicString( size );
    }

    MxGenericString( const MxByteArrayRef &ref );

    bool operator==( const MxByteArrayRef &ref ) const
    {
        Q_ASSERT( d != 0 );
        int strSize = size();
        if( strSize != ref.size() ) {
            return false;
        }
        T *refPtr = ref.constData();
        const T * strData = data();
        int i = 0;
        while( i < strSize )
        {
            if( *strData != *refPtr ) {
                return false;
            }
            i++;
            refPtr++;
            strData++;
        }
        return true;
    }

    bool equals( const T* other, int size ) const
    {
        Q_ASSERT( d != 0 );
        int thisSize = this->size();
        if( thisSize != size ) {
            return false;
        }
        const T *refPtr = other;
        const T * strData = data();
        int i = 0;
        while( i < thisSize )
        {
            if( *strData != *refPtr ) {
                return false;
            }
            i++;
            refPtr++;
            strData++;
        }
        return true;
    }

    MxGenericString( const MxGenericString &other )
    {
        d = other.d;
        if(d)
            ref();
    }

    MxGenericString( Data *data )
    {
        Q_ASSERT( data != 0 );
        d = data;
        if(d)
            ref();
    }

    /*! To detach a string, the only way, at the moment, is to assign a null string. */
    MxGenericString& operator=( const MxGenericString &other )
    {
        if(d && ! deref())
        {
            free(d);
        }

        d = other.d;
        if(d)
            ref();
        return *this;
    }

    MxGenericString& operator=( const MxByteArrayRef &ref )
    {
        Q_ASSERT( NULL != d );
        Q_ASSERT( d->refcount >= 0 );
        DynamicData *dd = static_cast<DynamicData*>(d);
        int refSize= ref.size();
        //Q_ASSERT( dd->alloc >= (dd->size + refSize) );
        if( (dd->size + refSize) > dd->alloc )
        {
            d = (Data*)realloc( d, sizeof(DynamicData) + ((ref.size()+1) * sizeof(T)));
        }

        T * strData = data();
        memcpy( strData, ref.data(), refSize);
        dd->size = refSize;
        return *this;
    }

    MxGenericString& operator=( const MxSubByteArray &ref )
    {
        reserveTotal( ref.size() + 1 );
        Q_ASSERT( NULL != d );
        Q_ASSERT( d->refcount >= 0 );
        DynamicData *dd = static_cast<DynamicData*>(d);
        int refSize= ref.size();
        if( (dd->size + refSize) > dd->alloc )
        {
            d = (Data*)realloc( d, sizeof(DynamicData) + ((ref.size()+1) * sizeof(T)));
        }
        T * strData = data();
        memcpy( strData, ref.data(), refSize);
        dd->size = refSize;
        return *this;
    }

    MxGenericString& operator=( Data *other )
    {
        if(d && ! deref())
        {
            free(d);
        }
        d = other;
        return *this;
    }

    void operator=( const char *str );
    bool operator==( const T * str ) const;
    bool operator==( const MxGenericString<T> &other ) const;
    bool operator!=( const MxGenericString<T> &other );
    T operator[]( int index ) const
    {
        Q_ASSERT( NULL != d );
        const T *arrayPtr = data();
        return arrayPtr[index];
    }

    T& operator[]( int index )
    {
        Q_ASSERT( NULL != d );
        T *arrayPtr = data();
        return arrayPtr[index];
    }

    MxGenericString& operator<<( const char *str )
    {
        append( str, strlen(str) );
        return *this;
    }

    MxGenericString& operator<<( char c )
    {
        append( c );
        return *this;
    }

    MxGenericString& operator<<( unsigned int number )
    {
        appendNumber( number );
        return *this;
    }


    MxGenericString& operator<<( const MxGenericString<T> &other )
    {
        append(other);
        return *this;
    }

    //! Strings are implicitly shared, holding a pointer to the string data. This function returns true if \a other holds the same pointer (this) string data
    bool isSame( const MxGenericString<T> &other ) const
    {
        return d == other.d;
    }

    T* data()
    {
        Q_ASSERT( 0 != d );
        return reinterpret_cast<T *>( reinterpret_cast<char *>(d) + d->offset );
    }

    const T* data() const
    {
        Q_ASSERT( NULL != d );
        return reinterpret_cast<T *>( reinterpret_cast<char *>(d) + d->offset );
    }

    int size() const
    {
        if( NULL == d ) {
            return 0;
        }
        return d->size;
    }

    bool isNull() const
    {
        return d == 0;
    }

    int count( T character ) const
    {
        Q_ASSERT( NULL != d );
        int counter = 0;
        const T *arrayPtr = data();
        for(int i=0; i < d->size; ++i)
        {
            if(arrayPtr[i] == character) {
                counter++;
            }
        }
        return counter;
    }

    bool isStatic() const
    {
        return (d->refcount == -1);
    }

    void resize( int size )
    {
        Q_ASSERT( d != NULL );
        Q_ASSERT( static_cast<DynamicData*>(d)->alloc >=  size );
        d->size = size;

    }

    void addToSize( int extra )
    {
        Q_ASSERT( d != NULL );
        Q_ASSERT( static_cast<DynamicData*>(d)->alloc >= ((d->size)+extra) );
        d->size += extra;

    }

    //! Reserve a total of \a size bytes
    void reserveTotal(int size)
    {
        if( d == NULL )
        {
            allocDynamicString( size );
            return;
        }

        Q_ASSERT( d->refcount >= 0 );

        DynamicData *dd = static_cast<DynamicData*>(d);
        if( size > dd->alloc )
        {
            d = (Data*)realloc( d, sizeof(DynamicData) + (size * sizeof(T)));
            Q_ASSERT( NULL != d );
            dd = static_cast<DynamicData*>(d);
            dd->alloc = size;
        }
    }

    //! Reserves extra space for appending N characters, with N being the input argument \a size.
    void reserveForAppend(int size)
    {
        Q_ASSERT( NULL != d );
        Q_ASSERT( d->refcount >= 0 );
        DynamicData *dd = static_cast<DynamicData*>(d);
        int newSize = d->size + size;
        if( newSize > dd->alloc )
        {
            d = (Data*)realloc( d, sizeof(DynamicData) + (newSize * sizeof(T)));
            Q_ASSERT( NULL != d );
            dd = static_cast<DynamicData*>(d);
            dd->alloc = newSize;
        }
    }

    void clear()
    {
        if( d != NULL )
        {
            Q_ASSERT( d->refcount >= 0 );
            DynamicData *dd = static_cast<DynamicData*>(d);
            dd->size = 0;
        }
    }

    void truncate( int position )
    {
        Q_ASSERT( NULL != d );
        Q_ASSERT( d->refcount >= 0 );
        DynamicData *dd = static_cast<DynamicData*>(d);
        dd->size = position;
    }

    void removeLast()
    {
        Q_ASSERT( NULL != d );
        Q_ASSERT( d->refcount >= 0 );
        DynamicData *dd = static_cast<DynamicData*>(d);
        dd->size--;
    }

    T last() const
    {
        Q_ASSERT( NULL != d );
        Q_ASSERT( d->refcount >= 0 );
        DynamicData *dd = static_cast<DynamicData*>(d);
        const T *arrayPtr = data();
        return arrayPtr[dd->size-1];
    }

    void removeFirstChars( int n )
    {
        Q_ASSERT( NULL != d );
        Q_ASSERT( d->refcount >= 0 );
        DynamicData *dd = static_cast<DynamicData*>(d);
        T *arrayPtr = data();
        dd->size -= n;
        Q_ASSERT( dd->size > 0 );
        memmove( arrayPtr, arrayPtr+n, dd->size );
    }

    void prepend( const T *data );

    void prepend( const T *data, int size )
    {
        Q_ASSERT( NULL != d );
        Q_ASSERT( d->refcount >= 0 );
        Q_ASSERT( data != NULL );

        DynamicData *dd = static_cast<DynamicData*>(d);
        Q_ASSERT( dd->alloc >= (dd->size + size) );
        T* startPtr = reinterpret_cast<T*>(dd) + dd->offset;
        memmove( startPtr + size, startPtr, size );
        memcpy( startPtr, data, size );
        dd->size += size;
    }




    void appendNumber( int number )
    {
        Q_ASSERT( NULL != d );
        Q_ASSERT( d->refcount >= 0 );

        DynamicData *dd = static_cast<DynamicData*>(d);
        T * dstPtr = data();
        dstPtr += dd->size;
        if( number >= 0 && number <= 99 )
        {
            Q_ASSERT( dd->alloc >= (dd->size + 1) );

            int firstDigit = number / 10;
            int secondDigit = number % 10;
            T digitChar;
            if( firstDigit > 0 )
            {
                digitChar = firstDigit + 48;
                *dstPtr = digitChar;
                dd->size++;
                dstPtr++;
            }

            Q_ASSERT( dd->alloc >= (dd->size + 1) );


            digitChar = secondDigit + 48;
            *dstPtr = digitChar;
            dd->size++;
        }
        else
        {
            // check how long will be the string
            bool changedSign = false;
            int stringSize = 0;
            if( number < 0 ) {
                changedSign = true;
                stringSize++;
                number *= -1;
            }

            int shifter = number;

            do{ //Move to where representation ends
                ++stringSize;
                shifter = shifter/10;
            }while(shifter);

            if( (dd->size + stringSize) >= dd->alloc )
            {
                int newSize = dd->size + stringSize + 10;
                reserveTotal(newSize);
            }


            dd->size += stringSize;

            char const digit[] = "0123456789";
            if(changedSign) {
                *dstPtr = '-';
                dstPtr++;
                stringSize--;
            }

            dstPtr += (stringSize-1);
            do{ //Move back, inserting digits as u go
                *dstPtr = digit[number%10];
                dstPtr--;
                number = number/10;
                stringSize--;
            }while(stringSize);



        }
    }




    int toInt() const;

    void append( T character )
    {
        if( NULL == d )
        {
            reserveTotal(MXGENERICSTRING_PREALLOC);
        }
        Q_ASSERT( d->refcount >= 0 );
        DynamicData *dd = static_cast<DynamicData*>(d);
        if( dd->alloc == dd->size )
        {
            reserveTotal(dd->alloc + MXGENERICSTRING_PREALLOC);
        }
        T * strData = data();
        strData += d->size;
        *strData = character;
        d->size++;
    }



    void appendNull() // append(0) is ambiguous on windows, can be T* or T
    {
        if (NULL == d)
        {
            reserveTotal(MXGENERICSTRING_PREALLOC);
        }
        Q_ASSERT(d->refcount >= 0);
        DynamicData* dd = static_cast<DynamicData*>(d);
        if (dd->alloc == dd->size)
        {
            reserveTotal(dd->alloc + MXGENERICSTRING_PREALLOC);
        }
        T* strData = data();
        strData += d->size;
        *strData = 0;
        d->size++;
    }

    void append( const MxGenericString<T> &other )
    {
        Q_ASSERT( NULL != d );
        Q_ASSERT( other.d != NULL );
        Q_ASSERT( d->refcount >= 0 );
        int otherSize = other.size();
        DynamicData *dd = static_cast<DynamicData*>(d);
        Q_ASSERT( dd->alloc >= (dd->size + otherSize) );
        T * strData = data();
        strData += dd->size;
        memcpy( strData, other.data(), otherSize * sizeof(T));
        dd->size += otherSize;
    }

    void prepend( const MxGenericString<T> &other )
    {
        Q_ASSERT( NULL != d );
        Q_ASSERT( other.d != NULL );
        Q_ASSERT( d->refcount >= 0 );
        int otherSize = other.size();
        DynamicData *dd = static_cast<DynamicData*>(d);
        Q_ASSERT( dd->alloc >= (dd->size + otherSize) );
        T * startPtr = data();
        memmove( startPtr + otherSize, startPtr, otherSize );
        memcpy( startPtr, other.data(), otherSize );
        dd->size += otherSize;
    }

    void appendAscii( const char *str , int size )
    {
        Q_ASSERT( NULL != d );
        Q_ASSERT( str != NULL );
        Q_ASSERT( d->refcount >= 0 );
        int otherSize = size;//strlen(str);
        DynamicData *dd = static_cast<DynamicData*>(d);
        Q_ASSERT( dd->alloc >= (dd->size + otherSize) );
        T * strData = data();
        strData += dd->size;
        for( int i=0; i<otherSize; ++i)
        {
            *strData = *str;
            strData++;
            str++;
        }
        dd->size += otherSize;
    }

    T* appendPointer( int size )
    {
        Q_ASSERT( NULL != d );
        Q_ASSERT( d->refcount >= 0 );
        DynamicData *dd = static_cast<DynamicData*>(d);
        Q_ASSERT( dd->alloc >= (dd->size + size) );
        T * strData = data();
        strData += dd->size;
        dd->size += size;
        return strData;
    }

    void replace( char before, const char *after )
    {
        Q_ASSERT( NULL != d );
        Q_ASSERT( d->refcount >= 0 );
        DynamicData *dd = static_cast<DynamicData*>(d);

        Q_ASSERT( after != NULL );
        int afterSize = strlen(after);
        T * strData = data();
        int i=0;
        while( i<dd->size )
        {
            if( *strData == before ) {
                reserveTotal( dd->size + afterSize );
                memmove(strData+afterSize, strData+1, dd->size - (i+1));
                dd->size += (afterSize-1);
                memcpy(strData, after, afterSize);
                strData += afterSize;
                i+=afterSize;
            }
            else {
                strData++;
                ++i;
            }

        }
    }

    // returns -1 if none is found
    int firstIndexOf( char c , int from = 0 ) const
    {
        Q_ASSERT( NULL != d );

        Q_ASSERT( from >= 0 ); // \TODO
        int end = size();
        const T * strData = data();
        while( from < end ) {
            if(strData[from] == c ) {
                return from;
            }
            ++from;
        }
        return -1;
    }

    // returns -1 if none is found
    int lastIndexOf( char c , int from = -1)
    {
        Q_ASSERT( NULL != d );

        Q_ASSERT( from == -1 ); // \TODO
        int i = size() + from;
        T * strData = data();
        while( i >= 0 ) {
            if(strData[i] == c ) {
                return i;
            }
            --i;
        }
        return -1;
    }

    // \TODO use MxSubByteArray inside
    bool endsWith( const char *match ) const
    {
        Q_ASSERT( NULL != d );
        int matchSize = strlen(match);

        int strSize = d->size;
        const char *strEnd = data() + (strSize - 1);
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

    
    // \TODO use MxSubByteArray inside
    bool startsWith( const char *match ) const
    {
        Q_ASSERT( NULL != d );
        int matchSize = strlen(match);
        int strSize = d->size;
        Q_ASSERT( matchSize < strSize );
        const char *str = data();
        const char *matchPtr = match;
        for(int i=0; i<matchSize; ++i)
        {
            if( *matchPtr != *str )
            {
                return false;
            }
            matchPtr++;
            str++;
        }
        return true;
    }

    void insert( int pos, T ch )
    {
        Q_ASSERT( NULL != d );


        if( pos == d->size )
        {
            append( ch );
        }
        else
        {
            reserveTotal( d->size + 2 );
            T *ptr = data() + pos;
            memmove( ptr + 1, ptr, (d->size - pos) * sizeof(T) );
            *ptr = ch;
            d->size++;
        }
    }


    // append NULL terminated string
    void append( const T *str )
    {
        Q_ASSERT( NULL != d );
        Q_ASSERT( d->refcount >= 0 );
        Q_ASSERT( str != NULL );
        DynamicData *dd = static_cast<DynamicData*>(d);
        T* dst = data();
        dst += dd->size;
        const T *ptr = str;
        int length = 0;
        while( *ptr != NULL ) {
            *dst = *ptr;
            ptr++;
            dst++;
            length++;
        }
        dd->size += length;
    }

    void append( const T *str, int size )
    {
        if( NULL == d )
        {
            // \TODO resize using steps function
            allocDynamicString( size * 2 * sizeof(T) );
        }

        Q_ASSERT( d->refcount >= 0 );
        Q_ASSERT( str != NULL );

        DynamicData *dd = static_cast<DynamicData*>(d);
        if( dd->alloc < (dd->size + size) )
        {
            d = (DynamicData*)realloc( d, sizeof(DynamicData) + ((dd->size + size + 1) * sizeof(T)));
            dd = static_cast<DynamicData*>(d);
        }
        T* dst = data();
        dst += dd->size;
        memcpy(dst, str, size * sizeof(T) );
        dd->size += size;
    }



    void remove( int position, int n )
    {
        Q_ASSERT( NULL != d );
        Q_ASSERT( position >= 0 && position < d->size );


        if( position == (d->size-1) )
        {
            // do nothing
        }
        else
        {
            T *ptr = data() + position + 1;
            memmove( ptr - 1, ptr, (d->size - position) * sizeof(T) );
        }
        d->size--;
    }


    bool equalsSubString( const char *str, int size ) const
    {
        Q_ASSERT( NULL != d );
        Q_ASSERT( str != NULL );

        if( d->size != size ) {
            return false;
        }
        return ( memcmp(str, data(), size) == 0 );
    }

    MxGenericString detachTruncated( int position )
    {
        Q_ASSERT( NULL != d );

        T * srcData = data();
        Q_ASSERT( size() >= position );

        MxGenericString newString;
        newString.allocDynamicString( position );
        DynamicData *dd = static_cast<DynamicData*>(newString.d);
        memcpy(newString.data(), srcData, position );
        dd->size = position;
        return newString;
    }

    MxGenericString detachSubstring( int from, int to )
    {
        Q_ASSERT( NULL != d );

        T * srcData = data();
        Q_ASSERT( size() > from && size() > to );

        int newSize = (to - from) + 1;
        MxGenericString newString;
        newString.allocDynamicString( newSize );
        DynamicData *dd = static_cast<DynamicData*>(newString.d);
        memcpy(newString.data(), srcData + from, newSize * sizeof(T) );
        dd->size = newSize;
        return newString;
    }

    // used to know if '\0' can be added to data(), needed when using fopen
    bool canAppendWithoutRealloc()
    {
        Q_ASSERT( NULL != d );
        Q_ASSERT( d->refcount >= 0 );
        DynamicData *dd = static_cast<DynamicData*>(d);
        return dd->alloc > dd->size;
    }

    void makeCString()
    {
        Q_ASSERT( canAppendWithoutRealloc() );
        T * srcData = data();
        srcData[size()] = '\0';
    }



///////// DATA MEMBERS /////////////
    Data *d;
////////////////////////////////////
private:



    inline void allocDynamicString( int size )
    {
        DynamicData *dd = (DynamicData*)malloc( sizeof(DynamicData) + (size * sizeof(T)));
        dd->refcount = 1;
        dd->size = 0;
        dd->offset = sizeof(DynamicData);
        dd->alloc = size;
        d = dd;
    }

    void detach()
    {
        Q_ASSERT( NULL != d );
        //if(d && ! deref())
        if( ! deref() )
        {
            free(d);
        }
    }


    void ref() {
        if (d->refcount > 0)
            d->refcount++;
    }

    // return false to delete
    bool deref() {
        if (d->refcount == -1)
            return true;

        return d->refcount--;
    }
};



template <typename T>
bool MxGenericString<T>::operator==( const MxGenericString<T> &other ) const
{
    return (d == other.d) || ((size() == other.size()) && (memcmp(data(), other.data(), size())==0));
}

template <typename T>
bool MxGenericString<T>::operator!=( const MxGenericString<T> &other )
{
    return !(*this == other);
}


/////////////////////////////////////////////////
// char specialization
/////////////////////////////////////////////////
template <>
void MxGenericString<char>::operator=( const char *str );

template <>
bool MxGenericString<char>::operator==( const char *str ) const;

template <>
void MxGenericString<char>::prepend( const char *str );

template <>
int MxGenericString<char>::toInt() const;

template <>
MxGenericString<char>::MxGenericString( const MxByteArrayRef &ref );


#endif
