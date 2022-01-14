/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXTHEMECOLORS_H
#define MXTHEMECOLORS_H

#include "mxvector.h"



/*!
 * \brief MxThemeColors is the one place holding all colors used by the UI
 *  Colors used by widgets
 */
namespace MxThemeColors
{
// used in: image view background, and glClearColor
MxVector4F const clearColor = {102.0f/255.0f, 102.0f/255.0f, 102.0f/255.0f, 1.0f};

// used in Store Window
MxVector4UC const windowBackground = {51,51,51,255};

MxVector4UC const darkText = {51,51,51,255};
MxVector4UC const whiteText = {179,179,179,255};

MxVector4UC const mainWindowBackground = {72, 72, 72, 255};
MxVector4UC const nodePanelBackground = {61, 61, 61, 255};
MxVector4UC const outputWindowBackground = {61, 61, 61, 255};
MxVector4UC const dialogWindowBackground = {61, 61, 61, 255};

// used in widget icons
MxVector4UC const iconLight = {179, 179, 179, 128};
MxVector4UC const iconBlue = {62, 149, 249, 128};
}



#endif // MXTHEMECOLORS_H
