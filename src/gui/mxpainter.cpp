/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxpainter.h"
#include "mxabstractsvg.h"
#include "mxsvgpainter.h"
#include "mxrenderer.h"
#include "mxabstractatlas.h"


MxPainter::MxPainter()
{
    pVectorDraw.pTranslation = &pTranslation;
    for( int i=0; i<ColorCount; ++i )
    {
        pIconDraw[i].pTranslation = &pTranslation;
    }
}

MxPainter::~MxPainter()
{
}

void MxPainter::discardGLResources()
{
}

void MxPainter::initArrays()
{
    for( int i=0; i<ColorCount; ++i )
    {
        pIconDraw[i].pArray = NULL;
    }
}

void MxPainter::initializeGL(MxRenderer *)
{
}

/*! \TODO all streams (arrays) should be handled this way.
 * set a vbo on the beggining of each render.
 */
void MxPainter::prepareRender( MxRenderer &renderer )
{
    GpuBuffer *vboBuffer = renderer.newGpuBuffer( MxVectorProgram::getVaoFormat() );
    vboBuffer->reserveForAppend(9500);
    pVectorDraw.pArray = vboBuffer;

    vboBuffer = renderer.newGpuBuffer( MxShaderProgram::Float_2_2 );
    vboBuffer->reserveForAppend(4800);
    pIconDraw[MxPainter::OriginalColor].pArray = vboBuffer;

    vboBuffer = renderer.newGpuBuffer( MxShaderProgram::Float_2_2 );
    vboBuffer->reserveForAppend(512);
    pIconDraw[MxPainter::BlueColor].pArray = vboBuffer;

    vboBuffer = renderer.newGpuBuffer( MxShaderProgram::Float_2_2 );
    vboBuffer->reserveForAppend(512);
    pIconDraw[MxPainter::LightColor].pArray = vboBuffer;
}


void MxPainter::render( MxRenderer &renderer )
{
    Q_ASSERT( NULL != pVectorDraw.pArray );
    if( pVectorDraw.pArray->size() > 0 ) {
        //renderer.enableDepthTest( false );
        MxVectorProgram * svgProgram = renderer.setVectorProgram();
        renderer.setBlending( MxRenderer::BlendingImages );
        svgProgram->setMatrix(renderer.pScreenProjectionMatrix);
        svgProgram->draw( pVectorDraw );
        renderer.checkGLError(__FILE__, __LINE__);

        pVectorDraw.pArray->pSize = -1;
        pVectorDraw.pArray = NULL;
    }


    MxIconProgram::ColorFilter ifilter[MxPainter::ColorCount];
    Q_ASSERT( 3 == MxPainter::ColorCount );
    ifilter[0] = MxIconProgram::IdentityFilter;
    ifilter[1] = MxIconProgram::BlueFilter;
    ifilter[2] = MxIconProgram::LightGrayFilter;
    for(int i=0; i < MxPainter::ColorCount; ++i) {
        if( renderer.pIconAtlas->isLoaded() && pIconDraw[i].pArray->size() > 0 ) {
            renderer.enableDepthTest( false );
            MxIconProgram *iconProgram = renderer.setIconProgram();
            renderer.bindTextureGL( renderer.pIconAtlas->texture() );
            renderer.setBlending( MxRenderer::BlendingImages );
            iconProgram->setModelViewMatrix(renderer.pScreenProjectionMatrix);
            iconProgram->setColorFilter( ifilter[i] );
            iconProgram->drawRects( pIconDraw[i] );
            pIconDraw[i].pArray->pSize = -1;
            pIconDraw[i].pArray = NULL;
        }
    }
}


void MxPainter::setTranslation( const MxVector2F &translation )
{
    pTranslation = translation;

    for( int i=0; i<ColorCount; ++i )
    {
        Q_ASSERT( pIconDraw[i].pTranslation == &pTranslation );
    }
}


void MxPainter::drawSvg( MxAbstractSvg *svg, const MxRectF &targetRect )
{
    Q_ASSERT( NULL != svg );

    const MxVector2F tx = targetRect.bottomLeft();
    const MxVector2F targetSize = targetRect.size();
    MxVector2F baseline = svg->canvasSize();
    MxVector2F scale = targetSize / baseline;

    MxSvgPainter svgPainter(pVectorDraw, scale);
    *(pVectorDraw.pTranslation) += tx;
    svg->draw( svgPainter );
    *(pVectorDraw.pTranslation) -= tx;
}

MxVectorDraw & MxPainter::vectorDraw()
{
    return pVectorDraw;
}

MxIconDraw & MxPainter::iconDraw(IconColor color)
{
 return pIconDraw[color];
}

MxTextDraw & MxPainter::textDraw()
{
    return pTextDraw;
}

/*! \class MxPainter
    \brief Holder to the different painters

    MxPainter is passed as argument to MxWidget::paint() function.

    \sa MxVectorDraw
*/
