/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef ROTOTREEVIEW_H
#define ROTOTREEVIEW_H


#include "mxwidget.h"
#include "rototransform.h"



class RotoTreeView : public MxWidget
{
public:
    RotoTreeView();
    virtual ~RotoTreeView();


    RotoShape* newRotoShape();
    void removeShape(  RotoShape *shape );

    enum ButtonName
    {
        InvertButton = 0,
        ViewButton,
        LockButton,

        ButtonCount // for counting purposes only
    };

    virtual MxVector2F sizePolicy();
    virtual void sizeChanged();

    virtual void mousePressEvent( MxMouseEvent &event );
    virtual void mouseMoveEvent( MxMouseEvent &event );
    virtual void mouseReleaseEvent( MxMouseEvent &event );
    virtual void paint( MxPainter &painter );



    RotoTransform pRootTransform;

private:

    void clearAllSelections();
    RotoTransform &rootTransform();
    int rowHeight() const;
    static float buttonWidth();
    void selectRow(int row , float mouseX);
    void setHoveringRow( int row );
    void setHoveringButton( int buttonIndex );
    int buttonAtPos( float mouseX );


    RotoTreeItem::DrawData pDrawData;

    int pRowCount;


};

#endif
