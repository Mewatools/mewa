/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxicondraw.h"
#include "mxgeometryutils.h"
#include "mxrenderer.h"
#include "mxabstractatlas.h"
#include "mxbuffer.h"


MxIconDraw::MxIconDraw()
    : MxQuadDraw(NULL)
{
    pTranslation = NULL;
}


MxIconDraw::MxIconDraw(MxBuffer *buffer, const MxVector2F *translation )
    : MxQuadDraw(buffer)
{
    pTranslation = translation;
    Q_ASSERT( pArray->size() == 0 );
}

MxIconDraw::~MxIconDraw()
{}

void MxIconDraw::clear()
{
    Q_ASSERT( (NULL != pArray) && (NULL != pTranslation) );

    //pVertices = 0;
    pArray->clear();
}

