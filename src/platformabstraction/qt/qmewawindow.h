/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef QMEWAWINDOW_H
#define QMEWAWINDOW_H

#include <QOpenGLWindow>


class QMewaWindow : public QOpenGLWindow
{
   
public:
    QMewaWindow();
    ~QMewaWindow();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent( QMouseEvent * event );
    void mouseMoveEvent( QMouseEvent * event );
    void mouseReleaseEvent( QMouseEvent * event );

};

#endif
