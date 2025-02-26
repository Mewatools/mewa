/****************************************************************************
** Copyright (C) 2020-2024 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef TRIANGLEPLUGIN_H
#define TRIANGLEPLUGIN_H

#include "mxplugin.h"
#include "triangleprogram.h"



class TrianglePlugin : public MxPlugin
{
public:
    TrianglePlugin();
    virtual ~TrianglePlugin();


    /////// VjLayer API /////
    virtual MxParameter* parameter( int );
    virtual int parameterCount() const;
    virtual const char* name() const;
    virtual void render( MxGuiRenderer &renderer, const MxInputs& inputs );
    virtual void populateParametersWindow( MxParametersPanel *window );

private:

    TriangleProgram pProgram;


};

#endif // TRIANGLEPLUGIN_H
