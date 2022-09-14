/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXFUZZY_H
#define MXFUZZY_H


#include "mxnumerics.h"

#include <complex>


namespace MxFuzzy
{



// returns true if they are considered equal. numbers are compared in a relative way, where the exactness is stronger the smaller the numbers are
inline bool fuzzyCompare( float p1, float p2 )
{
    return (std::abs(p1 - p2) * 100000.f <= qxMin(std::abs(p1), std::abs(p2)));
}

inline bool fuzzyCompare( double p1, double p2 )
{
    return (std::abs(p1 - p2) * 1000000000000. <= qxMin(std::abs(p1), std::abs(p2)));
}



inline bool fuzzyIsNull(double d)
{
    return std::abs(d) <= 0.000000000001;
}

inline bool fuzzyIsNull(float f)
{
    return std::abs(f) <= 0.00001f;
}


}

#endif
