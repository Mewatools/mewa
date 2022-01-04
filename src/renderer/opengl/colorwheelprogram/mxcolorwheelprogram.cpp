/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <MxColorWheelProgram@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxdebug.h"
#include "mxcolorwheelprogram.h"
#include "mxrenderer.h"

#include <cmath> // atan


MxVector3F MxColorWheelProgram::colorAt( const MxRectF &rect, const MxVector2F &coord )
{
    MxVector3F c;
    MxVector2F  normCoord = (coord - rect.bottomLeft()) / rect.size();
    MxVector2F centerPt(0.5f, 0.5f);

    // distance
    float dx = (normCoord.x() - centerPt.x());
    float dy = (normCoord.y()- centerPt.y());
    float rp = sqrt((dx*dx) + (dy*dy));
    //qDebug("rp: %f", rp);
    if( rp > 0.5 ) {
        c = MxVector3F(0.0f, 0.0f, 0.0f);
    }
    else
    {
        MxVector2F direction = normCoord - centerPt;
        float angle = atan2(direction.y() , direction.x());

        const float pi = 3.14159265f;
        if( angle > (2.0*pi / 3.0) )
        {
            float normAngle = (angle - (2.0*pi / 3.0)) / (pi / 3.0);
            // qDebug("angle: %f", normAngle * 180/pi);
            c = MxVector3F(0.0, 1.0, normAngle);
        }
        else if( angle > (pi / 3.0) )
        {
            float normAngle = (angle - (pi / 3.0)) / (pi / 3.0);
            // qDebug("angle: %f", normAngle * 180/pi);
            c = MxVector3F(1.0-normAngle, 1.0, 0.0);
        }
        else if( angle > 0.0 )
        {
            float normAngle = angle / (pi / 3.0);
            // qDebug("angle: %f", normAngle * 180/pi);
            c = MxVector3F(1.0, normAngle, 0.0);

        }
        else if( angle < (-2.0*pi / 3.0) )
        {
            float normAngle = (angle + (2.0*pi / 3.0)) / (-pi / 3.0);
            // qDebug("angle: %f", normAngle * 180/pi);
            c = MxVector3F(0.0, normAngle, 1.0);
        }
        else if( angle < (-pi / 3.0) )
        {
            float normAngle = (angle + (pi / 3.0)) / (-pi / 3.0);
            // qDebug("angle: %f", normAngle * 180/pi);
            c = MxVector3F( 1.0-normAngle, 0.0, 1.0);
        }
        else
        {
            float normAngle = angle / (-pi / 3.0);
            // qDebug("angle: %f", normAngle * 180/pi);
            c = MxVector3F(1.0, 0.0, normAngle);
        }
    }

    return c;
}

MxColorWheelProgram::MxColorWheelProgram()
{
    pUpdates = UpdateAll;
    pBackgroundColor = MxVector3F(0.0f, 0.0f, 0.0f);
    pSmoothEdge = 0.01f;
}

MxColorWheelProgram::~MxColorWheelProgram()
{}

void MxColorWheelProgram::init( MxRenderer *renderer )
{
    Q_ASSERT( NULL == pRenderer );
    pRenderer = renderer;
}

