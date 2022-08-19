#include "FrameBuffer.h"

#include "Log.h"

using namespace Renderer;

FrameBuffer::FrameBuffer(GLsizei width, GLsizei height, FBType type)
{
	// Generate Framebuffer
	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);

	switch (type)
	{
	case FBType::Color:
		CreateColorTexture(width, height);
		CreateDepthBuffer(width, height);
		break;

	case FBType::Depth:
		CreateDepthTexture(width, height);
		// FIXME: RenderBuffer not supported for color attachments
		break;

	case FBType::ColorAndDepth:
		CreateColorTexture(width, height);
		CreateDepthTexture(width, height);
		break;
	}

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR("Framebuffer [ID=", id, "] is incomplete!");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::CreateColorTexture(GLsizei width, GLsizei height)
{
	colorTexture = std::make_shared<Texture>
	(
		width,
		height,
		GL_RGBA,
		GL_RGBA,
		GL_UNSIGNED_BYTE
	);

	glFramebufferTexture2D
	(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		colorTexture->get()->id,
		0
	);
}

void FrameBuffer::CreateDepthTexture(GLsizei width, GLsizei height)
{
	depthTexture = std::make_shared<Texture>
	(
		width,
		height,
		GL_DEPTH_COMPONENT24,
		GL_DEPTH_COMPONENT,
		GL_FLOAT
	);

	glFramebufferTexture2D
	(
		GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D,
		depthTexture->get()->id,
		0
	);
}

void FrameBuffer::CreateDepthBuffer(GLsizei width, GLsizei height)
{
	renderBuffer = std::make_shared<RenderBuffer>(width, height);
	glFramebufferRenderbuffer
	(
		GL_FRAMEBUFFER,
		GL_DEPTH_STENCIL_ATTACHMENT,
		GL_RENDERBUFFER,
		renderBuffer->get()->id
	);
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