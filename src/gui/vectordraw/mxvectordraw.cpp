/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxvectordraw.h"
#include "mxopengl.h"
#include "mxrenderer.h"
#include <cmath> // sqrt and pow


MxVectorDraw::MxVectorDraw()
    :pArray(NULL), pTranslation(NULL)
{
}

MxVectorDraw::MxVectorDraw( MxRenderer &renderer , MxVector2F *translation )
{
    pArray = renderer.newGpuBuffer( MxShaderProgram::Float2_UChar4_Float2 );
    pTranslation = translation;
    Q_ASSERT( pArray->size() == 0 );
}

MxVectorDraw::MxVectorDraw( GpuBuffer *vbo, MxVector2F *translation )
{
    pArray = vbo;
    pTranslation = translation;
}

void MxVectorDraw::clear()
{
    Q_ASSERT( NULL != pArray );
    Q_ASSERT( NULL != pTranslation );

    pArray->clear();
}

int MxVectorDraw::pointCount() const
{
    Q_ASSERT( NULL != pArray );
    Q_ASSERT( NULL != pTranslation );

    return (pArray->size() / sizeof(Vertex));
}


void MxVectorDraw::loadingSymbol( const MxVector2F &center, float radius )
{

    // \TODO make it a private data member or pass in as argument
    static float sProgressAngle = 0.0f;

    MxVector2F triangleCenter = center;
    float thirdCircleAngle = 2.0944;
    MxVector2F posA = triangleCenter + MxVector2F( radius * cosf(sProgressAngle) , radius * sinf(sProgressAngle) );
    MxVector2F posB = triangleCenter + MxVector2F( radius * cosf(sProgressAngle+thirdCircleAngle) , radius * sinf(sProgressAngle+thirdCircleAngle) );
    MxVector2F posC = triangleCenter + MxVector2F( radius * cosf(sProgressAngle-thirdCircleAngle) , radius * sinf(sProgressAngle-thirdCircleAngle) );

    MxVector4UC colorA(30, 200, 90, 255);
    MxVector4UC colorB(210, 20, 9, 255);
    MxVector4UC colorC(30, 110, 190, 255);

    this->triangle(posB, colorA, posA, colorB, posC, colorC, MxVectorDraw::FullFill /*MxVectorDraw::OutterCurveFill*/ );
    sProgressAngle += 0.01f;
}


void MxVectorDraw::roundedRect( const MxRectF &area, float radius, const MxVector4UC &color )
{
    Q_ASSERT( NULL != pArray );
    Q_ASSERT( NULL != pTranslation );

    pArray->reserveForAppend( 3*14*sizeof(Vertex) ); // 14 triangles

    MxRectF rect = area;
    rect.translate(*pTranslation);

    /*

     p04--p14-----p24--p34
     |     |       |    |
     p03--p13-----p23--p33
     |     |       |    |
     p02--p12-----p22--p32
     |     |       |    |
     p01--p11-----p21--p31

     */

    MxVector2F p04 = rect.topLeft();
    MxVector2F p03( p04.x() , p04.y() - radius );
    MxVector2F p14( p04.x() + radius, p04.y() );
    appendTriangle_p( p14, p04, p03, 1.0f, color.constData() );

    MxVector2F p01 = rect.bottomLeft();
    MxVector2F p02( p01.x() , p01.y() + radius );
    MxVector2F p11( p01.x() + radius, p01.y() );
    appendTriangle_p( p02, p01, p11, 1.0f, color.constData() );

    MxVector2F p34 = rect.topRight();
    MxVector2F p24( p34.x() - radius, p34.y() );
    MxVector2F p33( p34.x(), p34.y() - radius );
    appendTriangle_p( p33, p34, p24, 1.0f, color.constData() );

    MxVector2F p31 = rect.bottomRight();
    MxVector2F p32( p31.x() , p31.y() + radius );
    MxVector2F p21( p31.x() - radius, p31.y() );
    appendTriangle_p( p21, p31, p32, 1.0f, color.constData() );


    // fill rectangles
    MxVector2F p13( p04.x() + radius , p04.y() - radius );
    MxVector2F p12( p01.x() + radius , p01.y() + radius );
    MxVector2F p22( p31.x() - radius , p31.y() + radius );
    MxVector2F p23( p34.x() - radius , p34.y() - radius );

    appendRect_p( p13, p22, p14, p24, color );
    appendRect_p( p02, p32, p03, p33, color );
    appendRect_p( p11, p21, p12, p22, color );


    appendFillTriangle_p( p03, p13, p14,  color.constData() );
    appendFillTriangle_p( p11, p12, p02,  color.constData() );
    appendFillTriangle_p( p32, p22, p21,  color.constData() );
    appendFillTriangle_p( p24, p23, p33,  color.constData() );

}


