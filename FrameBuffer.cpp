#include "FrameBuffer.h"

#include "Log.h"

using namespace Renderer;

FrameBuffer::FrameBuffer(GLsizei width, GLsizei height)
{
	// Generate Framebuffer
	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);

	// Generate Texture
	texture = std::make_shared<Texture>(width, height);

	// Attach texture
	glFramebufferTexture2D
	(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		texture->id,
		0
	);

	// Generate renderbuffer
	renderBuffer = std::make_shared<RenderBuffer>(width, height);

	// Attach renderbuffer
	glFramebufferRenderbuffer
	(
		GL_FRAMEBUFFER,
		GL_DEPTH_STENCIL_ATTACHMENT,
		GL_RENDERBUFFER,
		renderBuffer->id
	);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR("Framebuffer [ID=", id, "] is incomplete!");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FrameBuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &id);
}