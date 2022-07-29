#include "GL.h"

GLint GL::GetIntegerv(GLenum param)
{
	GLint value;
	glGetIntegerv(param, &value);
	return value;
}