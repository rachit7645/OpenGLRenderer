#ifndef UNIFORM_BUFFER_H
#define UNIFORM_BUFFER_H

#include <GL/glew.h>

namespace Renderer
{
	// Make macro for glBufferSubData
	class UniformBuffer
	{
	public:
		// Default constructor
		UniformBuffer() = default;
		// Main constructor
		UniformBuffer(GLuint slot, GLsizeiptr size);
		// Destructor
		~UniformBuffer();

		GLuint     slot = 0;
		GLuint     id   = 0;
		GLsizeiptr size = 0;
	};
}

#endif