void MxVectorDraw::addSquaredCircle( MxVector2F center, const MxVector2F &radius, const MxVector4UC &color )
{
    Q_ASSERT( NULL != pArray );
    Q_ASSERT( NULL != pTranslation );

    center += *pTranslation;

    pArray->reserveForAppend( 8*3*sizeof(Vertex) ); // 8 triangles

    MxVector2F left, right, bottom, top;
    left = center;
    left[0] -= radius[0];
    right = center;
    right[0] += radius[0];
    bottom = center;
    bottom[1] -= radius[1];
    top = center;
    top[1] += radius[1];

    MxVector2F corner( right.x(), top.y() );
    appendTriangle_p( top, corner, right, 1.0f, color.constData() );
    appendFillTriangle_p( top, center, right, color.constData() );

    corner = MxVector2F( right.x(), bottom.y() );
    appendTriangle_p( bottom, corner, right, 1.0f, color.constData() );
    appendFillTriangle_p( bottom, center, right, color.constData() );

    corner = MxVector2F( left.x(), bottom.y() );
    appendTriangle_p( bottom, corner, left, 1.0f, color.constData() );
    appendFillTriangle_p( bottom, center, left, color.constData() );

    corner = MxVector2F( left.x(), top.y() );
    appendTriangle_p( top, corner, left, 1.0f, color.constData() );
    appendFillTriangle_p( top, center, left, color.constData() );

}
/*
void LoopBlinnTrianglesArray::addRect( const QxRectF &rect, const QxVector4UC &color )
{
    addRect( rect.bottomLeft(), rect.bottomRight(), rect.topLeft(), rect.topRight(), color );
}*/



void MxVectorDraw::stroke( const MxVector2F &p1, const MxVector2F &p2, float thickness, const MxVector4UC &color )
{
    Q_ASSERT( NULL != pArray );
    Q_ASSERT( NULL != pTranslation );

    MxVector2F pA = p1 + *pTranslation;
    MxVector2F pB = p2 + *pTranslation;

    pArray->reserveForAppend(  2 * (3*sizeof(Vertex)) ); // 2 triangles


    const float halfThickness = thickness / 2.0f;
    MxVector2F vec = pB - pA;
    float length = sqrt( vec.x() * vec.x() + vec.y() * vec.y() );
    vec /= length;
    MxVector2F diagX( -1.0f * vec.y(), vec.x() );
    MxVector2F diagY( vec.y(), -1.0f * vec.x() );
    MxVector2F pAX = pA + (diagX * halfThickness);
    MxVector2F pAY = pA + (diagY * halfThickness);

    MxVector2F pBX = pB + (diagX * halfThickness);
    MxVector2F pBY = pB + (diagY * halfThickness);

    appendFillTriangle_p( pAX, pAY, pBY, &(color[0]) );
    appendFillTriangle_p( pBY, pBX, pAX, &(color[0]) );


    /*
    QxVector2F from = p1 ;//+ (*pTranslation);
    QxVector2F to = p2 ;//+ (*pTranslation);

    QxVector2F dir = to - from;
    QxVector2F perpendicular( dir.y(), dir.x() );
    float length = sqrt( pow(perpendicular.x(),2) + pow(perpendicular.y(),2) );
    perpendicular /= length;

    QxVector2F c = from + (perpendicular * (thickness/2.0f));

    appendFillTriangle( c, from, to, color.constData() );*/
}

