#ifndef UNIFORM_BUFFER_H
#define UNIFORM_BUFFER_H

#include <GL/glew.h>

namespace Renderer
{
	class UniformBuffer
	{
	public:
		// Default constructor
		UniformBuffer() = default;
		// Main constructor
		UniformBuffer(GLuint slot, GLsizeiptr size, GLenum usage);
		// Destructor
		~UniformBuffer();
		// Buffer ID
		GLuint id = 0;
		// Buffer slot
		GLuint slot = 0;
		// Buffer max size
		GLsizeiptr size = 0;
		// Buffer usage hint
		GLenum usage = 0;
	};
}

#endif