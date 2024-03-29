/****************************************************************************
** Copyright (C) 2020-2023 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXWIDGET_H
#define MXWIDGET_H


#include "mxrect.h"
#include "mxvector.h"
#include "mxwidgetlist.h"
#include "mxclippedwidget.h"


class MxPainter;
class MxRenderer;
class MxMouseEvent;
class MxWheelEvent;
class MxApplication;


class MxWidget
{
public:
    enum PropertyFlag {
        DirtyFlag = 0x01,
        HasBackground = 0x400, // used to draw child background without the need to ask parent to redraw itself
        HasTransparency = 0x800, // set it to popup widgets when these have tranparency and need background to be redrawn
        Layout = 0x40000,
        ClipPaint = 0x80000, //! clip painter drawing on itself and all its children

    };

    MxWidget();
    virtual ~MxWidget();


    void getRect( MxRectF *rect ) const;

    virtual void setPos( const MxVector2F &pos );
    virtual void setSize( const MxVector2F &size );
    const MxVector2F & pos() const;
    MxVector2F & rpos();
    const MxVector2F & size() const;
    MxWidget* parent() const;
    virtual void setParent( MxWidget *widget );
    void getGlobalPos(MxVector2F *pos ) const;
    virtual bool hasChildren() const { return false; }
    /*!
        needs to be overwritten if has child widgets
    */
    virtual MxWidget* itemAt( const MxVector2F &globalPos );

    /*! Negative values mean minimum size;
     *  Positive values mean fixed size
     *  Zero value means expand taking over all available space
     */
    virtual MxVector2F sizePolicy();


    MxVector2F mapFromGlobal(const MxVector2F &globalPos ); //const;
    // Fills \a pos with this widget screen position
    void mapToGlobal( MxVector2F *pos  ) const;

    /*! Requests a redraw. */
    void update();


    /*! Called whenever the the widget changed its size. */
    virtual void sizeChanged(){}
    //! This function is called after setParent(). Its used to initialize fonts and style stuff.
    virtual void dirtyCollidingWidgets(const MxRectF &collidingRect );

    virtual void mousePressEvent( MxMouseEvent &event );
    virtual void mouseMoveEvent( MxMouseEvent &event ); // store this item as under mouse item
    /*! Its only called if the event was accepted. */
    virtual void mouseReleaseEvent( MxMouseEvent &event );

    virtual void leaveEvent(){}

    /*! Dont forget to accept the event to win the focus. */
    virtual void wheelEvent( MxWheelEvent& ){}

    /*! The default implementation propagates the events to the parent. */
    virtual void keyEvent( int key, int modifiers );
    virtual void textInputEvent( const char *text, int count );


    /*! Use by widgets to draw geometries and text. See MxPainter interface. */
    virtual void paint( MxPainter &/*painter*/ ){}
    /*! Implement it to use a custom MxShaderProgram. */
    virtual void render( MxRenderer &/*renderer*/ ){}

    virtual void collectDirtyWidgets( MxWidgetList &list, bool clean );
    virtual void resetDirtyFlag();
    virtual bool isDirty() const;

    MxWidget* firstParentWithAttribute( int flag ) const;

    MxWidget* rootParent() const;
    virtual void childNeedsUpdate();

    const MxVector4UC& parentBackgroundColor();

    static MxApplication* application();



    MxWidget *pParent;
    int pAttributes;
    MxVector2F pSize;
    MxVector2F pPos;

    static MxApplication *sApplication;
};




#endif


