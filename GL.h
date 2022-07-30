#ifndef RACHIT_GL_H
#define RACHIT_GL_H

#include <GL/glew.h>

// Quick wrappers around common GL calls
namespace GL
{
	GLint GetIntegerv(GLenum param);
}

#endif