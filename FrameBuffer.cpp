#include "FrameBuffer.h"

#include "Log.h"
#include "Window.h"

using namespace Renderer;

using Window::WINDOW_DIMENSIONS;

void FrameBuffer::CreateFrameBuffer()
{
	glGenFramebuffers(1, &id);
}

void FrameBuffer::AddTexture(TxPtr& texture, const FBOAttachment& attachment)
{
	texture = std::make_shared<Texture>();
	texture->width  = width;
	texture->height = height;
	texture->type   = GL_TEXTURE_2D;

	texture->CreateTexture();
	texture->Bind();
	texture->SetParameter(GL_TEXTURE_MIN_FILTER, attachment.minFilter);
	texture->SetParameter(GL_TEXTURE_MAG_FILTER, attachment.maxFilter);
	texture->SetParameter(GL_TEXTURE_WRAP_S,     attachment.wrapMode);
	texture->SetParameter(GL_TEXTURE_WRAP_T,     attachment.wrapMode);

	texture->LoadImageData
	(
		nullptr,
		attachment.internalFormat,
		attachment.format,
		attachment.dataType
	);

	texture->Unbind();

	glFramebufferTexture2D
	(
		GL_FRAMEBUFFER,
		attachment.slot,
		texture->type,
		texture->id,
		0
	);
}

void FrameBuffer::AddTextureCubeMap(TxPtr& texture, const FBOAttachment& attachment)
{
	texture = std::make_shared<Texture>();
	texture->width  = width;
	texture->height = height;
	texture->type   = GL_TEXTURE_CUBE_MAP;

	texture->CreateTexture();
	texture->Bind();
	texture->SetParameter(GL_TEXTURE_MIN_FILTER, attachment.minFilter);
	texture->SetParameter(GL_TEXTURE_MAG_FILTER, attachment.maxFilter);
	texture->SetParameter(GL_TEXTURE_WRAP_S,     attachment.wrapMode);
	texture->SetParameter(GL_TEXTURE_WRAP_T,     attachment.wrapMode);
	texture->SetParameter(GL_TEXTURE_WRAP_R,     attachment.wrapMode);

	for (usize i = 0; i < 6; ++i)
	{
		texture->LoadImageData
		(
			nullptr,
			attachment.internalFormat,
			attachment.format,
			attachment.dataType,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
		);
	}

	texture->Unbind();
}

void FrameBuffer::AddTextureArray(TxPtr& texture, const FBOAttachment& attachment)
{
	texture = std::make_shared<Texture>();
	texture->width  = width;
	texture->height = height;
	texture->depth  = depth;
	texture->type   = GL_TEXTURE_2D_ARRAY;

	texture->CreateTexture();
	texture->Bind();
	texture->SetParameter(GL_TEXTURE_MIN_FILTER, attachment.minFilter);
	texture->SetParameter(GL_TEXTURE_MAG_FILTER, attachment.maxFilter);
	texture->SetParameter(GL_TEXTURE_WRAP_S,     attachment.wrapMode);
	texture->SetParameter(GL_TEXTURE_WRAP_T,     attachment.wrapMode);

	if (attachment.border != nullptr)
	{
		texture->SetParameter(GL_TEXTURE_BORDER_COLOR, attachment.border);
	}

	texture->LoadImageData3D
	(
		nullptr,
		attachment.internalFormat,
		attachment.format,
		attachment.dataType
	);

	texture->Unbind();

	glFramebufferTexture
	(
		GL_FRAMEBUFFER,
		attachment.slot,
		depthTexture->id,
		0
	);
}

void FrameBuffer::AddBuffer(RdBufPtr& buffer, const FBOAttachment& attachment)
{
	buffer = std::make_shared<RenderBuffer>
	(
		width,
		height,
		attachment.internalFormat
	);

	glFramebufferRenderbuffer
	(
		GL_FRAMEBUFFER,
		attachment.slot,
		GL_RENDERBUFFER,
		buffer->id
	);
}

void FrameBuffer::SetDrawBuffer(GLenum value)
{
	glDrawBuffer(value);
}

void FrameBuffer::SetReadBuffer(GLenum value)
{
	glReadBuffer(value);
}

void FrameBuffer::SetDrawBuffers(const std::vector<GLenum>& buffers)
{
	glDrawBuffers(static_cast<GLsizei>(buffers.size()), buffers.data());
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
	glViewport(0, 0, WINDOW_DIMENSIONS.x, WINDOW_DIMENSIONS.y);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &id);
}