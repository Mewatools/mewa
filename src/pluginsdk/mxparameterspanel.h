/****************************************************************************
** Copyright (C) 2020-2024 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXPARAMETERSPANEL_H
#define MXPARAMETERSPANEL_H


class MxParameter;


class MxParametersPanel
{
public:
    MxParametersPanel();



    void appendFloat(const char *name, MxParameter *param, float min, float max, float step );


};

#endif // MXPARAMETERSPANEL_H
