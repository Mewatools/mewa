/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include <QMouseEvent>
#include <QResizeEvent>

#include "qmewawindow.h"
#include "mxapplication.h"
#include "mxwidget.h"



QMewaWindow::QMewaWindow(QWindow *parent)
    : QWindow(parent)
{
    setSurfaceType(QWindow::OpenGLSurface);
    pApp.init( &pIconAtlas );
}


QMewaWindow::~QMewaWindow()
{
}



void QMewaWindow::initialize()
{
    MxApplication *app = MxWidget::application();
    app->initialize();
    pIconAtlas.loadGL( app->pRenderer );
}

void QMewaWindow::mousePressEvent( QMouseEvent * event )
{
    QPoint p = event->pos();
    MxWidget::application()->onMousePress(p.x(), p.y(), event->button(), event->modifiers() | event->buttons() );
    requestUpdate();
}

void QMewaWindow::mouseMoveEvent( QMouseEvent * event )
{
    QPoint p = event->pos();
    MxWidget::application()->onMouseMove(p.x(), p.y(), event->modifiers() | event->buttons() );
    requestUpdate();
}

void QMewaWindow::mouseReleaseEvent( QMouseEvent * event )
{
    QPoint p = event->pos();
    MxWidget::application()->onMouseRelease(p.x(), p.y());
    requestUpdate();
}

void QMewaWindow::setMainWidget( MxWidget *widget )
{
    MxWidget::application()->setMainWidget( widget );
}

void QMewaWindow::setIcon( MxThemeIcons::IconName name, const char *imageFileName )
{
    pIconAtlas.setIcon(name, imageFileName);
}

void QMewaWindow::renderLater()
{
    requestUpdate();
}

bool QMewaWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Resize:
    {
        QResizeEvent *resizeEvent = dynamic_cast<QResizeEvent*>(event);
        MxWidget *w = MxWidget::application()->mainWidget();
        if(w)
        {
        const QSize &newSize = resizeEvent->size();
        MxWidget::application()->onResizeWindow( newSize.width(), newSize.height() );
        renderNow();
        }
        return true;
    }
    case QEvent::UpdateRequest:
        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void QMewaWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed()) {
        renderNow();
    }
}

void QMewaWindow::renderNow()
{
    if (!isExposed())
        return;

    bool needsInitialize = false;

    if (!m_context) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();

        needsInitialize = true;
    }

    m_context->makeCurrent(this);

    if (needsInitialize) {
        MxApplication *app = MxWidget::application();
        app->pRenderer.initializeOpenGLFunctions();

        initialize();

    }

    MxWidget::application()->onRender();

    m_context->swapBuffers(this);

    if (m_animating)
        renderLater();
}

void QMewaWindow::setAnimating(bool animating)
{
    m_animating = animating;

    if (animating)
        renderLater();
}
