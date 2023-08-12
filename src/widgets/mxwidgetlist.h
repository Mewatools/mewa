/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXWIDGETLIST_H
#define MXWIDGETLIST_H

#include "mxlist.h"


class MxWidget;

////////////////// typedefs //////////////////////////

typedef MxList<MxWidget*, MxPodInitializer<MxWidget*> > MxWidgetList;
///////////////////////////////////////////////////////

#endif // MXWIDGETLIST_H
