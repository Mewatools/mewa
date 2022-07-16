/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXFIXEDSIZELIST_H
#define MXFIXEDSIZELIST_H

#include "mxlist.h"


template < typename T, typename Initializer = MxClassInitializer<T>, int N = 4 >
struct MxListStaticData {
    int refcount;
    int alloc;
    int begin;
    int end;
    T array[N];
};

#define MX_STATIC_POD_LIST(var,type,size) static MxListStaticData< type, MxPodInitializer<type>, size > var ## _static = { -1, size, 0, 0, {0} }; \
MxList< type, MxPodInitializer<type> > var( (MxList< type, MxPodInitializer<type> >::Data*) & var ## _static );


#define MX_POD_LIST(var,type,size) MxListStaticData< type, MxPodInitializer<type>, size > var ## _static = { -1, size, 0, 0 }; \
MxList< type, MxPodInitializer<type> > var( (MxList< type, MxPodInitializer<type> >::Data*) & var ## _static );


//////////////////////////////////////////////
// For use as class data members
//////////////////////////////////////////////

#define MX_MEMBER_POD_LIST_DECL(var,type,size) MxListStaticData<  type, MxPodInitializer<type>, size > var ## _list; type var ## _data[size];

#if QX_PLATFORM_WINDOWS
#define MX_MEMBER_POD_LIST_INIT(var,type,size) MxListStaticData< type, MxPodInitializer<type>, size > var ## _tmp = { -1, size, 0, 0, var ## _data[size] }; \
var ## _list = var ## _tmp; \
MxList< type, MxPodInitializer<type> > var ( (MxList< type, MxPodInitializer<type> >::Data*) & var ## _list);
#else
#define MX_MEMBER_POD_LIST_INIT(var,type,size) var ## _list = (MxListStaticData< type, MxPodInitializer<type>, size >) { -1, size, 0, 0, {var ## _data[size]} }; \
MxList< type, MxPodInitializer<type> > var ( (MxList< type, MxPodInitializer<type> >::Data*) & var ## _list);
#endif

#define MX_MEMBER_POD_LIST_USE(var,type) MxList< type, MxPodInitializer<type> > var ( (MxList< type, MxPodInitializer<type> >::Data*) & var ## _list);





#endif
