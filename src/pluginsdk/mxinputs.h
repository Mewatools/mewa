/****************************************************************************
** Copyright (C) 2020-2024 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXINPUTS_H
#define MXINPUTS_H

#include "mxvector.h"


class MxInputs
{
public:
    MxInputs();

    float time() const;
    float opacity() const;


private:
    friend class QMewaPluginWindow;

    float pTime;
    float pOpacity;
    MxVector2F pOutputResolution;

};

#endif // MXINPUTS_H
