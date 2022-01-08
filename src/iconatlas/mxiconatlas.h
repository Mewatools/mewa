/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXICONATLAS_H
#define MXICONATLAS_H

#include <QImage>

#include "mxabstractatlas.h"
#include "mxlist.h"
#include "mxrenderer.h"
#include "mximage.h"
#include "mxthemeicons.h"


/*! Generates an atlas with a set of given icons/images
 */
class MxIconAtlas : public MxAbstractAtlas
{
public:
    MxIconAtlas();

    virtual const MxRectF& iconRect( int name ) const;
    virtual const MxVector2I &iconSize( int name ) const;
    void loadGL( MxRenderer& renderer );
    bool isLoaded() const;


    void setIcon( MxThemeIcons::IconName name, const char *imageFileName );
    // Generate image atlas
    bool build();


private:
    virtual unsigned int texture() const;
    virtual const MxVector2I& textureSize() const;




    MxImage pAtlasImage;

    struct ImageDetails
    {
        MxRectF textureRect;
        MxVector2I iconSize;
       // QImage image;
        unsigned char* rgba;
    };

    ImageDetails pImageList[MxThemeIcons::ImageCount];


};



#endif
