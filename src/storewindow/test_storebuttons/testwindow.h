/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QOpenGLWindow>


class TestWindow : public QOpenGLWindow
{
   
public:
    TestWindow();
    ~TestWindow();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent( QMouseEvent * event );
    void mouseMoveEvent( QMouseEvent * event );
    void mouseReleaseEvent( QMouseEvent * event );
   /* void wheelEvent(QWheelEvent * event);
    void keyPressEvent( QKeyEvent *event );
    */


};

#endif
