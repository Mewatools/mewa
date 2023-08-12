/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXLAYOUT_H
#define MXLAYOUT_H

#include "mxdebug.h"
#include "mxwidget.h"



/*!
    \class MxLayout
    \brief The MxLayout class is the base class for layout widgets.
    \ingroup widgets

    MxLayout is the base class for layout widgets.
    It holds a list of widgets to wich it sets its size and position.
    It also forwards the draw calls to its child widgets.

    To create a custom layout derive the new layout class from MxLayout and
    overwrite the sizeChanged() function to set the child widgets positions and sizes.
    
 \TODO add virtual member for next focus when pressing tab.
 
    \sa MxLinearLayout

*/
class MxLayout : public MxWidget
{
public:
    MxLayout();
    virtual ~MxLayout();

    virtual int count() const;
    virtual MxWidget* widgetAt( int index ) const;

    virtual bool hasChildren() const { return true; }
    MxWidget* itemAt( const MxVector2F &pos );


    virtual void paint(MxPainter &painter );
    virtual void render(MxRenderer &renderer);

    /*! Called on MxWidget::setSize(). */
    virtual void sizeChanged() = 0;

    virtual void mousePressEvent( MxMouseEvent &event );
    virtual void mouseMoveEvent( MxMouseEvent &event );
    virtual void mouseReleaseEvent( MxMouseEvent &event );

    virtual void dirtyCollidingWidgets(const MxRectF &collidingRect);

    virtual void collectDirtyWidgets( MxWidgetList &list, bool clean);
    virtual void resetDirtyFlag();

    MxWidgetList & childWidgets();

    //! Set the this layout has the parent of the child widgets
    virtual void setChildWidgetsAndParent( MxWidgetList & widgets );


protected:

 MxList<MxWidget*, MxPodInitializer<MxWidget*> > pChildsList;


};



#endif

