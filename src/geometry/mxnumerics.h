/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef QXNUMERICS_H
#define QXNUMERICS_H


/*!
  In this file should contains functions
  that need platform abstraction.
  */


#if defined(QX_PLATFORM_WINDOWS)
// http://stackoverflow.com/questions/2789481/problem-calling-stdmax
#include <windows.h>

#define qxMax max
#define qxMin min
#define qxStrcpy strcpy_s
#else
#include <algorithm>
#define qxMax std::max
#define qxMin std::min
#define qxStrcpy strcpy
#endif

#endif
