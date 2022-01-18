/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXICONATLAS_H
#define MXICONATLAS_H

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



    void setIcon( MxThemeIcons::IconName name, const char *imageFileName );
    // Generate image atlas
    bool build();


private:
    virtual const MxVector2I& textureSize() const;




    MxImage pAtlasImage;

    struct ImageDetails
    {
        MxRectF textureRect;
        MxVector2I iconSize;
        unsigned char* rgba; // must be unsigned char* because of lodepng
    };

    ImageDetails pImageList[MxThemeIcons::ImageCount];


};



#endif
