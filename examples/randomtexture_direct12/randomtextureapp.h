/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef RANDOMTEXTUREAPP_H
#define RANDOMTEXTUREAPP_H

#include "mxrenderer.h"
#include "testshaderprogram.h"

// tmp
#include "mxrenderer.h"


// Cross-platform interface
class RandomTextureApp
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


	// MxApplication data members
	MxRenderer pRenderer;

};

#endif
