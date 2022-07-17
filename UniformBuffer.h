#ifndef UNIFORM_BUFFER_H
#define UNIFORM_BUFFER_H

#include <GL/glew.h>
#include "Util.h"

namespace Renderer
{
	class UniformBuffer
	{
	public:
		// Main constructor
		UniformBuffer(GLuint slot, GLsizeiptr size);
		// Default constructor
		UniformBuffer();
		// Destructor
		~UniformBuffer();

		GLuint id;
		GLsizeiptr size;
	};
}

#endif