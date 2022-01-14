/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxpainter.h"
#include "mxabstractsvg.h"
#include "mxsvgpainter.h"
#include "mxguirenderer.h"
#include "mxthemecolors.h"
#include "mxabstractatlas.h"
#include "mxvectorprogram.h"


MxPainter::MxPainter()
{
    pVectorDraw.pTranslation = &pTranslation;

    new (&pIconDraw) MxIconDraw(); // call ctor because it has inherited class
    pIconDraw.pTranslation = &pTranslation;
}

MxPainter::~MxPainter()
{
}

void MxPainter::initArrays()
{
    pVectorDraw.pArray = NULL;
    pIconDraw.pArray = NULL;
}

void MxPainter::initializeGL(MxRenderer *)
{
}

/*! Called at the beginning of a render
  to reserve a buffer to store vertex/color data
 */
void MxPainter::prepareRender( MxGuiRenderer &renderer )
{
    MxBuffer *buffer = renderer.getTemporaryBuffer(9500);
    pVectorDraw.pArray = buffer;
    Q_ASSERT( buffer->size() == 0 );

    buffer = renderer.getTemporaryBuffer(8192);
    pIconDraw.pArray = buffer;
    Q_ASSERT( buffer->size() == 0 );

}


void MxPainter::render( MxGuiRenderer &renderer )
{
    Q_ASSERT( NULL != pVectorDraw.pArray );
    if( pVectorDraw.pArray->size() > 0 ) {
        renderer.setViewportToWindow();
        renderer.enableDepthTest( false );
        MxVectorProgram * svgProgram = renderer.setVectorProgram();
        renderer.setBlending( MxRenderer::BlendingImages );
        svgProgram->setMatrix( renderer.windowMatrix() );
        svgProgram->draw( pVectorDraw );
        renderer.checkGLError(__FILE__, __LINE__);

        pVectorDraw.pArray = NULL;
    }
    if( renderer.pIconAtlas->isLoaded() && pIconDraw.pArray->size() > 0 ) {
        renderer.setViewportToWindow();
        renderer.enableDepthTest( false );
        MxIconProgram *iconProgram = renderer.setIconProgram();
        renderer.bindTextureGL( renderer.pIconAtlas->texture() );
        renderer.setBlending( MxRenderer::BlendingImages );
        iconProgram->draw( pIconDraw, renderer.windowMatrix() );
        pIconDraw.pArray = NULL;
    }
}


void MxPainter::setTranslation( const MxVector2F &translation )
{
    pTranslation = translation;

    Q_ASSERT( pIconDraw.pTranslation == &pTranslation );


}


MxIconDraw& MxPainter::iconDraw()
{
    return pIconDraw;
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


/*MxTextDraw& MxPainter::textDraw()
{
    return pTextDraw;
}*/

/*! \class MxPainter
    \brief Holder to the different painters

    MxPainter is passed as argument to MxWidget::paint() function.

    \sa MxVectorDraw
*/
