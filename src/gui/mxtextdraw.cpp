/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxtextdraw.h"
#include "mxbuffer.h"
#include "mxfont.h"

MxTextDraw::MxTextDraw()
    :pArray(NULL), pTranslation(NULL)
{


    pFont = NULL;

}



int MxTextDraw::vertexCount() const
{
    Q_ASSERT( (NULL != pArray) && (NULL != pTranslation) );


    int r = pArray->size() / sizeof(MxTextDraw::Vertex);
    return r;
}

void MxTextDraw::clear()
{
    Q_ASSERT( (NULL != pArray) && (NULL != pTranslation) );

    //pVertices = 0;
    pArray->clear();
}

void MxTextDraw::drawText( const char *ascii, int size, const MxVector2F &pos, const MxVector4UC &color )
{
    if( NULL == pFont ) { // font is being loaded, dont draw
        return;
    }

    Q_ASSERT( (NULL != pArray) && (NULL != pTranslation) );


    fillBuffer( ascii, size, pos, color );
}

void MxTextDraw::drawTextCentered( const MxString &string, const MxRectF &rect, const MxVector4UC &color )
{
    Q_ASSERT( (NULL != pArray) && (NULL != pTranslation) );

    int textWidth = pFont->width(string);
    const int textHeight = pFont->metric( MxFont::Ascender ) + pFont->metric( MxFont::Descender );
    MxVector2F centeredPos;
    centeredPos[0] = (rect.width() - textWidth) / 2.0f;
    centeredPos[0] += rect.left();
    centeredPos[1] = (rect.height() - textHeight) / 2.0f;
    centeredPos[1] += rect.bottom();


    //drawText( string.data(), string.size(), centeredPos, color );
    int drawableCharCount = 0;
    int charCount = string.size();
    pArray->reserveForAppend( charCount * (6 * sizeof(MxTextDraw::Vertex)) );
    MxTextDraw::Vertex *dst = (MxTextDraw::Vertex*) pArray->end();
    float startX = centeredPos.x() + (*pTranslation)[0];
    float currX = startX;
    float y = centeredPos.y()  + (*pTranslation)[1];
    //loopThroughString( dst, ascii, charCount, &drawableCharCount, startX, currX, y, color );
    const int lineHeight = pFont->metric(MxFont::Height);
    const float charWidth = pFont->metric(MxFont::SpaceAdvance);
    const char32_t *str = string.data();
    for(int i=0; i<charCount; ++i)
    {
        if(*str == ' ')
        {
            currX += charWidth;
        }
        else if( *str == '\n' )
        {
            currX = startX;
            y -= lineHeight;
        }
        else
        {
            drawableCharCount++;
            fillUnicode( dst, currX, y, *str, color );
        }
        ++str;
    }

    const int bytes = drawableCharCount * (6 * sizeof(MxTextDraw::Vertex));
    pArray->addToSize( bytes );

}

