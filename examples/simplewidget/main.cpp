/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include <QGuiApplication>
#include <QSurfaceFormat>
#include <QOpenGLContext>

#include "qmewawindow.h"
#include "simplewidget.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);

    QMewaWindow window;
    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    window.setAnimating(true);

    ///////////// setup Mx widget ////////////
    SimpleWidget *canvas =  new SimpleWidget();
    window.setMainWidget(canvas);
    /////////////////////////////////////////


    return app.exec();
}
