/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "teststoreview.h"
#include "mxpainter.h"
#include "mxthemecolors.h"
#include "mxmouseevent.h"

TestStoreView::TestStoreView()
{
    pIsMousePressed = false;
    pButtonUnderMouse = -1;

    int i=0;
    pAddons[i].pTestValues[0] = false; // isInstalled
    pAddons[i].pTestValues[1] = true; // hasUpdateAvailable
    pAddons[i].pTestValues[2] = false; // isInstalling
    pAddons[i].pTestValues[3] = false; // isUpdatingScore
    pAddons[i].pTestValues[4] = true; // userLike
}

TestStoreView::~TestStoreView()
{
}


void TestStoreView::mousePressEvent( MxMouseEvent &event )
{
    event.accept(this);

    MxVector2F widgetSize = size();
    int mouseX = (int)event.globalPos().x();
    int buttonColumn = (int)(widgetSize.width() / 3.0f);
    int buttonIndex = mouseX / buttonColumn;
    pButtonUnderMouse = (ButtonName)buttonIndex;
    pIsMousePressed = true;
    update();
}

void TestStoreView::mouseMoveEvent( MxMouseEvent &event )
{
    MxVector2F widgetSize = size();
    int mouseX = (int)event.globalPos().x();
    int buttonColumn = (int)(widgetSize.width() / 3.0f);
    int buttonIndex = mouseX / buttonColumn;
    pButtonUnderMouse = (ButtonName)buttonIndex;
    update();
}

void TestStoreView::mouseReleaseEvent( MxMouseEvent &event )
{
    pIsMousePressed = false;
    pButtonUnderMouse = -1;
    update();
}


void TestStoreView::paint( MxPainter &painter )
{
    MxVector2F rowSize = size();
    MxRectF buttonsArea(rowSize);

    // fill window with background color
    MxVectorDraw &p =  painter.vectorDraw();
    p.fillRect( buttonsArea, MxThemeColors::windowBackground );

    qDebug("pButtonUnderMouse = %d, pIsMousePressed = %d", pButtonUnderMouse, pIsMousePressed);
    drawButtons( buttonsArea, painter , pAddons[0], pButtonUnderMouse, pIsMousePressed);
}

float TestStoreView::rowHeight() const
{
    return buttonHeight() + margin();
}
