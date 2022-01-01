/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXDEBUG_H
#define MXDEBUG_H

// windows


#ifdef _DEBUG
#include<iostream>
#include <stdarg.h>

#include <assert.h>
#define Q_ASSERT assert

// \TODO
#define Q_ASSERT_X(cond, where, what) static_cast<void>(false && (cond))

#else
inline void _noop(void) {}
#define Q_ASSERT(cond) _noop()
#define Q_ASSERT_X(cond, where, what) static_cast<void>(false && (cond))
#endif

inline void DebugOutputFormatString(const char* format, ...) {
#ifdef _DEBUG
	va_list valist;
	va_start(valist, format);
	vprintf(format, valist);
	va_end(valist);
#endif
}
//#define qDebug DebugOutputFormatString

#define qDebug OutputDebugStringA

#endif
