/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxiconprogram.h"
#include "mxrect.h"
#include "mxicondraw.h"
#include "mxrenderer.h"
#include "mxbuffer.h"


/*! Creates a MxIconProgram
*/
MxIconProgram::MxIconProgram()
{
}


MxIconProgram::~MxIconProgram()
{
}

void MxIconProgram::init( MxRenderer *renderer )
{
    Q_ASSERT( NULL == pRenderer );
    pRenderer = renderer;

    compile();
}

void MxIconProgram::compile()
{
    Q_ASSERT( NULL != pRenderer );

    // TODO: change highp with mediump
    GLuint vshader = pRenderer->glCreateShader(GL_VERTEX_SHADER);

    const char *vsrc =
        "#version 120\n"
            "attribute vec4 vertex;\n"
            "attribute vec4 texcoord;\n"
            "attribute vec4 color;\n"
            "uniform mat4 matrix;\n"
            "varying vec4 vTexCoord;\n"
            "varying vec4 vColor;\n"
            "void main(void)\n"
            "{\n"
            "    gl_Position = matrix * vertex;\n"
            "    vColor = color;\n"
            "    vTexCoord = texcoord;\n"
            "}\n";
    pRenderer->glShaderSource(vshader, 1, &vsrc, NULL);
    pRenderer->glCompileShader(vshader);
    GLint compiled;
    pRenderer->glGetShaderiv(vshader, GL_COMPILE_STATUS, &compiled);

    qDebug("MxIconProgram vertex shader compiled: %d\n", compiled);


    GLuint fshader = pRenderer->glCreateShader(GL_FRAGMENT_SHADER);
    const char *fsrc =
             "#version 120\n"
        #ifdef MX_OPENGL_ES
            "precision mediump float;\n"
        #endif
            "uniform sampler2D tex;\n"
            "varying vec4 vTexCoord;\n"
            "varying vec4 vColor;\n"
            "void main(void)\n"
            "{\n"
            "    vec4 pixel = texture2D(tex, vTexCoord.st);\n"
            "    gl_FragColor = pixel * vColor;\n"
            "}\n";

    pRenderer->glShaderSource(fshader, 1, &fsrc, NULL);
    pRenderer->glCompileShader(fshader);
    pRenderer->glGetShaderiv(fshader, GL_COMPILE_STATUS, &compiled);
    qDebug("MxIconProgram fragment shader compiled: %d\n", compiled);
    if( ! compiled )
      {
          char str[90];
          GLsizei str_length = 0;
          pRenderer->glGetShaderInfoLog(fshader,90, &str_length, str);
          //qDebug() << str;
          qDebug("glGetProgramInfoLog: %s", str);
      }


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
          pRenderer->glGetProgramInfoLog(mProgramId,90, &str_length, str);
          //qDebug() << str;
          qDebug("glGetProgramInfoLog: %s", str);
      }*/


    vertexAttr2 = pRenderer->glGetAttribLocation(mProgramId, "vertex");
    texCoordAttr2 = pRenderer->glGetAttribLocation(mProgramId, "texcoord");
    matrixUniform2 = pRenderer->glGetUniformLocation(mProgramId, "matrix");
    mColorAttrib = pRenderer->glGetAttribLocation(mProgramId, "color");
    textureUniform2 = pRenderer->glGetUniformLocation(mProgramId, "tex");

}

void MxIconProgram::draw( MxIconDraw &rectsArray, const MxMatrix *matrix )
{
     pRenderer->setBlending( MxRenderer::BlendingImages );

    MxGpuArray *gpuArray = pRenderer->uploadToGpu( vaoFormat(), rectsArray.pArray->data(), rectsArray.pArray->size() );
    enableVao( gpuArray );

    if(matrix) {
       pRenderer->glUniformMatrix4fv(matrixUniform2, 1, GL_FALSE, matrix->constData());
    }

    pRenderer->glUniform1i(textureUniform2, 0); // \TODO flag it to avoid setting always

    pRenderer->glDrawArrays(GL_TRIANGLES, 0, rectsArray.vertexCount());
}


MxGpuProgram::VaoFormat MxIconProgram::vaoFormat()
{
    return MxGpuProgram::Float2_UChar4_Float2;
}

void MxIconProgram::enableAttributes()
{
    pRenderer->glEnableVertexAttribArray(vertexAttr2);
    pRenderer->glEnableVertexAttribArray(texCoordAttr2);
    pRenderer->glEnableVertexAttribArray(mColorAttrib);

    uintptr_t offset = 0;
    pRenderer->glVertexAttribPointer(vertexAttr2, 2, GL_FLOAT, GL_FALSE, sizeof(MxIconDraw::Vertex), (void *)offset);
    offset += (2*sizeof(GLfloat));
    pRenderer->glVertexAttribPointer(mColorAttrib, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(MxIconDraw::Vertex), (void *)offset);
    offset += (4*sizeof(GLubyte));
    pRenderer->glVertexAttribPointer(texCoordAttr2, 2, GL_FLOAT, GL_FALSE,  sizeof(MxIconDraw::Vertex), (void *)offset);
}
