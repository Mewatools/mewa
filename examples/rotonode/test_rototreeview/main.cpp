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
//#include "mxiconatlas.h"


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


    QMewaWindow *glWindow = new QMewaWindow();

    // set icons
    glWindow->setIcon(MxThemeIcons::Inverted, "../icons-ldpi/Inverted.png");
    glWindow->setIcon(MxThemeIcons::Locked, "../icons-ldpi/Locked.png");
    glWindow->setIcon(MxThemeIcons::Unlocked, "../icons-ldpi/Unlocked.png");
    glWindow->setIcon(MxThemeIcons::NotInverted, "../icons-ldpi/NotInverted.png");
    glWindow->setIcon(MxThemeIcons::Visible, "../icons-ldpi/Visible.png");
    glWindow->setIcon(MxThemeIcons::NotVisible, "../icons-ldpi/NotVisible.png");


    // setup Mx widget
    RotoTreeView *treeView =  new RotoTreeView();
    RotoTransform &rootTransform = treeView->pRootTransform;
    RotoTreeItem *child1 = new RotoShape( &rootTransform );
    RotoTransform *transform = new RotoTransform( &rootTransform );
    RotoTreeItem *child2 = new RotoShape( transform );
    glWindow->setMainWidget(treeView);




    glWindow->setMinimumSize( QSize(300, 300) );
    glWindow->showNormal();

    return app.exec();
}
