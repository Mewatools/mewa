/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include <QGuiApplication>
#include <QSurfaceFormat>
#include <QOpenGLContext>

#include "qmewawindow.h"
#include "mxapplication.h"
#include "rototreeview.h"
#include "rototransform.h"


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


    QMewaWindow glWindow;

    ///////////// setup Mx widget ////////////
    RotoTreeView *treeView =  new RotoTreeView();
    RotoTransform &root = treeView->pRootTransform;
    RotoTreeItem *shapeAtRoot = new RotoShape( &root );
    RotoTransform *transform = new RotoTransform( &root );
    RotoTreeItem *childShape = new RotoShape( transform );


    MxApplication::instance()->setMainWidget(treeView);
    /////////////////////////////////////////

    //glWindow.showMaximized();
    glWindow.setMinimumSize( QSize(300, 300) );
    glWindow.showNormal();

    return app.exec();
}