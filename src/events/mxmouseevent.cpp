/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxmouseevent.h"
#include "mxwidget.h"

#include <math.h> // sqrt


MxMouseEvent::MxMouseEvent()
{
    pAcceptedItem = NULL;
}

MxMouseEvent::~MxMouseEvent()
{}

MxMouseEvent::MouseButton MxMouseEvent::button() const
{
    return pButton;
}

unsigned int MxMouseEvent::modifiers() const
{
    return pModifiers;
}

const MxVector2F & MxMouseEvent::pos() const
{
    Q_ASSERT_X( ! isAccepted() , "MxMouseEvent::pos()", "event was not accepted. Map it from global coordinates.");
    return pPos; // layout mapped it to local
}



/*! Accept a mouse event inside a QxWidget::mousePressEvent() function. */
void MxMouseEvent::accept( MxWidget *item )
{
    pAcceptedItem = item;
    pPos = pGlobalPos;
}

MxWidget* MxMouseEvent::acceptedItem() const
{
    return pAcceptedItem;
}

bool MxMouseEvent::isAccepted() const
{
    return pAcceptedItem != 0;
}

MxVector2F MxMouseEvent::pressedGlobalPos() const
{
    return pMousePressedPos;
}


bool MxMouseEvent::grabLeaveEvent(MxWidget *widget)
{

    return false;
}

void MxMouseEvent::checkLeaveEvent( const MxVector2F &globalPos )
{

}

