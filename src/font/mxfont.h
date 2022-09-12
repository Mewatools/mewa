/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXFONT_H
#define MXFONT_H


#include "mxvector.h"
#include "mxstring.h"
#include "mxopengl.h"


class MxRenderer;

class MxFont
{
public:
    MxFont()
    {}


    enum MetricName
    {
        Height = 0, //! baseline-to-baseline distance
        Ascender,
        Descender, //! distance from the baseline to the bottom
        SpaceAdvance,
        UnderlinePosition,
        UnderlineThickness
    };


    virtual void loadGL( MxRenderer *renderer ) = 0;
    virtual GLuint texture() const = 0;




    struct CharDetails
    {
        unsigned short unicode;
        float textureX;
        float textureY;
        float textureWidth;
        float textureHeight;
        float width;
        float height;
        float xOffset;
        float yOffset; // to position char
        float xAdvance; // space added after char
    };


    virtual float metric( MetricName name ) const = 0;
    virtual const CharDetails& charDetails( const char32_t character ) const = 0;
    virtual int width( const char *str, int size ) const = 0;
  // virtual int widthOfNullTerminated( const char *str ) const = 0;
    virtual int width( const MxString &str ) const = 0;


};

#endif // QXABSTRACTFONT_H
