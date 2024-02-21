/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef SHAPELIST_H
#define SHAPELIST_H

#include "rotoshape.h"

typedef  MxList< RotoTreeItem* , MxPodInitializer<RotoTreeItem*> , 8> ShapeList;

#endif // SHAPELIST_H
