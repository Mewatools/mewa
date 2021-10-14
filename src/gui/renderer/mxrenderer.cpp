/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxrenderer.h"
#include "mxthemecolors.h"


MxRenderer::MxRenderer()
{
    pCurrBlend = NoBlending;
    pDepthTestEnabled = false;
    pCurrentTexture = 99999; //zero is reserved
    pCurrentActiveTextureSlot = GL_TEXTURE0;
    pVboList.reserve(16);
}

void MxRenderer::discardGLResources()
{
    pCurrBlend = NoBlending;
    pColorWheelEffect.discardGLResources();
    pDepthTestEnabled = false;
    pCurrentTexture = 99999; //zero is reserved
    pCurrentActiveTextureSlot = GL_TEXTURE0;

    pIconProgram.discardGLResources();
    pVectorProgram.discardGLResources();
}

void MxRenderer::initializeGL()
{
    pIconProgram.init( this );
    pIconProgram.initializeGL();

    MxVector4F windowColor = MxThemeColors::clearColor;
    glClearColor( windowColor[0], windowColor[1], windowColor[2], 1.0f );
    enableDepthTest( pDepthTestEnabled);
}


void MxRenderer::setWindowSize( int width, int height )
{
    pScreenSize = MxVector2I( width, height);
}

MxVector2I MxRenderer::windowSize() const
{
    return pScreenSize;
}


void MxRenderer::checkGLError( const char *fileName, int lineNumber )
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        qDebug("[%s line %d] GL Error: %d", fileName, lineNumber, (int)err);
    }
}

void MxRenderer::setProgram( MxShaderProgram *effect )
{
    Q_ASSERT( NULL != effect );
    if( effect->programId() != pCurrShaderProgram )
    {
        effect->enable();
        pCurrShaderProgram = effect->programId();
    }
}

MxColorWheelProgram * MxRenderer::colorWheelProgram()
{
    if( pColorWheelEffect.programId() == 0 )
    {
        pColorWheelEffect.init(this);
        pColorWheelEffect.initializeGL();
    }

    setProgram( &pColorWheelEffect );
    return &pColorWheelEffect;
}


MxVectorProgram * MxRenderer::setVectorProgram()
{
    if( pVectorProgram.programId() == 0 )
    {
        pVectorProgram.init(this);
        pVectorProgram.initializeGL();
    }
    setProgram( &pVectorProgram );
    return &pVectorProgram;
}

MxIconProgram * MxRenderer::setIconProgram()
{
    setProgram( &pIconProgram );
    return &pIconProgram;
}

void MxRenderer::setBlending( MxRenderer::Blending blend )
{
    if( blend == pCurrBlend )
        return;

    switch(blend)
    {
    case NoBlending:
        glDisable(GL_BLEND);
        break;
    case BlendingText:
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); // see http://stackoverflow.com/questions/4771224/iphone-opengl-es-alpha-blending-i-have-black-color-in-edge
        break;
    case BlendingImages:
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // see http://www.andersriggelsen.dk/glblendfunc.php
        break;
        /*case BlendingGeometry:
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
        break;*/
    default:
        qFatal("Not implemented: setBlending");
        break;
    }

    pCurrBlend = blend;
}

void MxRenderer::enableDepthTest( bool enable )
{
    if( enable != pDepthTestEnabled ) {
        pDepthTestEnabled = enable;
        if( enable ) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
    }
}

void MxRenderer::bindTextureGL( GLuint textureId, GLuint activeSlot )
{
    if( pCurrentTexture == textureId && activeSlot == pCurrentActiveTextureSlot )
        return;


    pCurrentActiveTextureSlot = activeSlot;
    glActiveTexture(pCurrentActiveTextureSlot);

    pCurrentTexture = textureId;
    glBindTexture( GL_TEXTURE_2D, pCurrentTexture );
}

GpuBuffer *MxRenderer::newGpuBuffer( MxShaderProgram::VaoFormat format )
{
    // find first available buffer
    int vboCount = pVboList.size();
    for(int i=0; i<vboCount; ++i)
    {
        GpuBuffer &buffer = pVboList[i];
        Q_ASSERT( buffer.pFormat != MxShaderProgram::Unknown );
        if( buffer.size() == -1 && buffer.pFormat == format )
        {
            buffer.pSize = 0; // set as taken
            return &buffer;
        }
    }

    GpuBuffer *buffer = pVboList.appendAndGet();
    buffer->pFormat = format;
    Q_ASSERT( buffer->size() == 0 );
    return buffer;
}


void MxRenderer::clearGpuBuffers()
{
    int vboCount = pVboList.size();
    for(int i=0; i<vboCount; ++i)
    {
        GpuBuffer &buffer = pVboList[i];
        buffer.pSize = -1;
    }
}

