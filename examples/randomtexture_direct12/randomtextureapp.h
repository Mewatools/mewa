/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef RANDOMTEXTUREAPP_H
#define RANDOMTEXTUREAPP_H

#include "testshaderprogram.h"
#include "mxapplication.h"



class RandomTextureApp : public MxApplication
{
public:
	RandomTextureApp();

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

	TestShaderProgram pProgram;
	MxTexture* pTexture;



};

#endif