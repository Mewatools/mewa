/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxpainter.h"
#include "mxabstractsvg.h"
#include "mxsvgpainter.h"


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

void MxPainter::initArrays( MxList<GpuBuffer, MxClassInitializer<GpuBuffer> > &vboList )
{
    vboList.reserve(16);
}

void MxPainter::initializeGL(MxRenderer *)
{
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
