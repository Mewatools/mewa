/****************************************************************************
** Copyright (C) 2020-2021 Mewatools
** Hugo Silva <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXVECTOR_H
#define MXVECTOR_H

template<int N, typename T>
class MxVector
{
public:
   


    T pVector[N];
};



////////// type definitions ///////////
typedef MxVector<4,unsigned char>  MxVector4UC;


#endif


