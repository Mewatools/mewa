/****************************************************************************
** Copyright (C) 2020-2024 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include <QMouseEvent>

#include "qmewapluginwindow.h"
#include "mxinputs.h"



QMewaPluginWindow::QMewaPluginWindow()
    : QOpenGLWindow(QOpenGLWindow::PartialUpdateBlit)
{
    pFirstTime = true;
}

QMewaPluginWindow::~QMewaPluginWindow()
{
}


void QMewaPluginWindow::initializeGL()
{
    pRenderer.initializeOpenGLFunctions();

}

void QMewaPluginWindow::resizeGL(int w, int h)
{
    pRenderer.setWindowSize( w, h );
}

void QMewaPluginWindow::paintGL()
{
    if( pFirstTime ) { // for unknown reason initializeGL() looses bounded textures, so load textures here!
        pFirstTime = false;

        pRenderer.initialize();

        pRenderer.glClearColor(102.0f / 255.0f, 102.0f / 255.0f, 102.0f / 255.0f, 1.0f);
        pRenderer.enableDepthTest(false);

    }

    pRenderer.glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    const MxVector2I& screen = pRenderer.windowSize();
    pRenderer.setViewport(0, 0, screen.width(), screen.height());
    



#if 0
    MxVectorProgram * svgProgram = pRenderer.setVectorProgram();

    MxVector2F tx(0.0f, 0.0f);
    MxBuffer *buffer = pRenderer.getTemporaryBuffer(9500);
    pVectorDraw.pArray = buffer;
    pVectorDraw.pTranslation = &tx;

    pVectorDraw.clear();
    MxVector4UC color(60, 220, 110, 255);
    MxRectF rect(20.0f, 200.0f, 30.0f, 300.0f);
    pVectorDraw.fillRect( rect, color );

    svgProgram->draw( pVectorDraw, pRenderer.windowMatrix());
#else
MxInputs inputs;
inputs.pTime = 1.0f;
     pPlugin.render( pRenderer, inputs);
#endif
    pRenderer.checkGLError(__FILE__, __LINE__);



update();

}

void QMewaPluginWindow::mousePressEvent( QMouseEvent * event )
{
    QPoint p = event->pos();
}

void QMewaPluginWindow::mouseMoveEvent( QMouseEvent * event )
{
    QPoint p = event->pos();
}

void QMewaPluginWindow::mouseReleaseEvent( QMouseEvent * event )
{
    QPoint p = event->pos();
}


/*! \class QMewaPluginWindow
    \brief Qt widget used to test Mewa plugins


    \sa MxPlugin, MxInputs and MxParameter
*/
