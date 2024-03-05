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
    const MxVector2F& outputResolution() const;
    bool isDrawingThumbnail() const;



private:
    friend class QMewaPluginWindow;

    float pTime;
    MxVector2F pOutputResolution;

};

#endif // MXINPUTS_H
