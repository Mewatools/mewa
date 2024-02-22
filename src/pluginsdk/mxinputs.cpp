/****************************************************************************
** Copyright (C) 2020-2024 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxinputs.h"

MxInputs::MxInputs()
{

}

 float MxInputs::time() const
 {
     return pTime;
 }

 float MxInputs::opacity() const
 {
     return pOpacity;
 }


/*! \class MxInputs
    \brief Holds the data provided to a plugin

    MxPlugin renders an image based on given inputs. Some inputs are plugin parameters hold by the MxParameter class.
	Other inputs are acccessed through MxInputs provided as argument to MxPlugin::render().

    \sa MxParameter and MxPlugin
*/
