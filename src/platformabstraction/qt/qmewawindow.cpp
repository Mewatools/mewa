/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include <QMouseEvent>

#include "qmewawindow.h"
#include "mxrect.h"
#include "mxapplication.h"



QMewaWindow::QMewaWindow()
    : QOpenGLWindow(QOpenGLWindow::PartialUpdateBlit)
{

}

QMewaWindow::~QMewaWindow()
{
}


void QMewaWindow::initializeGL()
{
    MxApplication::instance()->pRenderer.initializeOpenGLFunctions();
    
    MxApplication::instance()->initializeGL();
}

void QMewaWindow::resizeGL(int w, int h)
{
    MxApplication::instance()->onResizeWindow( w, h );
}

void QMewaWindow::paintGL()
{
    MxApplication::instance()->paintGL();
}

void QMewaWindow::mousePressEvent( QMouseEvent * event )
{
    QPoint p = event->pos();
    MxApplication::instance()->onMousePress(p.x(), p.y(), event->button(), event->modifiers() | event->buttons() );
    update();
}

void QMewaWindow::mouseMoveEvent( QMouseEvent * event )
{
    QPoint p = event->pos();
    MxApplication::instance()->onMouseMove(p.x(), p.y(), event->modifiers() | event->buttons() );
    update();
}

void QMewaWindow::mouseReleaseEvent( QMouseEvent * event )
{
    QPoint p = event->pos();
    MxApplication::instance()->onMouseRelease(p.x(), p.y());
    update();
}

