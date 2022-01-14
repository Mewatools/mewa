/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "randomtextureapp.h"

#include<vector>




RandomTextureApp::RandomTextureApp()
{
	pImgWidth = 230;
	pImgHeight = 250;

	pBuffer.reserveForAppend(256);

}


void RandomTextureApp::intialize()
{

	pProgram.init( &pRenderer );
	bool ok = pProgram.compile();
	if (!ok) {
		qDebug("Failed shader compilation");
	}

	
	


	pRenderer.setProgram( &pProgram );
	pRenderer.setTexturesParameters(MxTexture::MipmapFilter | MxTexture::RepeatWrap);



	
	MxVector2I texSize(pImgWidth, pImgHeight);



	// initialize texture
	std::vector<TexRGBA> texturedata(pImgWidth* pImgHeight);
	for (int i = 0; i < pImgHeight; ++i)
	{
		for (int j = 0; j < pImgWidth; ++j)
		{
			TexRGBA& rgba = texturedata[(i * pImgWidth) + j];
			rgba.R = 2;
			rgba.G = 155;
			rgba.B = 255;
			rgba.A = 255;
		}
	}

	

	pTexture = pRenderer.newTexture(texSize, MxTexture::UChar4);
	pTexture->setPixelData((const unsigned char*)texturedata.data(), texSize, MxTexture::UChar4);


}

void RandomTextureApp::onRender()
{


	std::vector<TexRGBA> texturedata(pImgWidth * pImgHeight);
	for (int i = 0; i < pImgHeight; ++i)
	{
		for (int j = 0; j < pImgWidth; ++j)
		{
			TexRGBA& rgba = texturedata[(i * pImgWidth) + j];

			if (j < 8 || j >(pImgWidth - 8))
			{
				rgba.R = 250;
			}
			else {
				rgba.R = rand() % 256;
			}


			rgba.G = 120;
			rgba.B = i % 256;
			rgba.A = 255;
		}
	}


	pRenderer.bindTexture(pTexture, MxTexture::NoFilter | MxTexture::ClampWrap, 0);
	pTexture->setPixelData((const unsigned char*)texturedata.data(), MxVector2I(pImgWidth, pImgHeight) , MxTexture::UChar4);


	pRenderer.setViewport(0,0, pWindowWidth, pWindowHeight);
	pRenderer.setScissor(MxVector2I(0, 0), MxVector2I(pWindowWidth, pWindowHeight));



	MxMatrix m;
	m.setToIdentity();
	m.ortho(0.0f, (float)pWindowWidth, 0.0f, (float)pWindowHeight);

	pBuffer.clear();
	MxIconDraw icondraw(&pBuffer, NULL);
	MxRectF texRect(0.0f, 1.0f, 0.0f, 1.0f);
	MxRectF viewRect(30.0f, 500.0f, 30.0f, 500.0f);
	//icondraw.drawImageRect(texRect, viewRect);

	pProgram.draw(icondraw, &m);


}

void RandomTextureApp::onResizeWindow(int width, int height)
{

	pWindowWidth = width;
	pWindowHeight = height;

}
