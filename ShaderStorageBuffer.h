#ifndef SHADER_STORAGE_BUFFER_H
#define SHADER_STORAGE_BUFFER_H

#include <GL/glew.h>

namespace Renderer
{
	class ShaderStorageBuffer
	{
	public:
		// Default constructor
		ShaderStorageBuffer() = default;
		// Main constructor
		ShaderStorageBuffer(GLuint slot, GLsizeiptr size, GLenum usage);
		// Destructor
		~ShaderStorageBuffer();

		GLuint     slot = 0;
		GLuint     id   = 0;
		GLsizeiptr size = 0;
	};
}

#endif
