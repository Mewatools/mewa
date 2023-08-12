/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxguirenderer.h"
#include "mxthemecolors.h"


MxGuiRenderer::MxGuiRenderer()
{
        pIconAtlas = NULL;
}

MxGuiRenderer::~MxGuiRenderer()
{}



void MxGuiRenderer::initialize()
{
    pIconProgram.init( this );

    MxVector4F windowColor = MxThemeColors::clearColor;
    glClearColor(windowColor[0], windowColor[1], windowColor[2], 1.0f);
    enableDepthTest( false );

    MxRenderer::initialize();
}

void MxGuiRenderer::setViewportToWindow()
{
setViewport(0,0, pScreenSize.width(), pScreenSize.height());
}

MxVectorProgram * MxGuiRenderer::setVectorProgram()
{
    if( ! pVectorProgram.isInitialized() )
    {
        pVectorProgram.init(this);
    }
    setProgram( &pVectorProgram );
    return &pVectorProgram;
}

MxIconProgram * MxGuiRenderer::setIconProgram()
{
    setProgram( &pIconProgram );
    return &pIconProgram;
}

MxBuffer* MxGuiRenderer::getTemporaryBuffer( int sizeEstimate )
{
    int count = pReusableMem.size();
    for(int i=0; i<count; ++i)
    {
        // \TODO find the best size fit
        ReusableBuffer &entry = pReusableMem[i];
        if(entry.inUse == false)
        {
            Q_ASSERT ( entry.buffer.size() == 0 );
            entry.inUse = true;
            return &(entry.buffer);
        }
    }

    ReusableBuffer *newEntry = pReusableMem.appendAndGet();
    newEntry->buffer.reserveForAppend( sizeEstimate );
    newEntry->inUse = true;
    return &(newEntry->buffer);
}



void MxGuiRenderer::renderEnd()
{
    MxRenderer::renderEnd();

    int count = pReusableMem.size();
    for(int i=0; i<count; ++i)
    {
        ReusableBuffer &bufferEntry = pReusableMem[i];
        bufferEntry.buffer.clear();
        bufferEntry.inUse = false;
    }
}