void MxTextDraw::drawElidedLineText(const char *text, int stringSize, const MxVector2F &pos, const MxVector4UC &color, float textMaxLength )
{
    Q_ASSERT( (NULL != pArray) && (NULL != pTranslation) );

    int drawableCharCount = 0;

    const int bytes = stringSize * 6 * sizeof(Vertex);
    pArray->reserveForAppend( bytes );
    //float *data = (float*) pArray->end();
    MxTextDraw::Vertex *dst = (MxTextDraw::Vertex*) pArray->end();

    float startX = pos.x() + (*pTranslation)[0];
    float currX = startX;
    float y = pos.y()  + (*pTranslation)[1];

    textMaxLength += currX;
const char *str = text;
    const int lineHeight = pFont->metric(MxFont::Height);
    const float charWidth = pFont->metric(MxFont::SpaceAdvance);
    for(int i=0; i<stringSize; ++i)
    {
        if( *str == ' ' ) {
            currX += charWidth;
        } else if( *str == '\n' ) {

            currX = startX;
            y -= lineHeight;

        } else {

            const MxFont::CharDetails &detail = pFont->charDetails( *str );

            float tx = detail.textureX;
            float ty = detail.textureY;
            float w = (int)detail.width;
            float h = (int)detail.height;
            float tw = detail.textureWidth;
            float th = detail.textureHeight;
            float texTop = ty + th;
            float texRight = tx + tw;
            float worldBottom = (int)(y - (h - detail.yOffset));
            float worldLeft = (int)(currX + detail.xOffset);
            float worldTop = worldBottom + h;
            float worldRight = (worldLeft) + w;

            if( worldRight >= textMaxLength )
            {
                // apply gradient
// rewind 3 characters and apply linear gradient

                MxTextDraw::Vertex *vPtr = dst;
                vPtr--;
                float gradEndX = vPtr->x;
                const int rewindCharacters = 5;
                const int rewindVertices = rewindCharacters*6;
                vPtr = dst - rewindVertices;
                float gradStartX = vPtr->x;
                float initialAlpha = (vPtr->a / 255.0f);

                // update alpha values
                float deltaX = gradEndX - gradStartX;
                for(int i=0; i < rewindVertices; ++i) {
                   float vx = vPtr->x - gradStartX;
                   float alphaF = (1.0f - (vx / deltaX)) * initialAlpha; // value from 0 to 1
                   unsigned char alphaUC = (alphaF * 255.0f);
                   vPtr->a = alphaUC;
                   vPtr++;
                }

                break;
            }
            else
            {
                /*
                // texture triangles
                *data = worldLeft; ++data; *data = worldBottom; ++data; // world bottom left
                *data = tx; ++data; *data = ty; ++data; // texture bottom left

                *data = worldRight; ++data; *data = worldBottom; ++data; // world bottom right
                *data = texRight; ++data; *data = ty; ++data; // texture bottom right

                *data = worldLeft; ++data; *data = worldTop; ++data; // world top left
                *data = tx; ++data; *data = texTop; ++data; // texture top left

                *data = worldRight; ++data; *data = worldTop; ++data; // top right
                *data = texRight; ++data; *data = texTop; ++data; // texture top right

                *data = worldLeft; ++data; *data = worldTop; ++data; // world top left
                *data = tx; ++data; *data = texTop; ++data; // texture top left

                *data = worldRight; ++data; *data = worldBottom; ++data; // world bottom right
                *data = texRight; ++data; *data = ty; ++data; // texture bottom right

                currX += detail.xAdvance;

                drawableCharCount++;*/

                drawableCharCount++;
                fillUnicode( dst, currX, y, *str, color );

            }
        }
        str++;
    }

    pArray->addToSize(drawableCharCount * 6 * sizeof(Vertex));

}

void MxTextDraw::drawElidedLineText(const MxString &text, const MxVector2F &pos, const MxVector4UC &color, float textMaxLength )
{
    Q_ASSERT( (NULL != pArray) && (NULL != pTranslation) );

    int drawableCharCount = 0;

    const int stringSize = text.size();
    const int bytes = stringSize * 6 * sizeof(Vertex);
    pArray->reserveForAppend( bytes );
    //float *data = (float*) pArray->end();
    MxTextDraw::Vertex *dst = (MxTextDraw::Vertex*) pArray->end();

    float startX = pos.x() + (*pTranslation)[0];
    float currX = startX;
    float y = pos.y()  + (*pTranslation)[1];

    textMaxLength += currX;
const char32_t *str = text.data();
    const int lineHeight = pFont->metric(MxFont::Height);
    const float charWidth = pFont->metric(MxFont::SpaceAdvance);
    for(int i=0; i<stringSize; ++i)
    {
        if( *str == ' ' ) {
            currX += charWidth;
        } else if( *str == '\n' ) {

            currX = startX;
            y -= lineHeight;

        } else {

            const MxFont::CharDetails &detail = pFont->charDetails( *str );
            float w = (int)detail.width;
            float worldLeft = (int)(currX + detail.xOffset);
            float worldRight = (worldLeft) + w;

            if( worldRight >= textMaxLength )
            {
                // apply gradient
// rewind 3 characters and apply linear gradient

                MxTextDraw::Vertex *vPtr = dst;
                vPtr--;
                float gradEndX = vPtr->x;
                const int rewindCharacters = 5;
                const int rewindVertices = rewindCharacters*6;
                vPtr = dst - rewindVertices;
                float gradStartX = vPtr->x;
                float initialAlpha = (vPtr->a / 255.0f);

                // update alpha values
                float deltaX = gradEndX - gradStartX;
                for(int i=0; i < rewindVertices; ++i) {
                   float vx = vPtr->x - gradStartX;
                   float alphaF = (1.0f - (vx / deltaX)) * initialAlpha; // value from 0 to 1
                   unsigned char alphaUC = (alphaF * 255.0f);
                   vPtr->a = alphaUC;
                   vPtr++;
                }

                break;
            }
            else
            {
                drawableCharCount++;
                fillUnicode( dst, currX, y, *str, color );

            }
        }
        str++;
    }

    pArray->addToSize(drawableCharCount * 6 * sizeof(Vertex));

}


