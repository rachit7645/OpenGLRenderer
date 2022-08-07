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
		RenderBuffer(GLsizei width, GLsizei height);
		// Destructor
		~RenderBuffer();

		GLuint id;
	};
}

#endif