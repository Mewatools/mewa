/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "testwindow.h"
#include "mxrect.h"
#include "mxcolorwheelprogram.h"


TestWindow::TestWindow()
    : QOpenGLWindow(QOpenGLWindow::PartialUpdateBlit)
{
pColorWheelProgram = NULL;
}

TestWindow::~TestWindow()
{
    makeCurrent();
    // delete GL resources
}


void TestWindow::initializeGL()
{
    pRenderer.initializeOpenGLFunctions();

    pRenderer.glEnable(GL_DEPTH_TEST);
    pRenderer.glEnable(GL_CULL_FACE);

}

void TestWindow::resizeGL(int w, int h)
{

}

void TestWindow::paintGL()
{
    pRenderer.glClearColor(0, 1, 0, 1);
    pRenderer.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QSize windowSize = size();

    MxMatrix m;
    m.setToIdentity();
    m.ortho(0.0, windowSize.width(), 0.0, windowSize.height(), -1.0, 1.0);

    MxRectF wheelRect( 0.0f, windowSize.width(), 0.0f, windowSize.height() );

    if( NULL == pColorWheelProgram ) {
        pColorWheelProgram = new MxColorWheelProgram();
    }
    pRenderer.setProgram(pColorWheelProgram);
    pColorWheelProgram->setMatrix(&m);

    pColorWheelProgram->setBackgroundColor( 0.0f, 0.0f, 0.0f );

    //  try using length as smoothEdge
    // effect->setSmoothEdge( 1.0f / wheelSize.width() );
    pColorWheelProgram->setSmoothEdge( 0.0f );
    pColorWheelProgram->draw( wheelRect );

    pRenderer.renderEnd();

}