void MxVectorDraw::strokeRect( MxRectF outerRect, float innerThickness, const MxVector4UC &color )
{
    Q_ASSERT( NULL != pArray );
    Q_ASSERT( NULL != pTranslation );

    outerRect.translate( pTranslation->x(), pTranslation->y() );

    pArray->reserveForAppend( 8 * 3*sizeof(Vertex) ); // 8 triangles

    MxRectF innerRect = outerRect;
    innerRect.adjust( innerThickness, -innerThickness, innerThickness, -innerThickness );

    // outside rect
    MxVector2F oTopRight = outerRect.topRight();
    MxVector2F oTopLeft = outerRect.topLeft();
    MxVector2F oBottomLeft = outerRect.bottomLeft();
    MxVector2F oBottomRight = outerRect.bottomRight();

    // inside rect
    MxVector2F iTopRight = innerRect.topRight();
    MxVector2F iTopLeft = innerRect.topLeft();
    MxVector2F iBottomLeft = innerRect.bottomLeft();
    MxVector2F iBottomRight = innerRect.bottomRight();

    appendFillTriangle_p( iTopRight, oTopRight, oTopLeft, &(color[0]) );
    appendFillTriangle_p( iTopRight, iTopLeft, oTopLeft, &(color[0]) );
    appendFillTriangle_p( oTopLeft, oBottomLeft, iBottomLeft, &(color[0]) );
    appendFillTriangle_p( iBottomLeft, iTopLeft, oTopLeft, &(color[0]) );
    appendFillTriangle_p( oBottomLeft, oBottomRight, iBottomRight, &(color[0]) );
    appendFillTriangle_p( iBottomRight, iBottomLeft, oBottomLeft, &(color[0]) );
    appendFillTriangle_p( oBottomRight, oTopRight, iTopRight, &(color[0]) );
    appendFillTriangle_p( iTopRight, iBottomRight, oBottomRight, &(color[0]) );

}


void MxVectorDraw::circle( MxVector2F center, float radius, const MxVector4UC &color )
{
    Q_ASSERT( NULL != pArray );
    Q_ASSERT( NULL != pTranslation );

    center += *pTranslation;

    MxVector2F inPoints[4];

    inPoints[0] = center;
    inPoints[0][1] += radius;
    inPoints[1] = inPoints[0];
    inPoints[1][0] += radius/2.0f;
    inPoints[3] = center;
    inPoints[3][0] += radius;
    inPoints[2] = inPoints[3];
    inPoints[2][1] += radius/2.0f;
    addCubicArc_p( center, inPoints, color );

    inPoints[1] = inPoints[0];
    inPoints[1][0] -= radius/2.0f;
    inPoints[3] = center;
    inPoints[3][0] -= radius;
    inPoints[2] = inPoints[3];
    inPoints[2][1] += radius/2.0f;
    addCubicArc_p( center, inPoints, color );

    inPoints[0] = center;
    inPoints[0][0] -= radius;
    inPoints[1] = inPoints[0];
    inPoints[1][1] -= radius/2.0f;
    inPoints[3] = center;
    inPoints[3][1] -= radius;
    inPoints[2] = inPoints[3];
    inPoints[2][0] -= radius/2.0f;
    addCubicArc_p( center, inPoints, color );

    inPoints[0] = center;
    inPoints[0][1] -= radius;
    inPoints[1] = inPoints[0];
    inPoints[1][0] += radius/2.0f;
    inPoints[3] = center;
    inPoints[3][0] += radius;
    inPoints[2] = inPoints[3];
    inPoints[2][1] -= radius/2.0f;
    addCubicArc_p( center, inPoints, color );

}


