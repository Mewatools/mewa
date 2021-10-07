/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef TESTSTOREVIEW_H
#define TESTSTOREVIEW_H

#include "mxstoreview.h"
#include "mxstoreaddon.h"

#define TEST_ADDON_COUNTER 1

class TestStoreView : public MxStoreView
{

public:
    TestStoreView();
    ~TestStoreView();


    virtual void mousePressEvent( MxMouseEvent &event );
    virtual void mouseMoveEvent( MxMouseEvent &event );
    virtual void mouseReleaseEvent( MxMouseEvent &event );

    virtual void paint( MxPainter &painter );


private:
    float rowHeight() const;

    class TestAddon : public MxStoreAddon
    {
    public:
        virtual bool isInstalled() const { return pTestValues[0]; }
        virtual bool hasUpdateAvailable() const { return pTestValues[1]; }
        virtual int totalLikes() const { return 5; }
        virtual bool isInstalling() const { return pTestValues[2]; }
        virtual bool isUpdatingScore() const { return pTestValues[3]; }
        virtual bool userLike() const { return pTestValues[4]; }

        bool pTestValues[5];

    };
    TestAddon pAddons[TEST_ADDON_COUNTER];

    int pButtonUnderMouse;
    bool pIsMousePressed;
};

#endif
