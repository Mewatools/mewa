/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxbutton.h"
#include "mxrect.h"
#include "mxmouseevent.h"
#include "mxeventhandler.h"




MxButton::MxButton()
    :pIsChecked(false),
      pIsEnabled(true),
       pIsCheckable(false),
      pClickedReceiver(NULL)
{
    pState = IdleState;


}


MxButton::~MxButton()
{
}



void MxButton::setCheckable( bool checked )
{
    pIsCheckable = checked;
    pIsChecked = false;
}

bool MxButton::isChecked() const
{
    return pIsChecked;
}

void MxButton::setChecked( bool checked )
{
    if( pIsChecked != checked ) {
    pIsChecked = checked;
    update();
    }
}

 void MxButton::setEnabled( bool enable )
 {
     if( pIsEnabled != enable ) {
     pIsEnabled = enable;
     update();
     }
 }

  bool MxButton::isEnabled() const
  {
      return pIsEnabled;
  }

void MxButton::setClickedReceiver( MxEventHandler *receiver, int msg )
{
    pClickedReceiver = receiver;
    pClickedMsg = msg;
}

void MxButton::mousePressEvent( MxMouseEvent &event )
{
        event.accept( this );

        if( ! pIsEnabled ) {
            return;
        }

	pState = PressedState;
        update();
}

void MxButton::mouseMoveEvent( MxMouseEvent &event )
{
    if( ! event.isAccepted() ) {
        if( event.grabLeaveEvent(this) ) {
         this->enterEvent();
        }
    }
}

void MxButton::mouseReleaseEvent( MxMouseEvent &event )
{
        if( pState == DraggingOutsideState )
        {
            pState = IdleState;
            update();
        }
        else if( state() == PressedState )
        {
            // signal
            pIsChecked = !pIsChecked;

            Q_ASSERT( NULL != pClickedReceiver );
            pClickedReceiver->eventReceived(pClickedMsg);

            pState = UnderMouseState; // this state is overwritten in leave event if released out of item
            update();
        }

}

void MxButton::enterEvent()
{
    if( ! pIsEnabled ) {
        return;
    }

	pState = UnderMouseState;
        update();
}

void MxButton::leaveEvent()
{
    if( ! pIsEnabled ) {
        return;
    }

    if( pState == PressedState ) {
        pState = DraggingOutsideState;
    } else {
        pState = IdleState;
        update();
    }
}

MxButton::ButtonDrawState MxButton::state() const
{
    return pState;
}

