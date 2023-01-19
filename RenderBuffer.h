#ifndef RENDER_BUFFER_H
#define RENDER_BUFFER_H

#include <GL/glew.h>

namespace Renderer
{
	class RenderBuffer
	{
	public:
		// Default constructor
		RenderBuffer() = default;
		// Main constructor
		RenderBuffer(GLsizei width, GLsizei height, GLenum internalformat);
		// Destructor
		~RenderBuffer();
		// Sets storage to new dimensions
		void SetStorage(GLsizei width, GLsizei height);
		// OpenGL ID
		GLuint id = 0;
		// Format
		GLenum format = 0;
	};
}

#endif