/****************************************************************************
** Copyright (C) 2020-2024 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef TRIANGLEPLUGIN_H
#define TRIANGLEPLUGIN_H

#include "mxplugin.h"
#include "triangleprogram.h"



// \TODO rename to BallPlugin
class TrianglePlugin : public MxPlugin
{
public:
    TrianglePlugin();
     virtual ~TrianglePlugin();


    /////// VjLayer API /////
    virtual MxParameter* parameter( int );
    virtual int parameterCount() const;
    virtual const char* name() const;
    virtual float getOpacity(float time) const;
    virtual void render( MxRenderer &renderer, const MxInputs& inputs );

private:

TriangleProgram pProgram;


};

#endif // FRACTALLAYER_H
