/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXBUTTON_H
#define MXBUTTON_H

#include "mxdebug.h"
#include "mxwidget.h"

class MxEventHandler;


/*! 
 \class MxButton
 \brief Base class for buttons
 \ingroup widgets
 
 \sa MxTextButton
 */
class MxButton : public MxWidget
{
public:
    MxButton();
    virtual ~MxButton();

    void setCheckable( bool checked );
    bool isChecked() const;
    void setChecked( bool checked );
    void setEnabled( bool enable );
    bool isEnabled() const;

    void setClickedReceiver( MxEventHandler *receiver, int msg );

    virtual void mousePressEvent( MxMouseEvent &event );
    virtual void mouseMoveEvent( MxMouseEvent &event );
    virtual void mouseReleaseEvent( MxMouseEvent &event );

protected:
    virtual void enterEvent();
    virtual void leaveEvent();

    enum ButtonDrawState { IdleState, UnderMouseState, DraggingOutsideState, PressedState };

    ButtonDrawState state() const;




    // private
    ButtonDrawState  pState;
    bool pOutsideButton;

    bool pIsChecked;
    bool pIsEnabled;

public: // needs to be public because FileBrowser needs to access it
        bool pIsCheckable;
    MxEventHandler *pClickedReceiver;
    int pClickedMsg;
};


#endif

