/****************************************************************************
** Copyright (C) 2020-2024 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXPARAMETER_H
#define MXPARAMETER_H

class MxInputs;


class MxParameter
{
public:
    /*!
     * \brief MxParameter holds the input parameter value
     * \param value Default parameter value
     * \param transitionTime Default transition time
     */
    MxParameter( float value, float transitionTime );
  //  MxParameter();

     float value(  const MxInputs &inputs  ) const;

};

#endif // MXPARAMETER_H
