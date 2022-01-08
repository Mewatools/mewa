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
    T z() const;

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



    void operator/=( T value )
    {
        for( int i=0; i<N; ++i ) {
            pVector[i] /= value;
        }
    }

    void operator/=( const MxVector<N,T> &other )
    {
        for( int i=0; i<N; ++i ) {
            pVector[i] /= other[i];
        }
    }

    void operator+=( T value )
    {
        for( int i=0; i<N; ++i ) {
            pVector[i] += value;
        }
    }

    void operator-=( T value )
    {
        for( int i=0; i<N; ++i ) {
            pVector[i] -= value;
        }
    }


    // code taken from http://stackoverflow.com/questions/2259476/rotating-a-point-about-another-point-2d
    void rotateBy( T radians ) // rotate relative to 0,0
    {
        T s = sin(radians);
        T c = cos(radians);

        // rotate point
        T tmp = pVector[0] * c - pVector[1] * s;
        pVector[1] = pVector[0] * s + pVector[1] * c;
        pVector[0] = tmp;
    }


    // \TODO make this 2 functions specialized to vec size 2
    friend inline const MxVector<N,T> operator*(float c, const MxVector<N,T> &p)
    {
        return MxVector<N,T>(p[0]*c, p[1]*c);
    }


    friend inline const MxVector<N,T> operator/(const MxVector<N,T> &p, float divisor)
    {
        return MxVector<N,T>(p[0]/divisor, p[1]/divisor);
    }



    ////// data member //////
    T pVector[N];
};

////////// type definitions ///////////
typedef MxVector<2,float>  MxVector2F;
typedef MxVector<3,float>  MxVector3F;
typedef MxVector<4,float>  MxVector4F;
typedef MxVector<4,int>  MxVector4I;
typedef MxVector<3,int>  MxVector3I;
typedef MxVector<2,int> MxVector2I;
typedef MxVector<2,unsigned int> MxVector2UI;
typedef MxVector<3,unsigned char>  MxVector3UC;
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
inline T MxVector<N,T>::z() const
{
    Q_ASSERT( 2 < N );
    return pVector[2];
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