/*
void LoopBlinnTrianglesArray::uploadGL()
{
    Q_ASSERT( NULL != pArray );
    Q_ASSERT( NULL != pTranslation );

    if ( pArray->pVbo == 0 )
    {
        glGenBuffers( 1,&(pArray->pVbo) );
        Q_ASSERT(pArray->pVbo);
    }

    //QX_CHECK_GLERROR();

    // make sure the buffer is bound, don't perform any checks because size may be zero
    glBindBuffer(GL_ARRAY_BUFFER, pArray->pVbo);

    //QX_CHECK_GLERROR();


    // if the buffer has already been created, just update the data providing it fits
    if ( pArray->pVboSize > 0 )
    {
        // if the data will fit in the existing buffer, just update it
        if( pArray->pSize <= pArray->pVboSize )
        {
            glBufferSubData(GL_ARRAY_BUFFER,0, pArray->pSize, pArray->data() );
        }
        else
        {
            // create a new buffer of the larger size,
            // gl should automatically deallocate the old buffer
            glBufferData(GL_ARRAY_BUFFER, pArray->pSize, pArray->data(), GL_DYNAMIC_DRAW);
            pArray->pVboSize = pArray->pSize;
        }
    }
    else
    {
        // create the buffer
        glBufferData(GL_ARRAY_BUFFER, pArray->pSize, pArray->data(), GL_DYNAMIC_DRAW);
        pArray->pVboSize = pArray->pSize;
    }

    QX_CHECK_GLERROR();

}*/

void MxVectorDraw::triangle(const MxVector2F &posA, const MxVector4UC &colorA,
                            const MxVector2F &posB, const MxVector4UC &colorB,
                            const MxVector2F &posC, const MxVector4UC &colorC, TriangleFill fill )
{
    Q_ASSERT( NULL != pArray );
    Q_ASSERT( NULL != pTranslation );

    pArray->reserveForAppend( 3*sizeof(Vertex) );

    float direction = 1.0f;
    float uvs[6];
    if( FullFill == fill ) {
        uvs[0] = 0.0f; uvs[1] = 1.0f;
        uvs[2] = 0.0f; uvs[3] = 1.0f;
        uvs[4] = 0.0f; uvs[5] = 1.0f;
    } else if( InnerCurveFill == fill ) {
        uvs[0] = 0.0f; uvs[1] = 0.0f;
        uvs[2] = 0.5f; uvs[3] = 0.0f;
        uvs[4] = 1.0f; uvs[5] = 1.0f;
    } else if( OutterCurveFill == fill ) {
        uvs[0] = 0.0f; uvs[1] = 0.0f;
        uvs[2] = 0.5f; uvs[3] = 0.0f;
        uvs[4] = 1.0f; uvs[5] = 1.0f;
        direction = -1.0f;
    }



    MxVectorDraw::Vertex *v = (MxVectorDraw::Vertex*)pArray->lastDataAndIncrement( 3*sizeof(Vertex) );
    v->x = posA.x() + pTranslation->x();
    v->y = posA.y() + pTranslation->y();
    v->u = uvs[0];
    v->v = uvs[1];
    v->r = colorA[0];
    v->g = colorA[1];
    v->b = colorA[2];
    v->a = colorA[3] * direction;

    v++;
    v->x = posB.x() + pTranslation->x();
    v->y = posB.y() + pTranslation->y();
    v->u = uvs[2];
    v->v = uvs[3];
    v->r = colorB[0];
    v->g = colorB[1];
    v->b = colorB[2];
    v->a = colorB[3] * direction;

    v++;
    v->x = posC.x() + pTranslation->x();
    v->y = posC.y() + pTranslation->y();
    v->u = uvs[4];
    v->v = uvs[5];
    v->r = colorC[0];
    v->g = colorC[1];
    v->b = colorC[2];
    v->a = colorC[3] * direction;
}

