/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef TRIANGLEAPP_H
#define TRIANGLEAPP_H

#include "mxvectorprogram.h"
#include "mxapplication.h"



class TriangleApp : public MxApplication
{
public:
	TriangleApp();

	void intialize();
	void onRender();
	void onResizeWindow( int width, int height );



	struct TexRGBA {
		unsigned char R, G, B, A;
	};

	int pImgWidth;
	int pImgHeight;

	int pWindowWidth;
	int pWindowHeight;

	MxVectorProgram pProgram;
	MxTexture pTexture;


	MxBuffer pBuffer;


};

#endif
