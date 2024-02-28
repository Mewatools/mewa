/****************************************************************************
** Copyright (C) 2020-2024 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include <QMouseEvent>

#include "qmewapluginwindow.h"
#include "mxinputs.h"
#include "mxplugin.h"


QMewaPluginWindow::QMewaPluginWindow()
    : QOpenGLWindow(QOpenGLWindow::PartialUpdateBlit)
{
    pPlugin = nullptr;
    pFirstTime = true;
}

QMewaPluginWindow::~QMewaPluginWindow()
{
}

void QMewaPluginWindow::setPlugin(MxPlugin *plugin)
{
    pPlugin = plugin;
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
    

    MxInputs inputs;
    inputs.pOutputResolution = screen;
    inputs.pTime = 1.0f;
    pPlugin->render( pRenderer, inputs);

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
