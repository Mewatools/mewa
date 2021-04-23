/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include <QMouseEvent>

#include "testwindow.h"
#include "mxrect.h"
#include "mxapplication.h"

TestWindow::TestWindow()
    : QOpenGLWindow(QOpenGLWindow::PartialUpdateBlit)
{

}

TestWindow::~TestWindow()
{
}


void TestWindow::initializeGL()
{
    MxApplication::instance()->pRenderer.initializeOpenGLFunctions();
    
    MxApplication::instance()->initializeGL();
}

void TestWindow::resizeGL(int w, int h)
{
    MxApplication::instance()->onResizeWindow( w, h );
}

void TestWindow::paintGL()
{
    MxApplication::instance()->paintGL();
}





void TestWindow::mousePressEvent( QMouseEvent * event )
{
    QPoint p = event->pos();
    MxApplication::instance()->onMousePress(p.x(), p.y(), event->button(), event->modifiers() | event->buttons() );
    //if( isRedrawNeeded() )
    {
        update();
    }
}

void TestWindow::mouseMoveEvent( QMouseEvent * event )
{
    QPoint p = event->pos();
    MxApplication::instance()->onMouseMove(p.x(), p.y(), event->modifiers() | event->buttons() );
    update();
}

void TestWindow::mouseReleaseEvent( QMouseEvent * event )
{
    QPoint p = event->pos();
    MxApplication::instance()->onMouseRelease(p.x(), p.y());
    update();
}


/*
void TestWindow::wheelEvent(QWheelEvent * event)
{
    QPoint p = event->pos();

#if QT_VERSION < 0x050000
    int numDegrees = event->delta() ;
    MxApplication::instance()->onWheelEvent( p.x(), p.y(), true, 0, numDegrees );
#else
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() ;

    if (!numPixels.isNull()) {
        MxApplication::instance()->onWheelEvent( p.x(), p.y(), numPixels.x(), numPixels.y() );
    } else if (!numDegrees.isNull()) {
        MxApplication::instance()->onWheelEvent( p.x(), p.y(), numDegrees.x(), numDegrees.y() );
    }
#endif

    event->accept();

update();
}


void TestWindow::keyPressEvent( QKeyEvent *event )
{
    QString keyText = event->text();
    QChar c = keyText.at(0);
    if( c.isLetterOrNumber() ) {
        QByteArray ba = keyText.toLatin1();
        const char *str = ba.data();
        MxApplication::instance()->onTextInput( str, ba.size() );


    } else {
        MxApplication::instance()->onKeyPress( event->key(), event->modifiers());
    }

    update();
}
*/
