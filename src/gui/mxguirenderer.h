/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXGUIRENDERER_H
#define MXGUIRENDERER_H

#include "mxbuffer.h"
#include "mxrenderer.h"
#include "mxvectorprogram.h"
#include "mxiconprogram.h"



class MxAbstractAtlas;


class MxGuiRenderer : public MxRenderer
{
public:
    MxGuiRenderer();
    virtual ~MxGuiRenderer();

    virtual void initialize();

    void setViewportToWindow();

    // shader programs
    MxVectorProgram * setVectorProgram();
    MxIconProgram * setIconProgram();


    //! The returned buffer is automatically deleted after render.
    MxBuffer *getTemporaryBuffer( int sizeEstimate = 1024 );
    virtual void renderEnd();

    MxAbstractAtlas *pIconAtlas;
    

private:
    MxVectorProgram pVectorProgram;
    MxIconProgram pIconProgram;


    struct ReusableBuffer
       {
           MxBuffer buffer;
           bool inUse;
       };
     // \TODO fine tune list allocation
    MxList<ReusableBuffer, MxClassInitializer<ReusableBuffer>, 32> pReusableMem;
};

#endif
