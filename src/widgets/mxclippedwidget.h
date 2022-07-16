/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXCLIPPEDWIDGET_H
#define MXCLIPPEDWIDGET_H

#include "mxrect.h"
#include "mxlist.h"

class MxWidget;


struct MxClippedWidget
{
    MxWidget *widget;
    MxRectF clipRect;
};


typedef MxList<MxClippedWidget, MxPodInitializer<MxClippedWidget> > MxClippedWidgetList;


#endif // MXCLIPPEDWIDGET_H
