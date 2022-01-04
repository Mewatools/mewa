/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxiconprogram.h"
#include "mxrect.h"
#include "mxicondraw.h"
#include "mxrenderer.h"


/*! Creates a MxIconProgram
*/
MxIconProgram::MxIconProgram()
{
    pColorFilter = IdentityFilter;
    pUpdates = UpdateAll;
}


MxIconProgram::~MxIconProgram()
{
}

void MxIconProgram::init( MxRenderer *renderer )
{
    Q_ASSERT( NULL == pRenderer );
    pRenderer = renderer;
}

void MxIconProgram::initialize()
{
    Q_ASSERT( NULL != pRenderer );

    // TODO: change highp with mediump
    GLuint vshader = pRenderer->glCreateShader(GL_VERTEX_SHADER);

    const char *vsrc =
            "attribute vec4 vertex;\n"
            "attribute vec4 texcoord;\n"
            "uniform mat4 matrix;\n"
            "varying vec4 qt_TexCoord0;\n"
            "void main(void)\n"
            "{\n"
            "    gl_Position = matrix * vertex;\n"
            "    qt_TexCoord0 = texcoord;\n"
            "}\n";
    pRenderer->glShaderSource(vshader, 1, &vsrc, NULL);
    pRenderer->glCompileShader(vshader);
    GLint compiled;
    pRenderer->glGetShaderiv(vshader, GL_COMPILE_STATUS, &compiled);

    qDebug("MxAlphaTexture vertex shader compiled: %d\n", compiled);


    GLuint fshader = pRenderer->glCreateShader(GL_FRAGMENT_SHADER);
    const char *fsrc =
        #ifdef QX_OPENGL_ES_3
            "precision mediump float;\n"
        #endif
            "uniform sampler2D tex;\n"
            "uniform  vec4 color;\n"
            "varying  vec4 qt_TexCoord0;\n"
            "void main(void)\n"
            "{\n"
            "    vec4 pixel = texture2D(tex, qt_TexCoord0.st);\n"
            "    gl_FragColor = pixel * color;\n"
            "}\n";

    pRenderer->glShaderSource(fshader, 1, &fsrc, NULL);
    pRenderer->glCompileShader(fshader);
    pRenderer->glGetShaderiv(fshader, GL_COMPILE_STATUS, &compiled);
    qDebug("MxAlphaTexture fragment shader compiled: %d\n", compiled);


    mProgramId = pRenderer->glCreateProgram();
    pRenderer->glAttachShader(mProgramId, vshader);
    pRenderer->glAttachShader(mProgramId, fshader);
    pRenderer->glLinkProgram(mProgramId);
    GLint value;
    pRenderer->glGetProgramiv(mProgramId, GL_LINK_STATUS, &value);
    qDebug("MxAlphaTexture shader program linked: %d\n", value);
    value = 0;
    pRenderer->glGetProgramiv(mProgramId, GL_INFO_LOG_LENGTH, &value);
    qDebug("MxAlphaTexture program GL_INFO_LOG_LENGTH: %d\n", value);
    /*if(value > 0)
      {
          char str[90];
          GLsizei str_length = 0;
          glGetProgramInfoLog(pProgram,90, &str_length, str);
          //qDebug() << str;
          LOGI("glGetProgramInfoLog: %s", str);
      }*/


    vertexAttr2 = pRenderer->glGetAttribLocation(mProgramId, "vertex");
    texCoordAttr2 = pRenderer->glGetAttribLocation(mProgramId, "texcoord");
    matrixUniform2 = pRenderer->glGetUniformLocation(mProgramId, "matrix");
    pColorUniform = pRenderer->glGetUniformLocation(mProgramId, "color");
    textureUniform2 = pRenderer->glGetUniformLocation(mProgramId, "tex");

}


MxMatrix MxIconProgram::modelViewMatrix() const
{
    return pModelview;
}

void MxIconProgram::setModelViewMatrix( const MxMatrix &matrix )
{
    pModelview = matrix;
    pUpdates |= UpdateMatrix;
}

void MxIconProgram::setColorFilter( const ColorFilter filter )
{
    if( filter != pColorFilter )
    {
        pColorFilter = filter;
        pUpdates |= UpdateAlpha;
    }
}

void MxIconProgram::draw( MxIconDraw &rectsArray )
{
    MxGpuArray *gpuArray = pRenderer->uploadToGpu( vaoFormat(), rectsArray.pArray->data(), rectsArray.pArray->size() );
    enableVao( gpuArray );
    updateUniformValues();
    pRenderer->glUniform1i(textureUniform2, 0); // \TODO flag it to avoid setting always

    pRenderer->glDrawArrays(GL_TRIANGLES, 0, rectsArray.vertexCount());
    disableVao();
}

MxShaderProgram::VaoFormat MxIconProgram::vaoFormat()
{
    return MxShaderProgram::Float_2_2;
}

void MxIconProgram::enableAttributes()
{
    pRenderer->glEnableVertexAttribArray(vertexAttr2);
    pRenderer->glEnableVertexAttribArray(texCoordAttr2);
    pRenderer->glVertexAttribPointer(vertexAttr2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    pRenderer->glVertexAttribPointer(texCoordAttr2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));
}


void MxIconProgram::updateUniformValues()
{
    if( pUpdates & UpdateMatrix )
    {
        pRenderer->glUniformMatrix4fv(matrixUniform2, 1, GL_FALSE, pModelview.constData());
    }

    if( pUpdates & UpdateAlpha )
    {

        if( pColorFilter == IdentityFilter ) {
            GLfloat identityMultiplier[] = {1.0f, 1.0f, 1.0f, 1.0f};
            pRenderer->glUniform4fv(pColorUniform, 1, identityMultiplier);
        } else if( pColorFilter == BlueFilter ) {
            GLfloat blueMultiplier[] = {0.243f, 0.584f, 0.973f, 0.5f};
            pRenderer->glUniform4fv(pColorUniform, 1, blueMultiplier);
        } else if( pColorFilter == LightGrayFilter ) {
            GLfloat lightGrayMultiplier[] = {0.7f, 0.7f, 0.7f, 0.5f};
            pRenderer->glUniform4fv(pColorUniform, 1, lightGrayMultiplier);
        } else {
            Q_ASSERT( pColorFilter == DarkGrayFilter );
            // values taken from https://www.tutorialspoint.com/dip/grayscale_to_rgb_conversion.htm
            GLfloat grayMultiplier[] = { 0.3f, 0.59f, 0.11f, 1.0f };
            pRenderer->glUniform4fv(pColorUniform, 1, grayMultiplier);
        }

    }
}
