/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxicondraw.h"
#include "mxgeometryutils.h"
#include "mxrenderer.h"
#include "mxabstractatlas.h"


MxIconDraw::MxIconDraw()
    : MxQuadDraw(NULL)
{
    pTranslation = NULL;
}


MxIconDraw::MxIconDraw(MxRenderer &renderer, const MxVector2F *translation )
    : MxQuadDraw(NULL)
{
    pArray = renderer.newGpuBuffer( MxShaderProgram::Float_2_2 );

    pTranslation = translation;

    Q_ASSERT( pArray->size() == 0 );
}

void MxIconDraw::clear()
{
    Q_ASSERT( (NULL != pArray) && (NULL != pTranslation) );

    //pVertices = 0;
    pArray->clear();
}

void MxIconDraw::drawImageRect( const MxRectF &source, const MxRectF &dest )
{
    Q_ASSERT( (NULL != pArray) && (NULL != pTranslation) );

    float *dst = (float*) pArray->lastDataAndIncrement( 24 * sizeof(float) );

    const float texBottom = source.bottom();
    const float texTop = source.top();
    const float texLeft = source.left();
    const float texRight = source.right();

    const float left = dest.left() + (*pTranslation)[0];
    const float bottom = dest.bottom() + (*pTranslation)[1];
    const float right  = dest.right() + (*pTranslation)[0];
    const float top = dest.top() + (*pTranslation)[1];

    // left bottom
    *dst = left;
    dst++;
    *dst = bottom;
    dst++;
    *dst = texLeft;
    dst++;
    *dst = texBottom;
    dst++;
    // right bottom
    *dst = right;
    dst++;
    *dst = bottom;
    dst++;
    *dst = texRight;
    dst++;
    *dst = texBottom;
    dst++;
    // left top
    *dst = left;
    dst++;
    *dst = top;
    dst++;
    *dst = texLeft;
    dst++;
    *dst = texTop;
    dst++;
    // right top
    *dst = right;
    dst++;
    *dst = top;
    dst++;
    *dst = texRight;
    dst++;
    *dst = texTop;
    dst++;
    // left top
    *dst = left;
    dst++;
    *dst = top;
    dst++;
    *dst = texLeft;
    dst++;
    *dst = texTop;
    dst++;
    // right bottom
    *dst = right;
    dst++;
    *dst = bottom;
    dst++;
    *dst = texRight;
    dst++;
    *dst = texBottom;

    //pVertices += 6;
}


void MxIconDraw::drawImageRectRotated( const MxRectF &source, const MxRectF &dest, float angle )
{
    Q_ASSERT( (NULL != pArray) && (NULL != pTranslation) );


    float *dst = (float*) pArray->lastDataAndIncrement( 24 * sizeof(float) );

    const float texBottom = source.bottom();
    const float texTop = source.top();
    const float texLeft = source.left();
    const float texRight = source.right();

    MxVector2F destCenter = dest.center();
    //destCenter[0] = dest.left() + ((dest.right() - dest.left())/2.0f);
    //destCenter[1] = dest.bottom() + ((dest.top() - dest.bottom())/2.0f);

    MxVector2F lb = dest.bottomLeft();
    MxGeometryUtils::rotatePoint( destCenter, lb, angle );
    MxVector2F rt = dest.topRight();
    MxGeometryUtils::rotatePoint( destCenter, rt, angle );
    MxVector2F lt = dest.topLeft();
    MxGeometryUtils::rotatePoint( destCenter, lt, angle );
    MxVector2F rb = dest.bottomRight();
    MxGeometryUtils::rotatePoint( destCenter, rb, angle );

    lb +=(*pTranslation);
    rt +=(*pTranslation);
    lt +=(*pTranslation);
    rb +=(*pTranslation);
    //const float left = bl.x() + (*pTranslation)[0];
    //const float bottom = bl.y() + (*pTranslation)[1];
    //const float right  = tr.x() + (*pTranslation)[0];
    //const float top = tr.y() + (*pTranslation)[1];

    // left bottom
    *dst = lb.x();
    dst++;
    *dst = lb.y();
    dst++;
    *dst = texLeft;
    dst++;
    *dst = texBottom;
    dst++;
    // right bottom
    *dst = rb.x();
    dst++;
    *dst = rb.y();
    dst++;
    *dst = texRight;
    dst++;
    *dst = texBottom;
    dst++;
    // left top
    *dst = lt.x();
    dst++;
    *dst = lt.y();
    dst++;
    *dst = texLeft;
    dst++;
    *dst = texTop;
    dst++;
    // right top
    *dst = rt.x();
    dst++;
    *dst = rt.y();
    dst++;
    *dst = texRight;
    dst++;
    *dst = texTop;
    dst++;
    // left top
    *dst = lt.x();
    dst++;
    *dst = lt.y();
    dst++;
    *dst = texLeft;
    dst++;
    *dst = texTop;
    dst++;
    // right bottom
    *dst = rb.x();
    dst++;
    *dst = rb.y();
    dst++;
    *dst = texRight;
    dst++;
    *dst = texBottom;

    //pVertices += 6;
}



