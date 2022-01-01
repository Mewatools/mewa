/****************************************************************************
** Copyright (C) 2021-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxgpuprogram.h"
#include "mxdebug.h"


MxGpuProgram::MxGpuProgram()
{
    pRenderer = nullptr;
}

void MxGpuProgram::init(MxRenderer* renderer)
{
    Q_ASSERT(NULL == pRenderer);
    pRenderer = renderer;
}


