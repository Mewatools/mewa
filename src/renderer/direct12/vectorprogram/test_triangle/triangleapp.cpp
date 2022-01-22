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
	MxVector2F tx(0.0f, 0.0f);
	MxVectorDraw vectordraw(&pBuffer, &tx);
	vectordraw.addSquaredCircle( MxVector2F(250.0f, 250.0f), MxVector2F(90.0f, 90.0f), MxVector4UC(80, 150, 10, 255) );
	//MxRectF texRect(0.0f, 1.0f, 0.0f, 1.0f);
	//MxRectF viewRect(30.0f, 500.0f, 30.0f, 500.0f);
	//icondraw.drawImageRect(texRect, viewRect);






	pProgram.draw(vectordraw, &m);


}

void TriangleApp::onResizeWindow(int width, int height)
{

	pWindowWidth = width;
	pWindowHeight = height;

}
