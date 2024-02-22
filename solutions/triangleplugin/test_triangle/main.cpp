/****************************************************************************
** Copyright (C) 2020-2024 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include <QGuiApplication>
#include <QSurfaceFormat>
#include <QOpenGLContext>

#include "qmewapluginwindow.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);

    // Request OpenGL 3.3 core or OpenGL ES 3.0.
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
        qDebug("Requesting 3.3 core context");
        fmt.setVersion(3, 3);
        fmt.setProfile(QSurfaceFormat::CoreProfile);
    } else {
        qDebug("Requesting 3.0 context");
        fmt.setVersion(3, 0);
    }

    QSurfaceFormat::setDefaultFormat(fmt);


    QMewaPluginWindow window;


    int windowWidth = 640;
    int windowHeight = 480;
    window.resizeGL( windowWidth, windowHeight );
    //window.showMaximized();
    window.setMinimumSize( QSize(windowWidth, windowHeight) );
    window.showNormal();

    return app.exec();
}
