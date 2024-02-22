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
     * \brief renders the plugin output
     * \param renderer Rendering functions
     * \param inputs Input data needed to render the plugin output
     *
     * Shaders are compiled in the initialize() function.
     *
     * MxRenderer::setProgram() calls initialize(), but the initialize() can be called before MxRenderer::setProgram().
	 *
	 * Use MxGpuProgram::isInitialized() to check if a shader was compiled successfully after setProgram().
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
