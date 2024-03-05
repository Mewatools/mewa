/****************************************************************************
** Copyright (C) 2020-2024 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef QMEWAPLUGINWINDOW_H
#define QMEWAPLUGINWINDOW_H

#include <QOpenGLWindow>

#include "mxguirenderer.h"

class MxPlugin;



class QMewaPluginWindow : public QOpenGLWindow
{

public:
    QMewaPluginWindow();
    virtual ~QMewaPluginWindow();

    void setPlugin(MxPlugin *plugin);

    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

    virtual void mousePressEvent( QMouseEvent * event );
    virtual void mouseMoveEvent( QMouseEvent * event );
    virtual void mouseReleaseEvent( QMouseEvent * event );





private:
    MxPlugin *pPlugin;
    MxGuiRenderer pRenderer;

    bool pFirstTime;
};

#endif
