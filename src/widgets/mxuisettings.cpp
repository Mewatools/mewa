/****************************************************************************
** Copyright (C) 2020-2023 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxuisettings.h"

MxUiSettings::MxUiSettings()
{

}

MxUiSettings::~MxUiSettings()
{

}

/*! Returns 1 on desktop, higher values on mobile devices */
float MxUiSettings::pixelRatio() const
{
    return 1.0f;
}

/*! Returns the maximum text height size, in pixels, of the default font.
 Used to calculate the height of widgest that contain text, such as buttons. */
float MxUiSettings::textHeight() const
{
    return 22.0f;
}

