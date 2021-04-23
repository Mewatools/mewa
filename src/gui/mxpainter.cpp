/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxpainter.h"
#include "mxabstractsvg.h"
#include "mxsvgpainter.h"
#include "mxrenderer.h"

MxPainter::MxPainter()
{
    pVectorDraw.pTranslation = &pTranslation;
}

MxPainter::~MxPainter()
{
}

void MxPainter::discardGLResources()
{
}

void MxPainter::initArrays()
{
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
}


void MxPainter::setTranslation( const MxVector2F &translation )
{
    pTranslation = translation;
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

MxTextDraw & MxPainter::textDraw()
{
    return pTextDraw;
}
