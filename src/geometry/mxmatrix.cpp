/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxmatrix.h"
#include "mxdebug.h"

MxMatrix::MxMatrix()
{
// leave data uninitialized as we dont know what the matrix will be
}

void MxMatrix::setToIdentity()
{
    pData[0][0] = 1.0f;
    pData[0][1] = 0.0f;
    pData[0][2] = 0.0f;
    pData[0][3] = 0.0f;
    pData[1][0] = 0.0f;
    pData[1][1] = 1.0f;
    pData[1][2] = 0.0f;
    pData[1][3] = 0.0f;
    pData[2][0] = 0.0f;
    pData[2][1] = 0.0f;
    pData[2][2] = 1.0f;
    pData[2][3] = 0.0f;
    pData[3][0] = 0.0f;
    pData[3][1] = 0.0f;
    pData[3][2] = 0.0f;
    pData[3][3] = 1.0f;
}

void MxMatrix::ortho(float left, float right, float bottom, float top, float near, float far)
{
    Q_ASSERT(left != right && bottom != top);
    float width = right - left;
    float invheight = top - bottom;

	pData[0][0] = 2.0f / width;
	pData[1][1] = 2.0f / invheight;
	pData[2][2] = -1.0f;
	pData[3][0] = -(right + left) / width;
	pData[3][1] = -(top + bottom) / invheight;
}


