#include "RenderBuffer.h"

using namespace Renderer;

RenderBuffer::RenderBuffer(GLsizei width, GLsizei height, GLenum internalformat)
	: format(internalformat)
{
	// Create render buffer
	glCreateRenderbuffers(1, &id);
	// Set storage format
	SetStorage(width, height);
}

void RenderBuffer::SetStorage(GLsizei width, GLsizei height)
{
	// Set storage format
	glNamedRenderbufferStorage
	(
		id,
		format,
		width,
		height
	);
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
