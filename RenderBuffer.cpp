#include "RenderBuffer.h"

using namespace Renderer;

RenderBuffer::RenderBuffer(GLsizei width, GLsizei height)
{
	glGenRenderbuffers(1, &id);
	glBindRenderbuffer(GL_RENDERBUFFER, id);
	glRenderbufferStorage
	(
		GL_RENDERBUFFER,
		GL_DEPTH24_STENCIL8,
		width,
		height
	);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

RenderBuffer::~RenderBuffer()
{
	glDeleteRenderbuffers(1, &id);
}
