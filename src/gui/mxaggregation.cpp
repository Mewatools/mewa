/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxaggregation.h"
#include "mxdebug.h"

MxAggregation * MxAggregation::sAggregation = 0;


MxAggregation::MxAggregation()
{
    pIconAtlas = NULL;
    pApplication = NULL;
}

MxAggregation* MxAggregation::instance()
{
    Q_ASSERT( 0 != sAggregation );
    return sAggregation;
}

float MxAggregation::displayScale() const
{
    return 1.0f;
}

const MxAbstractAtlas *MxAggregation::iconAtlas() const
{
    Q_ASSERT( NULL != pIconAtlas );
    return pIconAtlas;
}

MxApplication * MxAggregation::application() const
{
    Q_ASSERT( NULL != pApplication );
    return pApplication;
}
