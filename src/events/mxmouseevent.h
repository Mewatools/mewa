/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXMOUSEEVENT_H
#define MXMOUSEEVENT_H

#include "mxvector.h"
#include "mxmouseeventabstraction.h"


class MxWidget;




class MxMouseEvent
{
public:
    enum MouseButton
    {
        NoButton = 0,
        LeftButton = MX_MOUSEBUTTON_LEFT,
        MiddleButton = MX_MOUSEBUTTON_MIDDLE,
        RightButton = MX_MOUSEBUTTON_RIGHT
    };

    enum Modifier
    {
        NoModifier = 0x0,
        ShiftModifier = MX_MODIFIER_SHIFT,
        ControlModifier = MX_MODIFIER_CONTROL
    };

    MxMouseEvent();
    virtual ~MxMouseEvent();

    // \TODO rename to buttons (because its a mask)
    MouseButton button() const;

    unsigned int modifiers() const;

    /*! If the event was accepted it will return the offset, in mouse coordinates,
    relative to mouse press event (where the event was accepted).
    Use MxWidget mapFromGlobal() to calculate pos relative to the widget.*/
    const MxVector2F &pos() const;

    /*! Setting the accept parameter indicates that the event receiver wants the event.
       The item that accepted the event will receive the move and release events.

       Notes::
       - call accept() in the end of mousePressEvent
       - You only need to call accept() in the mousePressEvent
       - Use global coordinates over local in mouseMoveEvent and mouseReleaseEvent if
       the event was accepted.
    */
    void accept( MxWidget *item );
    MxWidget* acceptedItem() const;
    bool isAccepted() const;

    /*! Returns the global mouse pos of when pressed. Usefull for drag offset calculations. */
    MxVector2F pressedGlobalPos() const;

    const MxVector2F & globalPos() const { return pGlobalPos; }




    ////////// Internal API ///////
    void setPos( const MxVector2F &pos ){ pPos = pos; }

    MouseButton pButton;
    unsigned int pModifiers;
    MxVector2F pMousePressedPos;
    MxWidget *pAcceptedItem;
    MxVector2F pPos;
    MxVector2F pGlobalPos;
    ////////////////////////////////


};

#endif

