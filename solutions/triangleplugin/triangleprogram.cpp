/****************************************************************************
** Copyright (C) 2020-2024 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "triangleprogram.h"


TriangleProgram::TriangleProgram()
{
    mPositionAttrib = 0;
}


TriangleProgram::~TriangleProgram()
{}

bool TriangleProgram::initialize( MxRenderer *renderer )
{
    Q_ASSERT( NULL == pRenderer );
    pRenderer = renderer;


    const char * gVertexShader =
            "#version 120\n"
            "attribute vec2 vPosition;\n"
            "void main() {\n"
            "  gl_Position = vec4(vPosition.x, vPosition.y, 0.0, 1.0);\n"
            "}\n";

    const char * gFragmentShader =
            "#version 120\n"
        #ifdef MX_OPENGL_ES
            "precision mediump float;\n"
        #endif
            "void main() {\n"
            "  gl_FragColor = vec4(1.0, 0.5, 0.0, 1.0);\n"
            "}\n";

    mProgramId = createProgram(*renderer, gVertexShader, gFragmentShader);
    if (!mProgramId) {
        qDebug("Could not create program.");
        return false;
    }
    mPositionAttrib = renderer->glGetAttribLocation(mProgramId, "vPosition");
    renderer->checkGLError(__FILE__, __LINE__);
    return true;
}

MxGpuProgram::VaoFormat TriangleProgram::vaoFormat()
{
    return Float_2;
}

void TriangleProgram::enableAttributes()
{
    pRenderer->glVertexAttribPointer(mPositionAttrib, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), NULL);
    pRenderer->glEnableVertexAttribArray(mPositionAttrib);
}

GLuint TriangleProgram::loadShader( MxRenderer &renderer, GLenum shaderType, const char* pSource)
{
    GLuint shader = renderer.glCreateShader(shaderType);
    if (shader) {
        renderer.glShaderSource(shader, 1, &pSource, NULL);
        renderer.glCompileShader(shader);
        GLint compiled = 0;
        renderer.glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            renderer.glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    renderer.glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    qDebug("Could not compile shader %d:\n%s\n", shaderType, buf);
                    free(buf);
                }
                renderer.glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}


GLuint TriangleProgram::createProgram( MxRenderer &renderer, const char* pVertexSource, const char* pFragmentSource)
{
    GLuint vertexShader = loadShader(renderer, GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        return 0;
    }
    renderer.checkGLError(__FILE__, __LINE__);
    GLuint pixelShader = loadShader(renderer, GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        return 0;
    }
    renderer.checkGLError(__FILE__, __LINE__);
    GLuint program = renderer.glCreateProgram();
    if (program) {
        renderer.glAttachShader(program, vertexShader);
        renderer.glAttachShader(program, pixelShader);

        //renderer.glBindAttribLocation(program, 0, "vPosition");

        renderer.glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        renderer.glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            renderer.glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    renderer.glGetProgramInfoLog(program, bufLength, NULL, buf);
                    qDebug("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            renderer.glDeleteProgram(program);
            program = 0;
        }
    }
    renderer.checkGLError(__FILE__, __LINE__);

    // delete shaders as they are not needed anymore
    pRenderer->glDetachShader(program, vertexShader);
    pRenderer->glDetachShader(program, pixelShader);
    renderer.glDeleteShader( vertexShader );
    renderer.glDeleteShader( pixelShader );

    return program;
}




