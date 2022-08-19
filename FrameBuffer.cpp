#include "FrameBuffer.h"

#include "Log.h"
#include "Window.h"

using namespace Renderer;

using Window::DIMENSIONS;

FrameBuffer::FrameBuffer(GLsizei width, GLsizei height, FBType type)
	: width(width),
	  height(height),
	  type(type)
{
	// Generate Framebuffer
	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);

	switch (type)
	{
	case FBType::Color:
		CreateColorTexture();
		CreateDepthBuffer();
		break;

	case FBType::Depth:
		CreateDepthTexture();
		// FIXME: RenderBuffer not supported for color attachments
		break;

	case FBType::ColorAndDepth:
		CreateColorTexture();
		CreateDepthTexture();
		break;

	case FBType::None:
		LOG_ERROR("FBType is equal to \"None\"");
	}

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR("Framebuffer [ID=", id, "] is incomplete!");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::CreateColorTexture()
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
		colorTexture->id,
		0
	);
}

void FrameBuffer::CreateDepthTexture()
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
		depthTexture->id,
		0
	);
}

void FrameBuffer::CreateDepthBuffer()
{
	renderBuffer = std::make_shared<RenderBuffer>(width, height);
	glFramebufferRenderbuffer
	(
		GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER,
		renderBuffer->id
	);
}

void FrameBuffer::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glViewport(0, 0, width, height);
}

void FrameBuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, DIMENSIONS.x, DIMENSIONS.y);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &id);
}