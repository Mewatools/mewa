/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QOpenGLWindow>

#include "mxrenderer.h"


class TestWindow : public QOpenGLWindow
{
   
public:
    TestWindow();
    ~TestWindow();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();



    MxRenderer pRenderer;
};

#endif
