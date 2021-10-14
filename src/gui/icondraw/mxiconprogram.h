/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXICONPROGRAM_H
#define MXICONPROGRAM_H

#include "mxdebug.h"
#include "mxmatrix.h"
#include "mxvector.h"
#include "mxopengl.h"
#include "mxshaderprogram.h"
#include "mxrect.h"


class MxIconDraw;
//class GpuBuffer;



class MxIconProgram : public MxShaderProgram
{
public:
    MxIconProgram();
    virtual ~MxIconProgram();

    void init( MxRenderer *renderer );
    void initializeGL();

    void setModelViewMatrix( const MxMatrix &matrix );
    MxMatrix modelViewMatrix() const;

    enum ColorFilter {
        IdentityFilter,
        LightGrayFilter,
        BlueFilter,
        DarkGrayFilter,

    };

    void setColorFilter(const ColorFilter filter );
    //void drawTextureRectsArray( GpuBuffer &array );
    void drawRects(MxIconDraw &rectsArray );
    void drawTriangleStrip( const GLfloat *textureValues, const GLfloat *destValues, int numPoints );

    static MxShaderProgram::VaoFormat getVaoFormat();
    virtual VaoFormat vaoFormat(); // \TODO delete this member
    virtual void enableAttributes(); // \TODO add arg VaoFormat to this member
    void enableAttributes( VaoFormat format ); // \TODO add arg VaoFormat to this member
    virtual void disableAttributes();



public:
    void updateUniformValues();



    enum Update
    {
        UpdateMatrix = 0x00000001,
        UpdateAlpha = 0x00000002,
        UpdateAll = 0x7FFFFFFF
    };

    // GLuint pProgram;
    //QGLShaderProgram pProgram;

    MxMatrix pModelview;
    //QMatrix4x4 pModelview;


    //QxVector4F pAlpha;
    ColorFilter pColorFilter;
    //QxVector4F pColor;
    int pUpdates;
public:
    // gl
    GLint vertexAttr2;
    GLint texCoordAttr2;
    GLint matrixUniform2;
    GLint pColorUniform;
    GLint textureUniform2;

};


#endif
