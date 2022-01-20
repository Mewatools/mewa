/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "triangleapp.h"

#include<vector>




TriangleApp::TriangleApp()
{
	pImgWidth = 230;
	pImgHeight = 250;

	pBuffer.reserveForAppend(256);

}


void TriangleApp::intialize()
{

	pProgram.init( &pRenderer );



}

void TriangleApp::onRender()
{


	pRenderer.setViewport(0,0, pWindowWidth, pWindowHeight);
	pRenderer.setScissor(MxVector2I(0, 0), MxVector2I(pWindowWidth, pWindowHeight));



	pRenderer.setProgram(&pProgram);
	//pRenderer.setTexturesParameters(MxTexture::MipmapFilter | MxTexture::RepeatWrap);


	

	MxMatrix m;
	m.setToIdentity();
	m.ortho(0.0f, (float)pWindowWidth, 0.0f, (float)pWindowHeight);

	pBuffer.clear();
	MxVectorDraw vectordraw(&pBuffer, NULL);
	MxRectF texRect(0.0f, 1.0f, 0.0f, 1.0f);
	MxRectF viewRect(30.0f, 500.0f, 30.0f, 500.0f);
	//icondraw.drawImageRect(texRect, viewRect);



	pRenderer.setBlending(MxRenderer::BlendingImages);

    pRenderer.prepareToDraw();


	pProgram.draw(vectordraw, &m);


}

void TriangleApp::onResizeWindow(int width, int height)
{

	pWindowWidth = width;
	pWindowHeight = height;

}