void MxTextDraw::fillBuffer( const char *ascii, int charCount, const MxVector2F &pos, const MxVector4UC &color )
{
    Q_ASSERT( (NULL != pArray) && (NULL != pTranslation) );

    int stringSize = charCount;
    const char *str = ascii;
    int drawableCharCount = 0;

    pArray->reserveForAppend( charCount * (6 * sizeof(MxTextDraw::Vertex)) );
    MxTextDraw::Vertex *dst = (MxTextDraw::Vertex*) pArray->end();

    float startX = pos.x() + (*pTranslation)[0];
    float currX = startX;
    float y = pos.y()  + (*pTranslation)[1];

    // reset pointer to beggining of string
    /*str = ascii;
    for(int i=0; i<stringSize; ++i)
    {
        if(*str == ' ')
        {
            currX += pFont->metric( QxAbstractFont::SpaceAdvance );
        } else {
            drawableCharCount++;
            fillUnicode( dst, currX, y, *str, color );
        }
        ++str;
    }*/
    loopThroughString( dst, ascii, charCount, &drawableCharCount, startX, currX, y, color );



    const int bytes = drawableCharCount * (6 * sizeof(MxTextDraw::Vertex));
    pArray->addToSize( bytes );

}

void MxTextDraw::loopThroughString( MxTextDraw::Vertex *&ptr, const char *str, int charCount, int *charDraw, float startX, float &xPos, float &yPos, const MxVector4UC &color )
{

    const int lineHeight = pFont->metric(MxFont::Height);
    const float charWidth = pFont->metric(MxFont::SpaceAdvance);

    for(int i=0; i<charCount; ++i)
    {
        if(*str == ' ')
        {
            xPos += charWidth;
        }
        else if( *str == '\n' )
        {
            xPos = startX;
            yPos -= lineHeight;
        }
        else
        {
            (*charDraw)++;
            fillUnicode( ptr, xPos, yPos, *str, color );
        }
        ++str;
    }
}


void MxTextDraw::fillUnicode( MxTextDraw::Vertex *&ptr, float &currX, float &y, unsigned short unicode, const MxVector4UC &color )
{
    Q_ASSERT( (NULL != pArray) && (NULL != pTranslation) );


    Q_ASSERT( unicode != ' ' );


    // generate points
    const MxFont::CharDetails &detail = pFont->charDetails( unicode );
    float tx = detail.textureX;
    float ty = detail.textureY;
    float w = detail.width;
    float h = detail.height;
    // \TODO detail.textureWidth should hold tx + tw
    float tw = detail.textureWidth;//w / imageSize.width();
    float th = detail.textureHeight;//h / imageSize.height();
    float texTop = ty + th;
    float texRight = tx + tw;
    float worldBottom = (int)y - (h - detail.yOffset);
    float worldLeft = (int)currX + detail.xOffset;
    float worldTop = (int)worldBottom + h;
    float worldRight = worldLeft + w;


    ptr->x = worldLeft;
    ptr->y = worldBottom;
    ptr->r = color[0];
    ptr->g = color[1];
    ptr->b = color[2];
    ptr->a = color[3];
    ptr->u = tx;
    ptr->v = ty;

    ptr++;
    ptr->x = worldRight;
    ptr->y = worldBottom;
    ptr->r = color[0];
    ptr->g = color[1];
    ptr->b = color[2];
    ptr->a = color[3];
    ptr->u = texRight;
    ptr->v = ty;

    ptr++;
    ptr->x = worldLeft;
    ptr->y = worldTop;
    ptr->r = color[0];
    ptr->g = color[1];
    ptr->b = color[2];
    ptr->a = color[3];
    ptr->u = tx;
    ptr->v = texTop;

    ptr++;
    ptr->x = worldRight;
    ptr->y = worldTop;
    ptr->r = color[0];
    ptr->g = color[1];
    ptr->b = color[2];
    ptr->a = color[3];
    ptr->u = texRight;
    ptr->v = texTop;

    ptr++;
    ptr->x = worldLeft;
    ptr->y = worldTop;
    ptr->r = color[0];
    ptr->g = color[1];
    ptr->b = color[2];
    ptr->a = color[3];
    ptr->u = tx;
    ptr->v = texTop;

    ptr++;
    ptr->x = worldRight;
    ptr->y = worldBottom;
    ptr->r = color[0];
    ptr->g = color[1];
    ptr->b = color[2];
    ptr->a = color[3];
    ptr->u = texRight;
    ptr->v = ty;

    ptr++;

    currX += detail.xAdvance;


}

