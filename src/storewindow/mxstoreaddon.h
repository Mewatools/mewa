/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXSTOREADDON_H
#define MXSTOREADDON_H

/*! Interface class for a Store Addon.
*/
class MxStoreAddon
{
public:

    virtual bool isInstalled() const = 0;
    virtual bool hasUpdateAvailable() const = 0;
    virtual int totalLikes() const = 0;
    virtual bool isInstalling() const = 0;
    virtual bool isUpdatingScore() const = 0;
    virtual bool userLike() const = 0;
};


#endif // MXSTOREADDON_H
