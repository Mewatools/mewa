/****************************************************************************
** Copyright (C) 2020-2024 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXPLUGIN_H
#define MXPLUGIN_H


class MxParameter;
class MxRenderer;
class MxInputs;




class MxPlugin
{
public:
    MxPlugin();

    virtual MxParameter* parameter( int ) = 0;
    virtual int parameterCount() const = 0;
    virtual const char* name() const = 0;
    virtual float getOpacity(float time) const = 0;

    /*!
     * \brief render
     * \param renderer
     * \param inputs
     *
     * Shaders are compiled in initialize() function.
     * \code
    if( ! pProgram.isInitialized() )
    {
        bool ok = pProgram.initialize(&renderer);
        if( !ok )
        {
            qDebug("shader compilation failed");
            return;
        }
    }

    renderer.setProgram(&pProgram);
     * \endcode
     *
     * MxRenderer::setProgram() calls initialize(). In that case
     * check if shaders were compiled after setProgram().
     *
     * \code
    renderer.setProgram(&pProgram);
    if( ! pProgram.isInitialized() )
    {
        qDebug("shader compilation failed");
        return;
    }
     * \endcode
     *
     */
    virtual void render( MxRenderer &renderer, const MxInputs& inputs ) = 0;
};

#endif // VJLAYER_H
