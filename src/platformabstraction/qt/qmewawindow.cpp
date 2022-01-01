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
    pApp.init( &pIconAtlas );
}

QMewaWindow::~QMewaWindow()
{
}


void QMewaWindow::initializeGL()
{
    MxApplication *app = MxGuiAggregation::instance()->application();
    app->pRenderer.initializeOpenGLFunctions();
    app->initialize();

    pIconAtlas.loadGL( app->pRenderer );
}

void QMewaWindow::resizeGL(int w, int h)
{
    MxGuiAggregation::instance()->application()->onResizeWindow( w, h );
}

void QMewaWindow::paintGL()
{
    MxGuiAggregation::instance()->application()->onRender();
}

void QMewaWindow::mousePressEvent( QMouseEvent * event )
{
    QPoint p = event->pos();
    MxGuiAggregation::instance()->application()->onMousePress(p.x(), p.y(), event->button(), event->modifiers() | event->buttons() );
    update();
}

void QMewaWindow::mouseMoveEvent( QMouseEvent * event )
{
    QPoint p = event->pos();
    MxGuiAggregation::instance()->application()->onMouseMove(p.x(), p.y(), event->modifiers() | event->buttons() );
    update();
}

void QMewaWindow::mouseReleaseEvent( QMouseEvent * event )
{
    QPoint p = event->pos();
    MxGuiAggregation::instance()->application()->onMouseRelease(p.x(), p.y());
    update();
}

void QMewaWindow::setMainWidget( MxWidget *widget )
{
    MxGuiAggregation::instance()->application()->setMainWidget( widget );
}

void QMewaWindow::setIcon( MxThemeIcons::IconName name, const char *imageFileName )
{
    pIconAtlas.setIcon(name, imageFileName);
}