void MxVectorDraw::fillRect( const MxRectF &rect, const MxVector4UC &color )
{
    Q_ASSERT( NULL != pArray );
    Q_ASSERT( NULL != pTranslation );

    pArray->reserveForAppend( 6*sizeof(Vertex) );
    /*
    float fColor[4];
    fColor[0] = color[0] / 255.0f;
    fColor[1] = color[1] / 255.0f;
    fColor[2] = color[2] / 255.0f;
    fColor[3] = color[3] / 255.0f;
*/

    MxVectorDraw::Vertex *v = (MxVectorDraw::Vertex*)pArray->lastDataAndIncrement( 6*sizeof(Vertex) );
    v->x = rect.left() + pTranslation->x();
    v->y = rect.top() + pTranslation->y();
    v->u = 0.0f;
    v->v = 1.0f;
    v->r = color[0];
    v->g = color[1];
    v->b = color[2];
    v->a = color[3];

    v++;
    v->x = rect.left() + pTranslation->x();
    v->y = rect.bottom() + pTranslation->y();
    v->u = 0.0f;
    v->v = 1.0f;
    v->r = color[0];
    v->g = color[1];
    v->b = color[2];
    v->a = color[3];

    v++;
    v->x = rect.right() + pTranslation->x();
    v->y = rect.bottom() + pTranslation->y();
    v->u = 0.0f;
    v->v = 1.0f;
    v->r = color[0];
    v->g = color[1];
    v->b = color[2];
    v->a = color[3];


    v++;
    v->x = rect.left() + pTranslation->x();
    v->y = rect.top() + pTranslation->y();
    v->u = 0.0f;
    v->v = 1.0f;
    v->r = color[0];
    v->g = color[1];
    v->b = color[2];
    v->a = color[3];

    v++;
    v->x = rect.right() + pTranslation->x();
    v->y = rect.bottom() + pTranslation->y();
    v->u = 0.0f;
    v->v = 1.0f;
    v->r = color[0];
    v->g = color[1];
    v->b = color[2];
    v->a = color[3];

    v++;
    v->x = rect.right() + pTranslation->x();
    v->y = rect.top() + pTranslation->y();
    v->u = 0.0f;
    v->v = 1.0f;
    v->r = color[0];
    v->g = color[1];
    v->b = color[2];
    v->a = color[3];
}

void MxVectorDraw::gradientRect(const MxRectF &rect, const MxVector4UC &bottomLeftColor, const MxVector4UC &bottomRightColor, const MxVector4UC &topLeftColor, const MxVector4UC &topRightColor )
{
    Q_ASSERT( NULL != pArray );
    Q_ASSERT( NULL != pTranslation );

    pArray->reserveForAppend( 6*sizeof(Vertex) );

    MxVectorDraw::Vertex *v = (MxVectorDraw::Vertex*)pArray->lastDataAndIncrement( 6*sizeof(Vertex) );
    v->x = rect.left() + pTranslation->x();
    v->y = rect.top() + pTranslation->y();
    v->u = 0.0f;
    v->v = 1.0f;
    v->r = topLeftColor[0];
    v->g = topLeftColor[1];
    v->b = topLeftColor[2];
    v->a = topLeftColor[3];

    v++;
    v->x = rect.left() + pTranslation->x();
    v->y = rect.bottom() + pTranslation->y();
    v->u = 0.0f;
    v->v = 1.0f;
    v->r = bottomLeftColor[0];
    v->g = bottomLeftColor[1];
    v->b = bottomLeftColor[2];
    v->a = bottomLeftColor[3];

    v++;
    v->x = rect.right() + pTranslation->x();
    v->y = rect.bottom() + pTranslation->y();
    v->u = 0.0f;
    v->v = 1.0f;
    v->r = bottomRightColor[0];
    v->g = bottomRightColor[1];
    v->b = bottomRightColor[2];
    v->a = bottomRightColor[3];


    v++;
    v->x = rect.left() + pTranslation->x();
    v->y = rect.top() + pTranslation->y();
    v->u = 0.0f;
    v->v = 1.0f;
    v->r = topLeftColor[0];
    v->g = topLeftColor[1];
    v->b = topLeftColor[2];
    v->a = topLeftColor[3];

    v++;
    v->x = rect.right() + pTranslation->x();
    v->y = rect.bottom() + pTranslation->y();
    v->u = 0.0f;
    v->v = 1.0f;
    v->r = bottomRightColor[0];
    v->g = bottomRightColor[1];
    v->b = bottomRightColor[2];
    v->a = bottomRightColor[3];

    v++;
    v->x = rect.right() + pTranslation->x();
    v->y = rect.top() + pTranslation->y();
    v->u = 0.0f;
    v->v = 1.0f;
    v->r = topRightColor[0];
    v->g = topRightColor[1];
    v->b = topRightColor[2];
    v->a = topRightColor[3];
}

