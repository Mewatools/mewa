/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxvectorprogram.h"
#include "mxrenderer.h"
#include "mxbuffer.h"


MxVectorProgram::MxVectorProgram()
{
}

MxVectorProgram::~MxVectorProgram()
{
    pRenderer->glDeleteProgram( mProgramId );
}


void MxVectorProgram::init( MxRenderer *renderer )
{
    Q_ASSERT( NULL == pRenderer );
    pRenderer = renderer;

    compile();
}

void MxVectorProgram::compile()
{

    GLuint vshader = pRenderer->glCreateShader(GL_VERTEX_SHADER);
    const char *vsrc1 =
            "#version 300 es\n"
            "layout(location = 0) in  vec2 vertex;\n"
            "layout(location = 1) in  vec2 aUVT;\n"
            "layout(location = 2) in  vec4 color;\n"
            "uniform  mat4 matrix;\n"
            "out  vec2 fsUVT;\n"
            "out  vec4 vColor;\n"
            "void main(void)\n"
            "{\n"
            "   fsUVT = aUVT;\n"
            "   vColor = color;\n"
            "   gl_Position = matrix * vec4(vertex.x, vertex.y, 0.0, 1.0);\n"
            "}\n";


    pRenderer->glShaderSource(vshader, 1, &vsrc1, NULL);
    pRenderer->glCompileShader(vshader);
    GLint compiled;
    pRenderer->glGetShaderiv(vshader, GL_COMPILE_STATUS, &compiled);
    qDebug("MxVectorProgram vertex shader compiled: %d", compiled);


    GLuint fshader = pRenderer->glCreateShader(GL_FRAGMENT_SHADER);
    const char *fsrc_with_dev =
            "#version 300 es\n"
            "in  vec2 fsUVT;\n"
            "in  vec4 vColor;\n"
            "out highp vec4 fragColor;\n"
            "void main()\n"
            "{\n"
            "  float inside = sign(vColor.a - 0.5);\n" // + to draw outside, - to draw inside
            //"  float alphaValue = 1.0;\n"
            "  vec2 dx = dFdx(fsUVT);\n"
            "  vec2 dy = dFdy(fsUVT);\n"
            "  float fx = (2.0*fsUVT.x)*dx.x - dx.y;\n"
            "  float fy = (2.0*fsUVT.x)*dy.x - dy.y;\n"
            "  float sdf = (fsUVT.x*fsUVT.x - fsUVT.y)/sqrt(fx*fx + fy*fy);\n"
            "  float alpha = smoothstep(inside, -inside, sdf);"
            "  fragColor = vColor;\n"
            //"  gl_FragColor.a = min(alphaValue, alpha);"
            "  fragColor.a = alpha;"
            "}\n";

    pRenderer->glShaderSource(fshader, 1, &fsrc_with_dev, NULL);
    pRenderer->glCompileShader(fshader);
    pRenderer->glGetShaderiv(fshader, GL_COMPILE_STATUS, &compiled);
     pRenderer->checkGLError(__FILE__, __LINE__);
    if (compiled != GL_TRUE)
    {
        // \TODO use https://stackoverflow.com/questions/22442304/glsl-es-dfdx-dfdy-analog

        qDebug("extension GL_OES_standard_derivatives not supported, falling to aliased shader...\n");
        const char *fsrc =
                "precision highp float;\n"
                "varying  vec2 fsUVT;\n"
                "varying  vec4 vColor;\n"
                "void main()\n"
                "{\n"
                "  float alpha = fsUVT.x * fsUVT.x - fsUVT.y;\n"
                "  alpha = step(0.01, alpha);\n"
                "  gl_FragColor = vec4(vColor.rgb, 1.0-alpha);\n"
                "}\n";
        pRenderer->glShaderSource(fshader, 1, &fsrc, NULL);
        pRenderer->glCompileShader(fshader);
        pRenderer->glGetShaderiv(fshader, GL_COMPILE_STATUS, &compiled);
         pRenderer->checkGLError(__FILE__, __LINE__);
    }


    if (compiled != GL_TRUE)
    {
        GLint infoLogLength;
        pRenderer->glGetShaderiv(fshader, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* errMsg = new char[infoLogLength + 1];
        pRenderer->glGetShaderInfoLog(fshader, infoLogLength, NULL, errMsg);
        qDebug("MxVectorProgram shader compilation error: %s", errMsg);
    }
    else
    {
        qDebug("MxVectorProgram shader compilation completed sucessfully");
    }


    mProgramId = pRenderer->glCreateProgram();
    pRenderer->glAttachShader(mProgramId, vshader);
    pRenderer->glAttachShader(mProgramId, fshader);
    pRenderer->glLinkProgram(mProgramId);
    GLint value = 0;
    pRenderer->glGetProgramiv(mProgramId, GL_LINK_STATUS, &value);
    qDebug("MxVectorProgram shader program linked: %d", value);

    matrixUniform1 = pRenderer->glGetUniformLocation(mProgramId, "matrix");
    mVertexAttrib = pRenderer->glGetAttribLocation(mProgramId, "vertex");
    mUvAttrib = pRenderer->glGetAttribLocation(mProgramId, "aUVT");
    mColorAttrib = pRenderer->glGetAttribLocation(mProgramId, "color");

    pRenderer->checkGLError(__FILE__, __LINE__);

    // \TODO reuse frag shader

    // delete shaders as they are not needed anymore
    pRenderer->glDetachShader(mProgramId, vshader);
    pRenderer->glDetachShader(mProgramId, fshader);
    // glDeleteShader( vshader );
    // glDeleteShader( fshader );

}

void MxVectorProgram::draw( MxVectorDraw &stream, const MxMatrix *matrix )
{
     pRenderer->setBlending( MxRenderer::BlendingImages );


    MxGpuArray *gpuArray = pRenderer->uploadToGpu( vaoFormat(), stream.pArray->data(), stream.pArray->size() );

    enableVao( gpuArray );

    if(matrix) {
       pRenderer->glUniformMatrix4fv(matrixUniform1, 1, GL_FALSE, matrix->constData());
    }

    pRenderer->glDrawArrays( GL_TRIANGLES , 0, stream.pointCount() );
    disableVao();
}


MxGpuProgram::VaoFormat MxVectorProgram::vaoFormat()
{
    return MxGpuProgram::Float2_UChar4_Float2;
}

void MxVectorProgram::enableAttributes()
{
    pRenderer->glEnableVertexAttribArray(mVertexAttrib);
    pRenderer->glEnableVertexAttribArray(mUvAttrib);
    pRenderer->glEnableVertexAttribArray(mColorAttrib);

    // \TODO in OpenGL ES2 this does not need to be called always
    uintptr_t offset = 0;
    pRenderer->glVertexAttribPointer(mVertexAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(MxVectorDraw::Vertex), (void *)offset);
    offset += (2*sizeof(GLfloat));
    pRenderer->glVertexAttribPointer(mColorAttrib, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(MxVectorDraw::Vertex), (void *)offset);
    offset += (4*sizeof(GLubyte));
    pRenderer->glVertexAttribPointer(mUvAttrib , 2, GL_FLOAT, GL_FALSE, sizeof(MxVectorDraw::Vertex), (void *)offset);
}

void MxVectorProgram::disableAttributes()
{
    pRenderer->glBindBuffer(GL_ARRAY_BUFFER, 0); // \TODO needed ??
    pRenderer->glDisableVertexAttribArray(mVertexAttrib);
    pRenderer->glDisableVertexAttribArray(mUvAttrib);
    pRenderer->glDisableVertexAttribArray(mColorAttrib);
}

