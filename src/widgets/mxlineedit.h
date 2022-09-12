/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXLINEEDIT_H
#define MXLINEEDIT_H


#include "mxwidget.h"
#include "mxstring.h"



class MxEventHandler;




class MxLineEdit : public MxWidget
{
public:
    MxLineEdit();
    virtual ~MxLineEdit();

    virtual MxVector2F sizePolicy();

    //! Use text() to change the text
    MxString &text();


    void setReadOnly( bool readOnly );


     void setTextChangedReceiver( MxEventHandler *receiver, int msg );


    virtual void paint( MxPainter &painter );

    virtual void mousePressEvent( MxMouseEvent &event );



public:
     virtual void drawText( MxPainter &painter );


     bool pTextChanged;

protected:
     void updateCursorPosArray();
     int updateCursorPosition( float mouseX );


     bool m_blinkStatus; // needed ??
     MxString pText;
     bool pReadOnly;
     int pCursorIndex;
     MxEventHandler *pTextChangedEventReceiver;
     int pTextChangedMsg;
     MxList<float, MxPodInitializer<float> > pGlyphPositions;


};


#endif




