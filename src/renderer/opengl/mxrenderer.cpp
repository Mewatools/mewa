/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxrenderer.h"
#include "mxthemecolors.h"
#include "mxtexture.h"


MxRenderer::MxRenderer()
{
    pViewport.x = 0;
    pViewport.y = 0;
    pViewport.width = 0;
    pViewport.height = 0;


    pCurrShaderProgram = 9999; // very high number because 0 is reserved
    pCurrBlend = NoBlending;
    pDepthTestEnabled = false;
    pCurrentTexture = 99999; //zero is reserved
    pCurrentActiveTextureSlot = 0;
}

void MxRenderer::discardGLResources()
{
    pCurrBlend = NoBlending;
    pDepthTestEnabled = false;
    pCurrentTexture = 99999; //zero is reserved
    pCurrentActiveTextureSlot = 0;
    
    
}

void MxRenderer::initialize()
{
    MxVector4F windowColor = MxThemeColors::clearColor;
    glClearColor( windowColor[0], windowColor[1], windowColor[2], 1.0f );
    enableDepthTest( pDepthTestEnabled);
}


void MxRenderer::setWindowSize( int width, int height )
{
    pScreenSize = MxVector2I( width, height);
}

const MxVector2I& MxRenderer::windowSize() const
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

void MxRenderer::setProgram( MxGpuProgram *effect )
{
    Q_ASSERT( NULL != effect );
    if( effect->programId() != pCurrShaderProgram )
    {
        effect->enable();
        pCurrShaderProgram = effect->programId();
    }
}

void MxRenderer::setViewport( int x, int y, unsigned int width, unsigned int height )
{
    if(  pViewport.width != width ||
         pViewport.height != height ||
         pViewport.x != x ||
         pViewport.y != y )
    {
        pViewport.x = x;
        pViewport.y = y;
        pViewport.width = width;
        pViewport.height = height;

        glViewport( x, y, width, height);
    }
}

void MxRenderer::setScissor( int x, int y, unsigned int width, unsigned int height )
{
    glScissor( x, y, width, height);
}

void MxRenderer::setTexturesParameters( unsigned int flags )
{

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
    glActiveTexture( GL_TEXTURE0 + pCurrentActiveTextureSlot);
    pCurrentTexture = textureId;
    glBindTexture( GL_TEXTURE_2D, pCurrentTexture );
}

void MxRenderer::bindTexture( MxTexture *texture, unsigned char parameters, int inputIndex )
{
    Q_ASSERT( ! texture->isNull() );
    Q_ASSERT( texture->mTextureId > 0 );
    bindTextureGL( texture->mTextureId, inputIndex );

    texture->setParameters(this, parameters);
}

MxGpuArray *MxRenderer::uploadToGpu( MxGpuProgram::VaoFormat format, const char *data, unsigned int size )
{
    MxGpuArray *gpuArray = newGpuArray( format, size );
    gpuArray->uploadToVbo( this, data, size );
    return gpuArray;
}

MxGpuArray *MxRenderer::newGpuArray( MxGpuProgram::VaoFormat format, unsigned int size )
{
    // find first available buffer
    int vboCount = pReusableVbos.size();
    for(int i=0; i<vboCount; ++i)
    {
        ReusableVbo &reusableEntry = pReusableVbos[i];
        MxGpuArray &buffer = reusableEntry.gpuArray;
        Q_ASSERT( buffer.pFormat != MxGpuProgram::Unknown );
        if( !reusableEntry.inUse && buffer.pFormat == format )
        {
            //buffer.pSize = 0; // set as taken
            reusableEntry.inUse = true;
            return &buffer;
        }
    }

    ReusableVbo *newEntry = pReusableVbos.appendAndGet();
    newEntry->gpuArray.pFormat = format;
    newEntry->inUse = true;
    Q_ASSERT( newEntry->gpuArray.pVboSize == 0 );
    return &(newEntry->gpuArray);
}



void MxRenderer::renderBegin()
{
    glClear(0x0);
    glDisable(GL_CULL_FACE);
}

void MxRenderer::renderEnd()
{
    int count = pReusableVbos.size();
    for(int i=0; i<count; ++i)
    {
        // \TODO find the best size fit
        ReusableVbo &vbo = pReusableVbos[i];
        vbo.inUse = false;
    }
}


