/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXTEXTBUTTON_H
#define MXTEXTBUTTON_H

#include "mxdebug.h"
#include "mxbutton.h"
#include "mxstring.h"


class MxTextButton : public MxButton
{
public:
    MxTextButton();
    virtual ~MxTextButton();

    void setText(const MxString &text );

    virtual MxVector2F sizePolicy();

    virtual void paint( MxPainter &painter );



    MxString pText;


};

#endif
