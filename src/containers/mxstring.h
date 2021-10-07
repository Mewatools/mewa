/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXSTRING_H
#define MXSTRING_H

#include "mxgenericstring.h"


typedef MxGenericString<char32_t> MxString;


#define MX_STATIC_STRING(var,str) enum { var ## Size = sizeof( U ## str ) / sizeof(char32_t) }; \
static MxGenericStaticData< char32_t, var ## Size > var ## _static = { -1, var ## Size - 1, sizeof(MxGenericString<char32_t>::Data), U ## str }; \
MxString var( (MxString::Data*)& var ## _static );



#endif
