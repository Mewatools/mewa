/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXMATHUTILS_H
#define MXMATHUTILS_H

#include "mxdebug.h"

#ifdef Q_OS_WIN32
#define USE_32BIT_OPTIMIZED_NEXT_POWER_OF_TWO
#endif

#ifndef USE_32BIT_OPTIMIZED_NEXT_POWER_OF_TWO
#include "math.h"
#endif



namespace MxMathUtils
{




/*!
        Returns the next power of two that is greater than or
        equal to \a value.  The \a value must be positive or the
        result is undefined.

        This is a convenience function for use with GL texture
        handling code.
    */
inline int nextPowerOfTwo(int value)
{
#ifdef USE_32BIT_OPTIMIZED_NEXT_POWER_OF_TWO
    value--;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    // for 64-bit arch add: value |= value >> 32;
    ++value;
    return value;
#else
    if ( value < 2 )
        return 0;

    const float ONE_OVER_M_LN2 = 1.442695040888963407;
    int log2scale = log((float)value) * ONE_OVER_M_LN2;
    int result = 1 << log2scale;

    if ( result != value )
        result = result << 1;

    return result;
#endif
}

inline bool isPowerOfTwo( int value )
{
    Q_ASSERT( value != 0 );
    return ! (value & ( value - 1 ) );
}


}



#endif
