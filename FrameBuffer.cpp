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
		SetColorBuffer();
		CreateDepthTexture();
		break;

	case FBType::ColorAndDepth:
		CreateColorTexture();
		CreateDepthTexture();
		break;

	case FBType::Empty:
		CreateColorBuffer();
		CreateDepthBuffer();
		break;

	case FBType::None:
	case FBType::DepthArray:
		LOG_ERROR("{}\n", "FBType is invalid");
	}

	CheckStatus();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::FrameBuffer(GLsizei width, GLsizei height, GLsizei depth)
	: width(width),
	  height(height),
	  depth(depth),
	  type(FBType::DepthArray)
{
	// Generate Framebuffer
	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);

	SetColorBuffer();
	CreateDepthArrayTexture();

	CheckStatus();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);

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

void FrameBuffer::CreateColorBuffer()
{
	colorRenderBuffer = std::make_shared<RenderBuffer>
	(
		width,
		height,
		GL_RGBA
	);

	glFramebufferRenderbuffer
	(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_RENDERBUFFER,
		colorRenderBuffer->id
	);
}

void FrameBuffer::CreateDepthBuffer()
{
	depthRenderBuffer = std::make_shared<RenderBuffer>
	(
		width,
		height,
		GL_DEPTH_COMPONENT24
	);

	glFramebufferRenderbuffer
	(
		GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER,
		depthRenderBuffer->id
	);
}

void FrameBuffer::CreateDepthArrayTexture()
{
	depthTexture = std::make_shared<Texture>
	(
		width,
		height,
		depth,
		GL_DEPTH_COMPONENT24,
		GL_DEPTH_COMPONENT,
		GL_FLOAT
	);

	glFramebufferTexture
	(
		GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		depthTexture->id,
		0
	);
}

void FrameBuffer::SetColorBuffer()
{
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}

void FrameBuffer::CheckStatus()
{
	static const std::unordered_map<GLenum, const char*> GL_FRAMEBUFFER_ERROR_TYPES
	{
		{GL_FRAMEBUFFER_UNDEFINED,                     "[FRAMEBUFFER_UNDEFINED]"                    },
		{GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,         "[FRAMEBUFFER_INCOMPLETE_ATTACHMENT]"        },
		{GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT, "[FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT]"},
		{GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,        "[FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER]"       },
		{GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER,        "[FRAMEBUFFER_INCOMPLETE_READ_BUFFER]"       },
		{GL_FRAMEBUFFER_UNSUPPORTED,                   "[FRAMEBUFFER_UNSUPPORTED]"                  },
		{GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE,        "[FRAMEBUFFER_INCOMPLETE_MULTISAMPLE]"       },
		{GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS,      "[FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS]"     }
	};

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		auto errorStr = GL_FRAMEBUFFER_ERROR_TYPES.find(status)->second;
		LOG_ERROR("{} Framebuffer [ID={}] is incomplete!\n", errorStr, id);
	}
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