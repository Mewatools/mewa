/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxguiaggregation.h"
#include "mxdebug.h"
#include "mxapplication.h"


MxGuiAggregation * MxGuiAggregation::sAggregation = 0;


MxGuiAggregation::MxGuiAggregation()
{
    pIconAtlas = NULL;
    pApplication = NULL;
}

MxGuiAggregation* MxGuiAggregation::instance()
{
    Q_ASSERT( 0 != sAggregation );
    return sAggregation;
}

float MxGuiAggregation::displayScale() const
{
    return 1.0f;
}

const MxAbstractAtlas *MxGuiAggregation::iconAtlas() const
{
    Q_ASSERT( NULL != pIconAtlas );
    return pIconAtlas;
}

MxApplication * MxGuiAggregation::application() const
{
    Q_ASSERT( NULL != pApplication );
    return pApplication;
}

const MxFont * MxGuiAggregation::font()
{
    return pApplication->pPainterBuffer.font();
}


