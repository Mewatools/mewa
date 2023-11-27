/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxwidget.h"
#include "mxfillbackgroundinterface.h"


MxApplication * MxWidget::sApplication = nullptr;


MxWidget::MxWidget()
    :pParent(NULL), pAttributes(DirtyFlag)
{}

/*! Destroys the widget. */
MxWidget::~MxWidget()
{}


void MxWidget::getRect( MxRectF *rect ) const
{
    *rect = MxRectF( pPos.x(), pPos.x() + pSize.width(), pPos.y(), pPos.y() + pSize.height() );
}

void MxWidget::setPos( const MxVector2F &pos )
{
    pPos = pos;
}

/*! The layout will set the size based on sizePolicy().
 */
void MxWidget::setSize( const MxVector2F &size )
{
    Q_ASSERT(size.width() >= 0.0f && size.height() >= 0.0f);
    pSize = size;
    sizeChanged();
}

/*! This pos is relative to the parent. */
const MxVector2F &MxWidget::pos() const
{
    return pPos;
}

MxVector2F & MxWidget::rpos()
{
    return pPos;
}

/*! Returns the size of this widget.
\sa setSize(). */
const MxVector2F & MxWidget::size() const
{
    return pSize;
}

/*!
Returns the parent of this MxWidget. 
The parent of a MxWidget
is usually an MxLayout where the widget was added.
The parent is set to the widget when the widget is added to a layout.
*/
MxWidget* MxWidget::parent() const
{
    return pParent;
}

/*! Use only in custom layouts. */
void MxWidget::setParent( MxWidget *widget )
{
    pParent = widget;
}

void MxWidget::getGlobalPos( MxVector2F *global_pos ) const
{
    *global_pos = pPos;
    MxWidget *item_parent = pParent;

    if( item_parent == NULL ) {
        return;
    }

    while(item_parent->pParent)
    {
        *global_pos += item_parent->pPos;
        item_parent = item_parent->pParent;
    }
}

MxWidget* MxWidget::itemAt( const MxVector2F &globalPos )
{
    return NULL;
}

MxVector2F MxWidget::sizePolicy()
{
    MxVector2F s(0.0f, 0.0f);
    return s;
}



/*! Maps \a globalPos from global coordinates to local coordinates. */
MxVector2F MxWidget::mapFromGlobal( const MxVector2F &globalPos ) //const
{
    MxVector2F mappedPos = globalPos - pos();
    MxWidget *parent = pParent;
    while( parent->pParent != NULL )
    {
        mappedPos -= parent->pos();
        parent = parent->pParent;
    }

    return mappedPos;
}

/*! Maps \a pos relative to this widget coordinates to a position
  relative to the display. Usefull for drag and drop actions.
*/
void MxWidget::mapToGlobal( MxVector2F *pos ) const
{
    MxVector2F mappedPos = pPos;
    MxWidget *parent = pParent;
    while( parent != NULL )
    {
        mappedPos += parent->pos();
        parent = parent->pParent;
    }

    *pos = mappedPos;
}

void MxWidget::update()
{
    pAttributes |= DirtyFlag;
    childNeedsUpdate();
}

void MxWidget::dirtyCollidingWidgets(const MxRectF &collidingRect)
{
    MxRectF thisRect;
    getRect( &thisRect );
    if( collidingRect.intersects( thisRect ) ) {
        pAttributes |= DirtyFlag;
    }
}

void MxWidget::mousePressEvent( MxMouseEvent &/*event*/ )
{
}

void MxWidget::mouseMoveEvent( MxMouseEvent &event )
{
}

void MxWidget::mouseReleaseEvent( MxMouseEvent &event )
{
}

// propagate the event up to the root widget (MxApplication::mainWidget)
void MxWidget::keyEvent( int key, int modifiers )
{
#if defined(QX_DEBUG)
    if( NULL != pParent ) {
        pParent->keyEvent(key, modifiers);
    }
#else
    Q_ASSERT( NULL != pParent );
    pParent->keyEvent(key, modifiers);
#endif

}

void MxWidget::textInputEvent( const char *text, int count )
{
}


void MxWidget::collectDirtyWidgets( MxWidgetList &list, bool clean )
{
    if( isDirty() ) {
        list.append(this);
        if (clean) {
            resetDirtyFlag();
        }
        return;
    }
}

void MxWidget::resetDirtyFlag()
{
    pAttributes &= ~(DirtyFlag);
}

bool MxWidget::isDirty() const
{
    return (pAttributes & DirtyFlag);
}

MxWidget* MxWidget::firstParentWithAttribute( int flag ) const
{
    MxWidget* parentWidget = parent();
    while( parentWidget != NULL)
    {

        if( parentWidget->pAttributes & flag )
        {
            return parentWidget;
        }
        parentWidget = parentWidget->parent();
    }
    return NULL;
}

MxWidget* MxWidget::rootParent() const
{
    const MxWidget *parent = this;
    while( parent->pParent != NULL && parent->pParent->pParent != NULL )
    {
        parent = parent->pParent;
    }
    return parent->pParent;
}

void MxWidget::childNeedsUpdate()
{

}

const MxVector4UC& MxWidget::parentBackgroundColor()
{
    MxWidget *parentWidget = this;
    if( ! (parentWidget->pAttributes & MxWidget::HasBackground) ) {

        parentWidget = firstParentWithAttribute( MxWidget::HasBackground );
        Q_ASSERT( NULL != parentWidget );
        Q_ASSERT( parentWidget->pAttributes & MxWidget::HasBackground );
    }
    Q_ASSERT( NULL != parentWidget );
    MxFillBackgroundInterface *widgetWithBgd = dynamic_cast<MxFillBackgroundInterface*>(parentWidget);
    const MxVector4UC& backColor = widgetWithBgd->backgroundColor();
    return backColor;
}

MxApplication* MxWidget::application()
{
    Q_ASSERT( nullptr != sApplication );

    return sApplication;
}



