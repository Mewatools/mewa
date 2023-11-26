/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef QMEWAWINDOW_H
#define QMEWAWINDOW_H

#include <QWindow>

#include "mxiconatlas.h"
#include "mxapplication.h"


class MxWidget;



class QMewaWindow : public QWindow
{
    Q_OBJECT
public:
    explicit QMewaWindow(QWindow *parent = nullptr);
     ~QMewaWindow();



    virtual void initialize();


    void mousePressEvent( QMouseEvent * event ) override;
    void mouseMoveEvent( QMouseEvent * event ) override;
    void mouseReleaseEvent( QMouseEvent * event ) override;

    void setMainWidget( MxWidget *widget );
    void setIcon( MxThemeIcons::IconName name, const char *imageFileName );


    void setAnimating(bool animating);


public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent *event) override;

    void exposeEvent(QExposeEvent *event) override;

private:
    bool m_animating = false;

    QOpenGLContext *m_context = nullptr;

    MxIconAtlas pIconAtlas;
    MxApplication pApp;
};

#endif
