#ifndef UNIFORM_BUFFER_H
#define UNIFORM_BUFFER_H

#include <GL/glew.h>
#include "Util.h"

namespace Renderer
{
	class UniformBuffer
	{
	public:
		UniformBuffer(GLuint slot, GLsizeiptr size);
		~UniformBuffer();

		GLuint id;
		GLsizeiptr size;
	};
}

#endif