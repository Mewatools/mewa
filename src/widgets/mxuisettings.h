/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXUISETTINGS_H
#define MXUISETTINGS_H


class MxUiSettings
{
public:
    MxUiSettings();
    virtual ~MxUiSettings();

    float pixelRatio() const;
    float textHeight() const;
};

#endif // MXUISETTINGS_H
