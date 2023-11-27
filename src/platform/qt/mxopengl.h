/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXOPENGL_H
#define MXOPENGL_H

// taken from https://www.khronos.org/registry/OpenGL/api/GLSC/1.0/gl.h

typedef unsigned int GLenum;
typedef int GLint;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef signed long int GLsizeiptr;
typedef signed long int GLintptr;



#define 	GL_LINE_LOOP   0x0002
#define 	GL_LINE_STRIP   0x0003
#define 	GL_TRIANGLE_FAN   0x0006


// taken from https://www.khronos.org/registry/OpenGL/api/GL/glext.h

#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_ARRAY_BUFFER                   0x8892

#endif
