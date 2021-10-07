/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxbytearrayref.h"


void MxByteArrayRef::insert( int index, const char c )
{
    memmove (pStringStart+index+1, pStringStart+index, pSize);
    pStringStart[index] = c;
    pSize++;
}

