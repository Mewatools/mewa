/****************************************************************************
** Copyright (C) 2020-2024 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "triangleplugin.h"
#include "mxrenderer.h"
#include "mxparameter.h"


TrianglePlugin::TrianglePlugin()
{
}

TrianglePlugin::~TrianglePlugin()
{
}

MxParameter* TrianglePlugin::parameter( int )
{
    return nullptr;
}

int TrianglePlugin::parameterCount() const
{
    return 0;
}

const char* TrianglePlugin::name() const
{
    return "Triangle";
}

float TrianglePlugin::getOpacity(float time) const
{
    return 1.0f;
}

void TrianglePlugin::render( MxRenderer &renderer, const MxInputs& inputs  )
{
    renderer.setProgram(&pProgram);
    Q_ASSERT( pProgram.isInitialized() );

    const GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };
    MxGpuArray *gpuArray = renderer.uploadToGpu( pProgram.vaoFormat(), (char*)gTriangleVertices, 6*sizeof(GLfloat) );
    pProgram.enableVao( gpuArray );

    renderer.glDrawArrays(GL_TRIANGLES, 0, 3);

    renderer.checkGLError(__FILE__, __LINE__);
}
