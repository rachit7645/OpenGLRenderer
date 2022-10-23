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

		GLuint     slot  = 0;
		GLsizeiptr size  = 0;
		GLuint     id    = 0;
		GLenum     usage = 0;
	};
}

#endif