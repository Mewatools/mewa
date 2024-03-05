/****************************************************************************
** Copyright (C) 2020-2024 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxparameter.h"

MxParameter::MxParameter( float value, float transitionTime )
{

}

float MxParameter::value(  const MxInputs &inputs  ) const
{
return 0.88;
}


/*! \class MxParameter
    \brief Holds a plugin parameter value

    MxPlugin renders an image based on given inputs. Some inputs are plugin parameters hold by the MxParameter class.


    \sa MxInputs and MxPlugin
*/
