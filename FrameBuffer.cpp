#include "FrameBuffer.h"

#include "Log.h"

using namespace Renderer;

FrameBuffer::FrameBuffer(GLsizei width, GLsizei height)
{
	// Generate Framebuffer
	glGenFramebuffers(1, &frameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

	// Generate Texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D
	(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		nullptr
	);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Attach texture
	glFramebufferTexture2D
	(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		textureID,
		0
	);

	// Generate renderbuffer
	glGenRenderbuffers(1, &renderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
	glRenderbufferStorage
	(
		GL_RENDERBUFFER,
		GL_DEPTH24_STENCIL8,
		width,
		height
	);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer
	(
		GL_FRAMEBUFFER,
		GL_DEPTH_STENCIL_ATTACHMENT,
		GL_RENDERBUFFER,
		renderBufferID
	);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR("Framebuffer [id=", frameBufferID,"] is incomplete!");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteTextures(1, &textureID);
	glDeleteRenderbuffers(1, &renderBufferID);
	glDeleteFramebuffers(1, &frameBufferID);
}