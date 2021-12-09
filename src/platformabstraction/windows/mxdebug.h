/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXDEBUG_H
#define MXDEBUG_H

#define MX_PLATFORM_WINDOWS



/*#include <wtypes.h>
#include <stdio.h> // , <wchar.h>, <cstdio>, or <cwchar>
static void qDebugW(const wchar_t* szFormat, ...)
{
	wchar_t szBuff[512];
	va_list arg;
	va_start(arg, szFormat);
	_vsnwprintf(szBuff, sizeof(szBuff), szFormat, arg);
	va_end(arg);

	OutputDebugString(szBuff);
}*/


// windows predefined macros https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-170
#if defined(_DEBUG)
#define MX_DEBUG
#else
inline void qt_noop(void) {}
#define qDebug(...) qt_noop()
#define qWarning printf
#define qFatal printf
inline void assert_noop(bool ) {}
#define Q_ASSERT(cond) assert_noop(cond)
#define Q_ASSERT_X(cond, where, what) qt_noop()
#endif







#endif
