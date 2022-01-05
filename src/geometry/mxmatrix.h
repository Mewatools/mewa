/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXMATRIX_H
#define MXMATRIX_H

// \TODO remove all Qt dependencies
#if QT_VERSION >= 0x040100
#include <QMatrix4x4>

// \TODO use glm::mat4
typedef QMatrix4x4 MxMatrix;
#else


class MxMatrix
{
public:


	MxMatrix();



};


#endif



#endif
