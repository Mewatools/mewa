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
    for( int i=0; i<ColorCount; ++i )
    {
        new (&(pIconDraw[i])) MxIconDraw(); // call ctor because it has inherited class
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

    buffer = renderer.getTemporaryBuffer(4800);
    pIconDraw[MxPainter::OriginalColor].pArray = buffer;
    Q_ASSERT( buffer->size() == 0 );

    buffer = renderer.getTemporaryBuffer(512);
    pIconDraw[MxPainter::BlueColor].pArray = buffer;
    Q_ASSERT( buffer->size() == 0 );

    buffer = renderer.getTemporaryBuffer(512);
    pIconDraw[MxPainter::LightColor].pArray = buffer;
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

    MxIconProgram::ColorFilter ifilter[MxPainter::ColorCount];
    Q_ASSERT( 3 == MxPainter::ColorCount );
    ifilter[0] = MxIconProgram::IdentityFilter;
    ifilter[1] = MxIconProgram::BlueFilter;
    ifilter[2] = MxIconProgram::LightGrayFilter;
    for(int i=0; i < MxPainter::ColorCount; ++i) {
        if( renderer.pIconAtlas->isLoaded() && pIconDraw[i].pArray->size() > 0 ) {
            renderer.setViewportToWindow();
            renderer.enableDepthTest( false );

            MxIconProgram *iconProgram = renderer.setIconProgram();
#ifdef MX_DIRECTX12_RENDERER
            renderer.bindTexture(renderer.pIconAtlas->texture(), MxTexture::NoFilter | MxTexture::ClampWrap, 0);
#else
            renderer.bindTextureGL(renderer.pIconAtlas->texture());
#endif
            
            renderer.setBlending( MxRenderer::BlendingImages );
            iconProgram->setModelViewMatrix(renderer.windowMatrix());
            iconProgram->setColorFilter( ifilter[i] );
            iconProgram->draw( pIconDraw[i] );
            //pIconDraw[i].pArray->pSize = -1;
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


MxIconDraw& MxPainter::iconDraw( IconColor color )
{
    return pIconDraw[color];
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
