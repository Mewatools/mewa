/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXSTOREVIEW_H
#define MXSTOREVIEW_H

#include "mxwidget.h"

class MxStoreAddon;


class MxStoreView : public MxWidget
{
public:
    MxStoreView();
    virtual ~MxStoreView();


protected:
    void drawButtons(const MxRectF &rect, MxPainter &painter , const MxStoreAddon &entry, int buttonUnderMouse , bool isMousePressed);
    void calcButtonsRects(const MxRectF &rowRect, MxRectF buttonRects[]);
    float buttonHeight() const;

    // margin space around the buttons
    float margin() const;


    enum ButtonName
    {
        InstallButton = 2,
        UpdateButton = 1,
        LikeButton = 0
    };

    float pButtonHeight;
};

#endif // MXSTOREVIEW_H
