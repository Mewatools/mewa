/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXEVENTHANDLER_H
#define MXEVENTHANDLER_H

#include "mxdebug.h"

/*!
 *  Interface for classes that receive events.
 *  Events can come from any thread.
 */
class MxEventHandler
{
public:

    //! Implement this member with the handling of event \a eventId.
    virtual void eventReceived( int eventId ) = 0;

};

#endif
