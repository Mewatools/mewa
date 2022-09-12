/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXTEXTDRAW_H
#define MXTEXTDRAW_H

#include "mxvector.h"
#include "mxstring.h" // because of qxunicodechar
#include "mxrect.h"


class MxBuffer;
class MxFont;


class MxTextDraw
{
public:
    MxTextDraw();

    int vertexCount() const;

    void clear();

    void drawText(const char *ascii, int size, const MxVector2F &pos , const MxVector4UC &color );
    void drawTextCentered( const MxString &string, const MxRectF &rect, const MxVector4UC &color );
    void drawElidedLineText(const char *text, int stringSize, const MxVector2F &pos, const MxVector4UC &color, float textMaxLength );
    void drawElidedLineText(const MxString &text, const MxVector2F &pos, const MxVector4UC &color, float textMaxLength );


    const MxVector2F *pTranslation;
    MxBuffer *pArray;
    MxFont *pFont;

    struct Vertex
    {
        float x;
        float y;

        float u;
        float v;

        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };


private:


    void fillBuffer(const char *ascii, int charCount , const MxVector2F &pos, const MxVector4UC &color);
    void loopThroughString(Vertex *&ptr, const char *str, int charCount, int *charDraw, float startX, float &xPos, float &yPos, const MxVector4UC &color );
    void fillUnicode(Vertex *&ptr, float &currX, float &y, unsigned short unicode , const MxVector4UC &color);



};

#endif
