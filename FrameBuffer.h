#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>

namespace Renderer
{
	class FrameBuffer
	{
	public:
		// Default constructor
		FrameBuffer() = default;
		// Main constructor
		FrameBuffer(GLsizei width, GLsizei height);
		// Destructor
		~FrameBuffer();

		// Framebuffer ID
		GLuint frameBufferID = 0;
		// Texture ID
		GLuint textureID = 0;
		// Renderbuffer ID
		GLuint renderBufferID = 0;
	};
}

#endif