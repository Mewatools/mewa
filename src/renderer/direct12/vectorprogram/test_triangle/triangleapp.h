/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef TRIANGLEAPP_H
#define TRIANGLEAPP_H

#include "mxvectorprogram.h"
#include "mxapplication.h"
#include "mxtexture.h"



class TriangleApp : public MxApplication
{
public:
	TriangleApp();

	void intialize();
	void onRender();
	void onResizeWindow( int width, int height );




	int pImgWidth;
	int pImgHeight;

	int pWindowWidth;
	int pWindowHeight;

	MxVectorProgram pProgram;
	


	MxBuffer pBuffer;


};

#endif
