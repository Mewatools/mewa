/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXDEBUG_H
#define MXDEBUG_H

#define MX_PLATFORM_LINUX

#ifdef DEBUG
#define MX_DEBUG
#else
#include <stdio.h>
#define qDebug printf
#define qWarning printf
#define qFatal printf
inline void qt_noop(void) {}
#define Q_ASSERT(cond) if(!(cond)) printf("ASSERT in file %s, line %d",__FILE__,__LINE__); else qt_noop()
#define Q_ASSERT_X(cond, where, what) qt_noop()
#endif



#endif
