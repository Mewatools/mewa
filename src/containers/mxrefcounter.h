/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXREFCOUNTER_H
#define MXREFCOUNTER_H


class MxRefCounter
{
public:

    int refCounter;

    inline void incRefCounter() {
        if (refCounter > 0)
            refCounter++;
    }

    inline bool decRefCounter() {
        if (refCounter <= 0)
            return true;
        return refCounter--;
    }

};

#endif // MXREFCOUNTER_H