void MxVectorDraw::verticalGradientRect(const MxRectF &rect, const MxVector4UC &bottomColor, const MxVector4UC &topColor )
{
    gradientRect( rect, bottomColor, bottomColor, topColor, topColor);
}

void MxVectorDraw::roundedRectV4Gradient( const MxRectF &rect, float radius, const MxVector4UC colors[4]  )
{
    Q_ASSERT( NULL != pArray );
    Q_ASSERT( NULL != pTranslation );

    pArray->reserveForAppend( 14*sizeof(Vertex) );


    float x0 = rect.left();
    float x1 = x0 + radius;
    float x3 = rect.right();
    float x2 = x3 - radius;

    float y0 = rect.bottom();
    float y1 = y0 + radius;
    float y3 = rect.top();
    float y2 = y3 - radius;

    // bottom-left corner
    triangle( MxVector2F(x0, y1), colors[1],
            MxVector2F(x0, y0) , colors[0],
            MxVector2F(x1, y0), colors[0], MxVectorDraw::InnerCurveFill );
    triangle( MxVector2F(x1, y0), colors[0],
            MxVector2F(x1, y1) , colors[1],
            MxVector2F(x0, y1), colors[1], MxVectorDraw::FullFill );


    // bottom-right corner
    triangle( MxVector2F(x2, y0), colors[0],
            MxVector2F(x3, y0) , colors[0],
            MxVector2F(x3, y1), colors[1], MxVectorDraw::InnerCurveFill );
    triangle(  MxVector2F( x3, y1), colors[1],
            MxVector2F(x2, y1) , colors[1],
            MxVector2F( x2, y0), colors[0], MxVectorDraw::FullFill );



    MxRectF bottomRect( x1, x2, y0, y1 );
    verticalGradientRect( bottomRect, colors[0], colors[1] );


    MxRectF bodyRect( x0 , x3 , y1, y2 );
    verticalGradientRect( bodyRect, colors[1], colors[2] );


    // TOP
    triangle( MxVector2F(x0, y2), colors[2],
            MxVector2F(x0, y3), colors[3],
            MxVector2F(x1, y3) , colors[3], MxVectorDraw::InnerCurveFill );
    triangle(MxVector2F(x1, y3), colors[3],
            MxVector2F(x1, y2) , colors[2],
            MxVector2F(x0, y2), colors[2], MxVectorDraw::FullFill );


    triangle( MxVector2F(x2, y3), colors[3],
            MxVector2F(x3, y3), colors[3],
            MxVector2F(x3, y2) , colors[2], MxVectorDraw::InnerCurveFill );
    triangle(MxVector2F(x3, y2), colors[2],
            MxVector2F(x2, y2) , colors[2],
            MxVector2F(x2, y3), colors[3], MxVectorDraw::FullFill );

    MxRectF topRect( x1, x2, y2, y3);
    verticalGradientRect( topRect, colors[2], colors[3] );
}

void MxVectorDraw::appendChaplinHat(float left, float right,
                                    float bottom, float top, float radius, const MxVector4UC &color )
{

    triangle(
                MxVector2F(left, top), color,
                MxVector2F(left , bottom), color,
                MxVector2F(left + radius, bottom), color,
                MxVectorDraw::InnerCurveFill
                );

    triangle(
                MxVector2F(right, top), color,
                MxVector2F(right , bottom), color,
                MxVector2F(right - radius, bottom), color,
                MxVectorDraw::InnerCurveFill
                );

    // fill triangles
    triangle(
                MxVector2F(left, top), color,
                MxVector2F(right - radius , bottom), color,
                MxVector2F(left + radius, bottom), color,
                MxVectorDraw::FullFill
                );

    triangle(
                MxVector2F(left, top), color,
                MxVector2F(right, top), color,
                MxVector2F(right - radius, bottom), color,
                MxVectorDraw::FullFill
                );
}

