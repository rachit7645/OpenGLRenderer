#include "RenderBuffer.h"

using namespace Renderer;

RenderBuffer::RenderBuffer(GLsizei width, GLsizei height, GLenum internalformat)
{
	// Create render buffer
	glGenRenderbuffers(1, &id);
	// Bind render buffer
	glBindRenderbuffer(GL_RENDERBUFFER, id);
	// Set storage format
	glRenderbufferStorage
	(
		GL_RENDERBUFFER,
		internalformat,
		width,
		height
	);
	// Unbind render buffer
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

RenderBuffer::~RenderBuffer()
{
	// ID must not be zero
	if (id != 0)
	{
		// Clear render buffer
		glDeleteRenderbuffers(1, &id);
	}
}
