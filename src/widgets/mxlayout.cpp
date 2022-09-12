/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxlayout.h"
#include "mxmouseevent.h"
#include "mxpainter.h"




MxLayout::MxLayout()
{
    pAttributes |= MxWidget::Layout;
}


MxLayout::~MxLayout()
{}



int MxLayout::count() const
{
    return pChildsList.size();
}

MxWidget* MxLayout::widgetAt( int index ) const
{
    return pChildsList.at(index);
}



MxWidget* MxLayout::itemAt( const MxVector2F &pos )
{
    int listSize = count();
    for(int i=0; i < listSize; ++i)
    {
        MxWidget *item = widgetAt(i);
        MxRectF itemRect;
        itemRect.setRectFromPointAndSize(item->pos(), item->size());
        if( itemRect.contains(pos) )
        {
            return item;
        }
    }

    return NULL;
}


void MxLayout::paint( MxPainter &painter )
{
    int listSize = count();
    for(int i=0; i<listSize; ++i)
    {
        MxWidget *item = widgetAt(i);
        painter.pTranslation += item->pos();
        item->paint( painter );
        painter.pTranslation -= item->pos();
    }
}

void MxLayout::render( MxRenderer &renderer )
{
    int listSize = count();
    for(int i=0; i<listSize; ++i)
    {
        MxWidget *item = widgetAt(i);
        item->render(renderer);
    }
}

void MxLayout::mousePressEvent( MxMouseEvent &event )
{
    const MxVector2F currMousePos = event.pos();
    MxWidget *item = itemAt(currMousePos);
    if( item )
    {
        event.setPos(currMousePos - item->pos());
        item->mousePressEvent(event);
    }
}

void MxLayout::mouseMoveEvent( MxMouseEvent &event )
{
    const MxVector2F currMousePos = event.pos();
    int listSize = count();
    for(int i=0; i < listSize; ++i)
    {
        MxWidget *item = widgetAt(i);
        MxRectF itemRect;
        itemRect.setRectFromPointAndSize(item->pos(), item->size());
        if( itemRect.contains(event.pos()) )
        {
            event.setPos(currMousePos - item->pos());
            item->mouseMoveEvent(event);
            return;
        }
    }

}

void MxLayout::mouseReleaseEvent( MxMouseEvent &event )
{
    const MxVector2F currMousePos = event.pos();
    int listSize = count();
    for(int i=0; i<listSize; ++i)
    {
        MxWidget *item = widgetAt(i);
        MxRectF itemRect;
        itemRect.setRectFromPointAndSize(item->pos(), item->size());
        if( itemRect.contains(event.pos()) )
        {
            event.setPos(currMousePos - item->pos());
            item->mouseReleaseEvent(event);
            break;
        }
    }
}

void MxLayout::dirtyCollidingWidgets(const MxRectF &collidingRect )
{
    MxRectF thisRect;
    getRect( &thisRect );
    if( !isDirty() && collidingRect.intersects(thisRect) )
    {
        MxRectF mappedRect = collidingRect.translated(-pos().x(), -pos().y());
        int listSize = count();
        for(int i=0; i < listSize; ++i)
        {
            MxWidget *item = widgetAt(i);
            item->dirtyCollidingWidgets(mappedRect);
        }
    }
}


void MxLayout::collectDirtyWidgets( MxClippedWidgetList &list, const MxRectF &clipRect, bool clean )
{
Q_ASSERT( ! (pAttributes & MxWidget::ClipPaint) );

    if( isDirty() ) {
        MxClippedWidget *clip = list.appendAndGet();
        clip->widget = this;
        clip->clipRect = clipRect;

        if (clean) {
            resetDirtyFlag();
        }
        return;
    }

    int listSize = count();
    for(int i=0; i<listSize; ++i)
    {
        MxWidget *item = widgetAt(i);
        item->collectDirtyWidgets(list, clipRect, clean);
    }
}

void MxLayout::resetDirtyFlag()
{
    MxWidget::resetDirtyFlag();
    int listSize = count();
    for(int i=0; i<listSize; ++i)
    {
        MxWidget *item = widgetAt(i);
        item->resetDirtyFlag();
    }
}

MxWidgetList &MxLayout::childWidgets()
{
    return pChildsList;
}

void MxLayout::setChildWidgetsAndParent( MxWidgetList & widgets )
{
    pChildsList = widgets;
    for(int i=0; i < count(); ++i)
    {
        widgetAt(i)->setParent(this);
    }
}