void MxIconDraw::draw3x3Frame( const MxRectF &target, const MxRectI &margins, const MxAbstractAtlas *style, MxThemeIcons::IconName name, bool flipVertically )
{
    Q_ASSERT( (NULL != pArray) && (NULL != pTranslation) );

#if 0
    const int bytes = 24 * sizeof(float);
    float *dst = (float*) pArray->lastDataAndIncrement( bytes );

    QxRectF textureRect = style.imageRect(name);
    const float texBottom = textureRect.bottom();
    const float texTop = textureRect.top();
    const float texLeft = textureRect.left();
    const float texRight = textureRect.right();

    const float left = target.left() + (*pTranslation)[0];
    const float bottom = target.bottom() + (*pTranslation)[1];
    const float right  = target.right() + (*pTranslation)[0];
    const float top = target.top() + (*pTranslation)[1];

    GLfloat vertices[] = {
        left, bottom,  texLeft, texBottom, // square 0,0
        right, bottom, texRight, texBottom,
        left, top,     texLeft, texTop,
        right, top,    texRight, texTop,
        left, top,     texLeft, texTop,
        right, bottom, texRight, texBottom
    };

    memcpy(dst, vertices, bytes);

     pVertices += 6;
#endif


    /*
       ..y5--+--+--+--+
       ..y4--|--|
       ..y1--|
       ..y0--|
       .....x0-x1-x4-x5
     */

    const int bytes = 9*6*4 * sizeof(float);
    float *dst = (float*) pArray->lastDataAndIncrement( bytes );

    MxRectF translatedTarget = target.translated(*pTranslation);
    // vertices
    float x0 = translatedTarget.left();
    float x1 = x0 + margins.left();
    float x5 = translatedTarget.right();
    float x4 = x5 - margins.right();

    float y0, y1, y4, y5;
    y0 = translatedTarget.bottom();
    y1 = y0 + margins.bottom();
    y5 = translatedTarget.top();
    y4 = translatedTarget.top() - margins.top();


    // texture coordinates
   const MxRectF &textureRect = style->iconRect(name);
    float u0, u1, u4, u5;

    MxVector2F pixelTexFactor;
            style->getPixelSize( &pixelTexFactor );
    float pixelsToTexFactor = pixelTexFactor.width();//textureRect.width() / (float)imageSize.width();
    u0 = textureRect.left();
    u1 = u0 + (margins.left() * pixelsToTexFactor);
    u5 = textureRect.right();
    u4 = u5 - (margins.right() * pixelsToTexFactor);



    float v0, v1, v4, v5;
    pixelsToTexFactor = pixelTexFactor.height();//textureRect.height() / (float)imageSize.height();
    if( flipVertically )
    {
        v0 = textureRect.top();
        v1 = v0 - (margins.top() * pixelsToTexFactor);
        v5 = textureRect.bottom();
        v4 = v5 + (margins.bottom() * pixelsToTexFactor);
    }
    else
    {
        v0 = textureRect.bottom();
        v1 = v0 + (margins.bottom() * pixelsToTexFactor);
        v5 = textureRect.top();
        v4 = v5 - (margins.top() * pixelsToTexFactor);
    }



    // points are set from bottom to top, left to right
    GLfloat vertices[] = {
        x0, y0, u0, v0, // square 0,0
        x1, y0, u1, v0,
        x0, y1, u0, v1,
        x1, y0, u1, v0,
        x1, y1, u1, v1,
        x0, y1, u0, v1,

        x0, y1, u0, v1, // square 0,1
        x1, y1, u1, v1,
        x0, y4, u0, v4,
        x1, y1, u1, v1,
        x1, y4, u1, v4,
        x0, y4, u0, v4,

        x0, y4, u0, v4, // square 0,2
        x1, y4, u1, v4,
        x0, y5, u0, v5,
        x1, y4, u1, v4,
        x1, y5, u1, v5,
        x0, y5, u0, v5,

        x1, y0, u1, v0, // square 1,0
        x4, y0, u4, v0,
        x1, y1, u1, v1,
        x4, y0, u4, v0,
        x4, y1, u4, v1,
        x1, y1, u1, v1,

        x1, y1, u1, v1, // square 1,1
        x4, y1, u4, v1,
        x1, y4, u1, v4,
        x4, y1, u4, v1,
        x4, y4, u4, v4,
        x1, y4, u1, v4,

        x1, y4, u1, v4, // square 1,2
        x4, y4, u4, v4,
        x1, y5, u1, v5,
        x4, y5, u4, v5,
        x1, y5, u1, v5,
        x4, y4, u4, v4,

        x4, y0, u4, v0, // square 2,0
        x5, y0, u5, v0,
        x4, y1, u4, v1,
        x5, y1, u5, v1,
        x4, y1, u4, v1,
        x5, y0, u5, v0,

        x4, y1, u4, v1, // square 2,1
        x5, y1, u5, v1,
        x4, y4, u4, v4,
        x5, y1, u5, v1,
        x5, y4, u5, v4,
        x4, y4, u4, v4,

        x4, y4, u4, v4, // square 2,2
        x5, y4, u5, v4,
        x4, y5, u4, v5,
        x5, y5, u5, v5,
        x4, y5, u4, v5,
        x5, y4, u5, v4
    };

    memcpy(dst, vertices, bytes);

    //pVertices += 54;
}


