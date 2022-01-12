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
	void setToIdentity();
	void ortho(float left, float right, float bottom, float top, float near = -1.0f, float far = 1.0f );

	const float* data() const;

private:
	float pData[4][4];
};


#endif



#endif
