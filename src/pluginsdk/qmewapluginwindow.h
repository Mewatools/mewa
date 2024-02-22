/****************************************************************************
** Copyright (C) 2020-2024 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef QMEWAPLUGINWINDOW_H
#define QMEWAPLUGINWINDOW_H

#include <QOpenGLWindow>

#include "mxrenderer.h"
#include "triangleplugin.h"



class QMewaPluginWindow : public QOpenGLWindow
{
   
public:
    QMewaPluginWindow();
    virtual ~QMewaPluginWindow();

   virtual void initializeGL();
   virtual void resizeGL(int w, int h);
   virtual void paintGL();

   virtual void mousePressEvent( QMouseEvent * event );
   virtual void mouseMoveEvent( QMouseEvent * event );
   virtual void mouseReleaseEvent( QMouseEvent * event );


    TrianglePlugin pPlugin;


private:
    MxRenderer pRenderer;

    bool pFirstTime;
};

#endif
