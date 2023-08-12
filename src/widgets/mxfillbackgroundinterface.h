/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXFILLBACKGROUNDINTERFACE_H
#define MXFILLBACKGROUNDINTERFACE_H

#include "mxvector.h"


/*!
 * \brief The MxFillBackgroundInterface class
 * Don't forget to add
 *  pAttributes |= MxWidget::HasBackground;
 * in the ctor of the derived class
 */
class MxFillBackgroundInterface
{
public:

    // fill rect with background color
     virtual const MxVector4UC& backgroundColor() const = 0;


};

#endif