void MxVectorDraw::appendTopRoundedRect( const MxRectF &area, float radius, const MxVector4UC &color )
{
    float top = area.top();
    float middle = area.top() - radius;
    float bottom = area.bottom();
    float left = area.left();
    float right = area.right();

    appendChaplinHat( left,
                      right,
                      top,
                      middle , radius, color );

    // fill triangles
    triangle(
                MxVector2F(left, middle), color,
                MxVector2F(right , bottom), color,
                MxVector2F(left, bottom), color,
                MxVectorDraw::FullFill
                );

    triangle(
                MxVector2F(left, middle), color,
                MxVector2F(right, middle), color,
                MxVector2F(right, bottom), color,
                MxVectorDraw::FullFill
                );
}

void MxVectorDraw::appendFillTriangle_p( const MxVector2F &a, const MxVector2F &b, const MxVector2F &c, const unsigned char *pathColor )
{
    Q_ASSERT( NULL != pArray );
    Q_ASSERT( NULL != pTranslation );

    //pArray->reserveForAppend( 3*sizeof(Vertex) );

    MxVectorDraw::Vertex *v = (MxVectorDraw::Vertex*)pArray->lastDataAndIncrement( 3*sizeof(Vertex) );//pArray.appendPointer();
    v->x = a.x();
    v->y = a.y();
    v->u = 0.0f;
    v->v = 1.0f;
    v->r = pathColor[0];
    v->g = pathColor[1];
    v->b = pathColor[2];
    v->a = pathColor[3];

    v++;
    v->x = b.x();
    v->y = b.y();
    v->u = 0.0f;
    v->v = 1.0f;
    v->r = pathColor[0];
    v->g = pathColor[1];
    v->b = pathColor[2];
    v->a = pathColor[3];

    v++;
    v->x = c.x();
    v->y = c.y();
    v->u = 0.0f;
    v->v = 1.0f;
    v->r = pathColor[0];
    v->g = pathColor[1];
    v->b = pathColor[2];
    v->a = pathColor[3];
}

void MxVectorDraw::appendColoredTriangle_p( const MxVector2F &a, const unsigned char *colorA, const MxVector2F &b, const unsigned char *colorB, const MxVector2F &c, const unsigned char *colorC )
{
    Q_ASSERT( NULL != pArray );
    Q_ASSERT( NULL != pTranslation );

    //pArray->reserveForAppend( 3*sizeof(Vertex) );

    const float alpha = 1.0f;

    MxVectorDraw::Vertex *v = (MxVectorDraw::Vertex*)pArray->lastDataAndIncrement( 3*sizeof(Vertex) ); //pArray.appendPointer();
    v->x = a.x();
    v->y = a.y();
    v->u = 0.0f;
    v->v = 0.0f;
    v->r = colorA[0];
    v->g = colorA[1];
    v->b = colorA[2];
    v->a = alpha;

    v++;
    v->x = b.x();
    v->y = b.y();
    v->u = 0.5f;
    v->v = 0.0f;
    v->r = colorB[0];
    v->g = colorB[1];
    v->b = colorB[2];
    v->a = alpha;

    v++;
    v->x = c.x();
    v->y = c.y();
    v->u = 1.0f;
    v->v = 1.0f;
    v->r = colorC[0];
    v->g = colorC[1];
    v->b = colorC[2];
    v->a = alpha;
}

