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
}


void RandomTextureApp::intialize()
{

	pProgram.init( &pRenderer );
	bool ok = pProgram.compile();
	if (!ok) {
		qDebug("Failed shader compilation");
	}

	
	


	pRenderer.setProgram( &pProgram );
	pRenderer.setTexturesParameters(MxRenderer::MipmapFilter | MxRenderer::RepeatWrap);



	
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
	pTexture->setPixelData((const char*)texturedata.data(), texSize, MxTexture::UChar4);

	pProgram.setInputTexture(pTexture);
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



	pTexture->setPixelData((const char*)texturedata.data(), MxVector2I(pImgWidth, pImgHeight) , MxTexture::UChar4);


	pRenderer.setViewport((float)pWindowWidth, (float)pWindowHeight);
	pRenderer.setScissor(MxVector2I(0, 0), MxVector2I(pWindowWidth, pWindowHeight));





	TestShaderProgram::Vertex vertices[] = {
	{{-0.5f ,-0.9f,0.0f},{0.0f,1.0f} },
	{{-0.5f ,0.9f,0.0f} ,{0.0f,0.0f}},
	{{0.5f ,-0.9f,0.0f} ,{1.0f,1.0f}},
	{{0.5f ,0.9f,0.0f} ,{1.0f,0.0f}},
	};

	unsigned short indices[] = { 0,1,2, 2,1,3 };

	pProgram.draw(vertices, sizeof(vertices), indices, sizeof(indices));


}

void RandomTextureApp::onResizeWindow(int width, int height)
{

	pWindowWidth = width;
	pWindowHeight = height;

}