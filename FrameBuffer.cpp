#include "FrameBuffer.h"

#include "Log.h"
#include "Window.h"
#include "GLM.h"

using namespace Renderer;

using Window::DIMENSIONS;

constexpr glm::vec4 borderColor = {1.0f, 1.0f, 1.0f, 1.0f};

void FrameBuffer::CreateFrameBuffer()
{
	glGenFramebuffers(1, &id);
}

void FrameBuffer::CreateColorTexture()
{
	colorTexture = std::make_shared<Texture>();
	colorTexture->width  = width;
	colorTexture->height = height;

	colorTexture->CreateTexture();
	colorTexture->Bind();
	colorTexture->SetParameter(GL_TEXTURE_MIN_FILTER, filter);
	colorTexture->SetParameter(GL_TEXTURE_MAG_FILTER, filter);
	colorTexture->SetParameter(GL_TEXTURE_WRAP_S,     GL_REPEAT);
	colorTexture->SetParameter(GL_TEXTURE_WRAP_T,     GL_REPEAT);
	colorTexture->LoadImageData(nullptr, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	colorTexture->Unbind();

	glFramebufferTexture2D
	(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		colorTexture->id,
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

void FrameBuffer::SetColorBuffer(GLenum value)
{
	glDrawBuffer(value);
	glReadBuffer(value);
}

void FrameBuffer::CreateDepthTexture()
{
	depthTexture = std::make_shared<Texture>();
	depthTexture->width  = width;
	depthTexture->height = height;

	depthTexture->CreateTexture();
	depthTexture->Bind();
	depthTexture->SetParameter(GL_TEXTURE_MIN_FILTER, filter);
	depthTexture->SetParameter(GL_TEXTURE_MAG_FILTER, filter);
	depthTexture->SetParameter(GL_TEXTURE_WRAP_S,     GL_REPEAT);
	depthTexture->SetParameter(GL_TEXTURE_WRAP_T,     GL_REPEAT);
	depthTexture->LoadImageData
	(
		nullptr,
		GL_DEPTH_COMPONENT24,
		GL_DEPTH_COMPONENT,
		GL_FLOAT
	);
	depthTexture->Unbind();

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
	depthTexture = std::make_shared<Texture>();
	depthTexture->width  = width;
	depthTexture->height = height;
	depthTexture->depth  = depth;
	depthTexture->type   = GL_TEXTURE_2D_ARRAY;

	depthTexture->CreateTexture();
	depthTexture->Bind();
	depthTexture->SetParameter(GL_TEXTURE_MIN_FILTER,   filter);
	depthTexture->SetParameter(GL_TEXTURE_MAG_FILTER,   filter);
	depthTexture->SetParameter(GL_TEXTURE_WRAP_S,       GL_CLAMP_TO_BORDER);
	depthTexture->SetParameter(GL_TEXTURE_WRAP_T,       GL_CLAMP_TO_BORDER);
	depthTexture->SetParameter(GL_TEXTURE_BORDER_COLOR, &borderColor[0]);
	depthTexture->LoadImageData3D
	(
		nullptr,
		GL_DEPTH_COMPONENT24,
		GL_DEPTH_COMPONENT,
		GL_FLOAT
	);
	depthTexture->Unbind();

	glFramebufferTexture
	(
		GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		depthTexture->id,
		0
	);
}

void FrameBuffer::EnableDepth()
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
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