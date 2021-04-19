/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXVECTOR_H
#define MXVECTOR_H

#include "mxdebug.h"


template<int N, typename T>
class MxVector
{
public:
    MxVector();
    MxVector( T x, T y );
    MxVector( T x, T y, T z );
    MxVector( T x, T y, T z, T w );

    T x() const;
    T y() const;

    T width() const;
    T height() const;


    // compatibility with Qt API
    const T * constData() const;
    T & operator[]( int i );
    const T & operator[]( int i ) const;

    template<typename S>
    MxVector<N,T> & operator=( const MxVector<N,S> &other );

    template<typename S>
    MxVector<N,T> & operator+=( const MxVector<N,S> &other );

    template<typename S>
    MxVector<N,T> & operator-=( const MxVector<N,S> &other );

    template<typename S>
    MxVector<N,T> & operator*=( const MxVector<N,S> &other );

    template<typename S>
    MxVector<N,T> & operator*=( S s );




    T pVector[N];
};

////////// type definitions ///////////
typedef MxVector<2,float>  MxVector2F;
typedef MxVector<3,float>  MxVector3F;
typedef MxVector<4,unsigned char>  MxVector4UC;



template<int N, typename T>
inline MxVector<N,T>::MxVector()
{}


template<int N, typename T>
inline MxVector<N,T>::MxVector( T x, T y )
{
    pVector[0] = x;
    pVector[1] = y;
}

template<int N, typename T>
inline MxVector<N,T>::MxVector( T x, T y, T z )
{
    pVector[0] = x;
    pVector[1] = y;
    pVector[2] = z;
}

template<int N, typename T>
inline MxVector<N,T>::MxVector( T x, T y, T z, T w )
{
    pVector[0] = x;
    pVector[1] = y;
    pVector[2] = z;
    pVector[3] = w;
}


template<int N, typename T>
inline T MxVector<N,T>::x() const
{
    Q_ASSERT( 0 < N );
    return pVector[0];
}

template<int N, typename T>
inline T MxVector<N,T>::y() const
{
    Q_ASSERT( 1 < N );
    return pVector[1];
}


template<int N, typename T>
inline T MxVector<N,T>::width() const
{
    Q_ASSERT( 0 < N );
    return pVector[0];
}

template<int N, typename T>
inline T MxVector<N,T>::height() const
{
    Q_ASSERT( 1 < N );
    return pVector[1];
}


template<int N, typename T>
inline const T * MxVector<N,T>::constData() const
{
    return &pVector[0];
}


template<int N, typename T>
inline T & MxVector<N,T>::operator[]( int i )
{
    Q_ASSERT( i < N );
    return( pVector[i] );
}

template<int N, typename T>
inline const T & MxVector<N,T>::operator[]( int i ) const
{
    Q_ASSERT( i < N );
    return( pVector[i] );
}


template<int N, typename T>
template<typename S>
inline MxVector<N,T> & MxVector<N,T>::operator=( const MxVector<N,S> &other )
{
    for ( int i=0 ; i<N ; ++i )
    {
        pVector[i] = T(other[i]);
    }
    return( *this );
}

template<int N, typename T>
template<typename S>
inline MxVector<N,T> & MxVector<N,T>::operator+=( const MxVector<N,S> &other )
{
    for( int i=0 ; i<N ; ++i )
        pVector[i] += T(other[i]);

    return( *this );
}

template<int N, typename T>
template<typename S>
inline MxVector<N,T> & MxVector<N,T>::operator-=( const MxVector<N,S> &other )
{
    for( int i=0 ; i<N ; ++i )
        pVector[i] -= T(other[i]);

    return( *this );
}

template<int N, typename T>
template<typename S>
inline MxVector<N,T> & MxVector<N,T>::operator*=( const MxVector<N,S> &other )
{
    for( int i=0 ; i<N ; ++i )
        pVector[i] *= T(other[i]);

    return( *this );
}

// \TODO Optimization: make specialized functions to replace the for loop
template<int N, typename T>
template<typename S>
inline MxVector<N,T> & MxVector<N,T>::operator*=( S s )
{
    for ( unsigned int i=0 ; i<N ; ++i )
    {
        pVector[i] *= T(s);
    }
    return( *this );
}

///////////////// non-member functions //////////////
template<int N, typename T>
MxVector<N,T> operator+( const MxVector<N,T> & v0, const MxVector<N,T> & v1 );

template<int N, typename T>
inline MxVector<N,T>  operator+( const MxVector<N,T> & v0, const MxVector<N,T> & v1 )
{
    MxVector<N,T> ret(v0);
    ret += v1;
    return ret;
}


template<int N, typename T>
MxVector<N,T> operator-( const MxVector<N,T> & v0, const MxVector<N,T> & v1 );

template<int N, typename T>
inline MxVector<N,T>  operator-( const MxVector<N,T> & v0, const MxVector<N,T> & v1 )
{
    MxVector<N,T> ret(v0);
    ret -= v1;
    return ret;
}


template<int N, typename T, typename S>
inline MxVector<N,T> operator*( const MxVector<N,T> & v, S s );

template<int N, typename T, typename S>
inline MxVector<N,T>  operator*( const MxVector<N,T> & v, S s )
{
    MxVector<N,T> ret(v);
    ret *= s;
    return( ret );
}


template<int N, typename T>
MxVector<N,T> operator*( const MxVector<N,T> & v0, const MxVector<N,T> & v1 );

template<int N, typename T>
inline MxVector<N,T> operator*( const MxVector<N,T> & v0, const MxVector<N,T> & v1 )
{
    MxVector<N,T> res;
    for ( unsigned int i=0 ; i<N ; ++i )
    {
        res[i] = v0[i] * v1[i];
    }
    return res;
}



template<int N, typename T>
MxVector<N,T> operator/( const MxVector<N,T> & v0, const MxVector<N,T> & v1 );

template<int N, typename T>
inline MxVector<N,T> operator/( const MxVector<N,T> & v0, const MxVector<N,T> & v1 )
{
    MxVector<N,T> res;
    for ( unsigned int i=0 ; i<N ; ++i )
    {
        res[i] = v0[i] / v1[i];
    }
    return res;
}


template<int N, typename T>
bool operator==( const MxVector<N,T> & v0, const MxVector<N,T> & v1 );

template<int N, typename T>
bool operator==( const MxVector<N,T> & v0, const MxVector<N,T> & v1 )
{
    for(int i=0; i<N; ++i) {
        if( v0[i] != v1[i] ) {
            return false;
        }
    }
    return true;
}


template<int N, typename T>
bool operator!=( const MxVector<N,T> & v0, const MxVector<N,T> & v1 );

template<int N, typename T>
bool operator!=( const MxVector<N,T> & v0, const MxVector<N,T> & v1 )
{
    for(int i=0; i<N; ++i) {
        if( v0[i] != v1[i] ) {
            return true;
        }
    }
    return false;
}



#endif


