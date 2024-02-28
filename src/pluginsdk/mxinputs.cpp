/****************************************************************************
** Copyright (C) 2020-2024 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxinputs.h"

MxInputs::MxInputs()
{

}

float MxInputs::time() const
{
    return pTime;
}

float MxInputs::opacity() const
{
    return pOpacity;
}

const MxVector2F& MxInputs::outputSize() const
{
    return pOutputResolution;
}