void MxColorWheelProgram::initializeGL()
{
    // TODO: change highp with mediump
    GLuint vshader = pRenderer->glCreateShader(GL_VERTEX_SHADER);

    const char *vsrc =
            "attribute vec4 vertex;\n"
            "uniform mat4 matrix;\n"
            "varying vec2 pixelCoord;\n"
            "void main(void)\n"
            "{\n"
            "    gl_Position = matrix * vec4(vertex.xy, 0.0, 1.0);\n"
            "    pixelCoord = vertex.zw;\n"
            "}\n";
    pRenderer->glShaderSource(vshader, 1, &vsrc, NULL);
    pRenderer->glCompileShader(vshader);
    GLint compiled;
    pRenderer->glGetShaderiv(vshader, GL_COMPILE_STATUS, &compiled);

    qDebug("MxColorWheelProgram : vertex shader compiled: %d", compiled);


    GLuint fshader = pRenderer->glCreateShader(GL_FRAGMENT_SHADER);
    const char *fsrc =
        #ifdef MX_OPENGL_ES
            "precision mediump float;\n"
        #endif
            "#define pi 3.141592653589793238462643383279\n"
            "varying vec2 pixelCoord;\n"
            "uniform vec3 background;\n"
            "uniform float uSmoothness;\n"
            "void main(void)\n"
            "{\n"
            "vec2 normPixelCoord = pixelCoord;"
            "vec2 centerPt = vec2(0.5, 0.5);"
            "float rp = distance(centerPt, normPixelCoord);"
            //"if( rp > 0.5 ) {"
            //"gl_FragColor = vec4( background, 1.0); }"
            //"else {"
            "vec2 direction = normPixelCoord - centerPt;"
            "float angle = atan(direction.y, direction.x);"

            "if( angle > (2.0*pi / 3.0) ) {"
            "float normAngle = (angle - (2.0*pi / 3.0)) / (pi / 3.0);"
            "gl_FragColor = vec4(0.0, 1.0, normAngle, 1.0);\n"

            "} else if( angle > (pi / 3.0) ) {"
            "float normAngle = (angle - (pi / 3.0)) / (pi / 3.0);"
            "gl_FragColor = vec4(1.0-normAngle, 1.0, 0.0, 1.0);\n"

            "} else if( angle > 0.0 ) {"
            "float normAngle = angle / (pi / 3.0);"
            "gl_FragColor = vec4(1.0, normAngle, 0.0, 1.0);\n"

            "} else if( angle < (-2.0*pi / 3.0) ) {"
            "float normAngle = (angle + (2.0*pi / 3.0)) / (-pi / 3.0);"
            "gl_FragColor = vec4(0.0, normAngle, 1.0, 1.0);\n"

            "} else if( angle < (-pi / 3.0) ) {"
            "float normAngle = (angle + (pi / 3.0)) / (-pi / 3.0);"
            "gl_FragColor = vec4( 1.0-normAngle, 0.0, 1.0, 1.0);\n"

            "} else {"
            "float normAngle = angle / (-pi / 3.0);"
            "gl_FragColor = vec4(1.0, 0.0, normAngle, 1.0); }\n"
            //"}"
            //"float edge = 1.0/144.0;"
            "gl_FragColor = mix(gl_FragColor, vec4( background, 1.0), smoothstep(0.5 - uSmoothness, 0.5 + uSmoothness, rp) );"

            "}\n";

    pRenderer->glShaderSource(fshader, 1, &fsrc, NULL);
    pRenderer->glCompileShader(fshader);

    pRenderer->glGetShaderiv(fshader, GL_COMPILE_STATUS, &compiled);
    qDebug("MxColorWheelProgram : fragment shader compiled: %d", compiled);


    mProgramId = pRenderer->glCreateProgram();
    pRenderer->glAttachShader(mProgramId, vshader);
    pRenderer->glAttachShader(mProgramId, fshader);
    pRenderer->glLinkProgram(mProgramId);
    GLint value;
    pRenderer->glGetProgramiv(mProgramId, GL_LINK_STATUS, &value);
    qDebug("MxColorWheelProgram : shader program linked: %d", value);
    value = 0;
    pRenderer->glGetProgramiv(mProgramId, GL_INFO_LOG_LENGTH, &value);
    qDebug("MxColorWheelProgram : program GL_INFO_LOG_LENGTH: %d", value);
#ifdef MX_PLATFORM_ANDROID
    if(value > 0)
    {
        char str[90];
        GLsizei str_length = 0;
        glGetProgramInfoLog(mProgramId,90, &str_length, str);
        //qDebug() << str;
        LOGI("glGetProgramInfoLog: %s", str);
    }
#endif


    pVertexAttrib = pRenderer->glGetAttribLocation(mProgramId, "vertex");
    pMatrixUniform = pRenderer->glGetUniformLocation(mProgramId, "matrix");
    pColorUniform = pRenderer->glGetUniformLocation(mProgramId, "background");
    pSmoothnessUniform = pRenderer->glGetUniformLocation(mProgramId, "uSmoothness");
}

void MxColorWheelProgram::setMatrix( const MxMatrix &matrix )
{
    pModelview = matrix;
    pUpdates |= UpdateMatrix;
}


void MxColorWheelProgram::setBackgroundColor( float r, float g, float b )
{
    if( pBackgroundColor[0] != r ) { // optimization ??
        pBackgroundColor = MxVector3F(r, g, b);
        pUpdates |= BackgroundColor;
    }
}

void MxColorWheelProgram::setSmoothEdge( float threshold )
{
    if( pSmoothEdge != threshold ) { // optimization ??
        pSmoothEdge = threshold;
        pUpdates |= UpdateSmoothness;
    }
}

void MxColorWheelProgram::draw(const MxRectF &rect , MxRenderer &renderer )
{

    GLfloat afVertices[] = {
        rect.left(), rect.bottom(), 0.0f, 0.0f,
        rect.right(),rect.bottom(), 1.0f, 0.0f,
        rect.left(),rect.top(), 0.0f, 1.0f,
        rect.right(), rect.top(), 1.0f, 1.0f,
        rect.left(),rect.top(), 0.0f, 1.0f,
        rect.right(),rect.bottom(), 1.0f, 0.0f
    };

    // \TODO should avoid uploading the same data everytime ??
    MxGpuArray *gpuArray = pRenderer->uploadToGpu( vaoFormat(), (char*)afVertices, 6*4*sizeof(GLfloat) );


    enableVao(gpuArray);

    if( pUpdates & BackgroundColor ) {
        pRenderer->glUniform3fv(pColorUniform, 1, pBackgroundColor.constData());
    }

    if( pUpdates & UpdateSmoothness ) {
        pRenderer->glUniform1f(pSmoothnessUniform, pSmoothEdge);
    }

    if( pUpdates & UpdateMatrix )
    {
        pRenderer->glUniformMatrix4fv(pMatrixUniform, 1, GL_FALSE, pModelview.constData());
    }
    pUpdates = 0;
    pRenderer->glDrawArrays(GL_TRIANGLES, 0, 6);
    disableVao();
}


void MxColorWheelProgram::enable()
{
    pUpdates = UpdateAll;
    MxShaderProgram::enable();
}


void MxColorWheelProgram::disableVao()
{
    pRenderer->glBindVertexArray(0);
    pRenderer->glBindBuffer(GL_ARRAY_BUFFER, 0); // \TODO needed ?
}

void MxColorWheelProgram::enableAttributes()
{
    pRenderer->glEnableVertexAttribArray(pVertexAttrib);
    pRenderer->glVertexAttribPointer(pVertexAttrib, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

void MxColorWheelProgram::disableAttributes()
{
    pRenderer->glBindBuffer(GL_ARRAY_BUFFER, 0);
    pRenderer->glDisableVertexAttribArray(pVertexAttrib);
}