void MxVectorDraw::appendTriangle_p(const MxVector2F &a, const MxVector2F &b, const MxVector2F &c, float direction, const unsigned char *pathColor )
{
    Q_ASSERT( NULL != pArray );
    Q_ASSERT( NULL != pTranslation );

    //pArray->reserveForAppend( 3*sizeof(Vertex) );

    const float alpha = pathColor[3] * direction;

    MxVectorDraw::Vertex *v = (MxVectorDraw::Vertex*)pArray->lastDataAndIncrement( 3*sizeof(Vertex) ); //pArray.appendPointer();
    v->x = a.x();
    v->y = a.y();
    v->u = 0.0f;
    v->v = 0.0f;
    v->r = pathColor[0];
    v->g = pathColor[1];
    v->b = pathColor[2];
    v->a = alpha;

    v++;
    v->x = b.x();
    v->y = b.y();
    v->u = 0.5f;
    v->v = 0.0f;
    v->r = pathColor[0];
    v->g = pathColor[1];
    v->b = pathColor[2];
    v->a = alpha;

    v++;
    v->x = c.x();
    v->y = c.y();
    v->u = 1.0f;
    v->v = 1.0f;
    v->r = pathColor[0];
    v->g = pathColor[1];
    v->b = pathColor[2];
    v->a = alpha;
}


void MxVectorDraw::appendRect_p( const MxVector2F &leftBottom, const MxVector2F &rightBottom, const MxVector2F &topLeft, const MxVector2F &topRight, const MxVector4UC &color )
{
    Q_ASSERT( NULL != pArray );
    Q_ASSERT( NULL != pTranslation );

    appendFillTriangle_p( topLeft, leftBottom, rightBottom, color.constData() );
    appendFillTriangle_p( rightBottom, topRight, topLeft, color.constData() );
}

void MxVectorDraw::addCubicArc_p( MxVector2F center, const MxVector2F inPoints[], const MxVector4UC &color )
{
    Q_ASSERT( NULL != pArray );
    Q_ASSERT( NULL != pTranslation );

    double eigth = 1.0 / 8.0;
    double sixteenth = 1.0 / 16.0;

    MxVector2F l = (inPoints[0] - (inPoints[1] * 3.0) +
            (inPoints[2] * 3.0) - inPoints[3]) *
            eigth;
    MxVector2F m = ((inPoints[0] * 3.0) + (inPoints[1] * 9.0) -
            (inPoints[2] * 3.0) - inPoints[3]) *
            sixteenth;
    MxVector2F n =
            ((inPoints[0] * -3.0) + (inPoints[1] * 3.0) +
            (inPoints[2] * 3.0) + (inPoints[3] * 5.0)) *
            sixteenth;
    MxVector2F p = bezierPoint(inPoints, 0.5);


    struct Triangle
    {
        MxVector2F mCoords[3];
    };

    Triangle t0 , t1;

    t0.mCoords[0] = inPoints[0];
    t1.mCoords[2] = inPoints[3];
    t0.mCoords[2] = p;
    t1.mCoords[0] = p;
    t0.mCoords[1] = l * 0.25 + m + p * 0.5;
    t1.mCoords[1] = l * 0.75 + n + p * 0.5;

    appendTriangle_p( t0.mCoords[0], t0.mCoords[1], t0.mCoords[2], 1.0f, color.constData() );
    appendTriangle_p( t1.mCoords[0], t1.mCoords[1], t1.mCoords[2], 1.0f, color.constData() );

    appendFillTriangle_p( center, t0.mCoords[0], t0.mCoords[2], color.constData() );
    appendFillTriangle_p( center, t1.mCoords[0], t1.mCoords[2], color.constData() );
}


MxVector2F MxVectorDraw::bezierPoint( const MxVector2F inPoints[4], double inT )
{
    return (inPoints[0] * pow(1.0 - inT, 3)) +
            (inPoints[1] * 3.0 * (pow(1.0 - inT, 2) * inT)) +
            (inPoints[2] * 3.0 * ((1.0 - inT) * pow(inT, 2))) +
            (inPoints[3] * pow(inT, 3));
}


/*! \class MxVectorDraw
    \brief Draw Mewa triangles

    A Mewa triagle draws a filled quadratic curve.
     
    ![Mewa Triangle](MewaTriangle.jpg)

    Why Mewa triangles?
    Mewa triangles are an extremely efficient 
    method for drawing scale-invariant, antialiased curves.

    \sa MxPainter
*/