void MxIconDraw::draw3x1Frame(const MxRectF &target, const MxVector2I &margins, const MxAbstractAtlas *style, MxThemeIcons::IconName name, bool flipVertically )
{
    Q_ASSERT( (NULL != pArray) && (NULL != pTranslation) );

    /*
       ..y1--+--+--+--+
       ..y0--|
       .....x0-x1-x4-x5
     */

    const int bytes = 3*6*4 * sizeof(float);
    float *dst = (float*) pArray->lastDataAndIncrement( bytes );

    MxRectF translatedTarget = target.translated(*pTranslation);

    // vertices
    float x0 = translatedTarget.left();
    float x1 = x0 + margins[0];
    float x5 = translatedTarget.right();
    float x4 = x5 - margins[1];


    float y0 = translatedTarget.bottom();
    float y1 = translatedTarget.top();

    // texture coordinates
   const MxRectF &textureRect = style->iconRect(name);
    float u0, u1, u4, u5;

    MxVector2F pixelTexFactor;
    style->getPixelSize( &pixelTexFactor );
    float pixelsToTexFactor = pixelTexFactor.width();
    u0 = textureRect.left();
    u1 = u0 + (margins[0] * pixelsToTexFactor);
    u5 = textureRect.right();
    u4 = u5 - (margins[1] * pixelsToTexFactor);



    float v0, v5;
    if(flipVertically)
    {
        v5 = textureRect.bottom();
        v0 = textureRect.top();
    }
    else
    {
        v0 = textureRect.bottom();
        v5 = textureRect.top();
    }


        // points are set from bottom to top, left to right
        GLfloat vertices[] = {
            x0, y0, u0, v0, // square 0
            x1, y0, u1, v0,
            x0, y1, u0, v5,
            x1, y0, u1, v0,
            x1, y1, u1, v5,
            x0, y1, u0, v5,

            x1, y0, u1, v0, // square 1
            x4, y0, u4, v0,
            x1, y1, u1, v5,
            x4, y0, u4, v0,
            x4, y1, u4, v5,
            x1, y1, u1, v5,

            x4, y0, u4, v0, // square 2
            x5, y0, u5, v0,
            x4, y1, u4, v5,
            x5, y0, u5, v0,
            x5, y1, u5, v5,
            x4, y1, u4, v5
        };



memcpy(dst, vertices, bytes);

    // \TODO optimize this: put both arrays in one
    //drawTriangleStrip__( textureCoord, 2,vertices, 3, 8);

        //pVertices += 18;
}



