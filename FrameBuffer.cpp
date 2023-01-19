#include "FrameBuffer.h"

#include "Log.h"
#include "Window.h"
#include "Settings.h"

// Using namespaces
using namespace Renderer;

// Usings
using Engine::Settings;

void FrameBuffer::CreateFrameBuffer()
{
	// Generate FBO ID
	glGenFramebuffers(1, &id);
}

void FrameBuffer::CreateTexture(TxPtr& texture, const FBOAttachment& attachment)
{
	// Set up texture
	texture         = std::make_shared<Texture>();
	texture->width  = width;
	texture->height = height;
	texture->type   = GL_TEXTURE_2D;

	// Create texture
	texture->CreateTexture();
	texture->SetParameter(GL_TEXTURE_MIN_FILTER, attachment.minFilter);
	texture->SetParameter(GL_TEXTURE_MAG_FILTER, attachment.maxFilter);
	texture->SetParameter(GL_TEXTURE_WRAP_S,     attachment.wrapMode);
	texture->SetParameter(GL_TEXTURE_WRAP_T,     attachment.wrapMode);

	// If attachment has (or will have) mipmaps
	if (CheckMipMap(attachment.minFilter))
	{
		// Allocate memory for base texture and mipmaps
		texture->Storage2DMipMap(attachment.intFormat);
	}
	else
	{
		// Allocate memory for base texture
		texture->Storage2D(attachment.intFormat);
	}
}

void FrameBuffer::CreateTextureCubeMap(TxPtr& texture, const FBOAttachment& attachment)
{
	// Set up cube map
	texture         = std::make_shared<Texture>();
	texture->width  = width;
	texture->height = height;
	texture->type   = GL_TEXTURE_CUBE_MAP;

	// Create cube map
	texture->CreateTexture();
	texture->SetParameter(GL_TEXTURE_MIN_FILTER, attachment.minFilter);
	texture->SetParameter(GL_TEXTURE_MAG_FILTER, attachment.maxFilter);
	texture->SetParameter(GL_TEXTURE_WRAP_S,     attachment.wrapMode);
	texture->SetParameter(GL_TEXTURE_WRAP_T,     attachment.wrapMode);
	texture->SetParameter(GL_TEXTURE_WRAP_R,     attachment.wrapMode);

	// If attachment has (or will have) mipmaps
	if (CheckMipMap(attachment.minFilter))
	{
		// Allocate memory for base texture and mipmaps
		texture->Storage2DMipMap(attachment.intFormat);
	}
	else
	{
		// Allocate memory for base texture
		texture->Storage2D(attachment.intFormat);
	}
}

void FrameBuffer::CreateTextureArray(TxPtr& texture, const FBOAttachment& attachment)
{
	// Set up texture array
	texture         = std::make_shared<Texture>();
	texture->width  = width;
	texture->height = height;
	texture->depth  = depth;
	texture->type   = GL_TEXTURE_2D_ARRAY;

	// Create texture array
	texture->CreateTexture();
	texture->SetParameter(GL_TEXTURE_MIN_FILTER, attachment.minFilter);
	texture->SetParameter(GL_TEXTURE_MAG_FILTER, attachment.maxFilter);
	texture->SetParameter(GL_TEXTURE_WRAP_S,     attachment.wrapMode);
	texture->SetParameter(GL_TEXTURE_WRAP_T,     attachment.wrapMode);

	// Set border
	if (attachment.border != nullptr)
	{
		texture->SetParameter(GL_TEXTURE_BORDER_COLOR, attachment.border);
	}

	// If attachment has (or will have) mipmaps
	if (CheckMipMap(attachment.minFilter))
	{
		// Allocate memory for base texture and mipmaps
		texture->Storage3DMipMap(attachment.intFormat);
	}
	else
	{
		// Allocate memory for base texture
		texture->Storage3D(attachment.intFormat);
	}
}

void FrameBuffer::AttachTexture(TxPtr& texture, const FBOAttachment& attachment)
{
	// Attach texture
	glFramebufferTexture2D
	(
		GL_FRAMEBUFFER,
		attachment.slot,
		texture->type,
		texture->id,
		0
	);
}

UNUSED void FrameBuffer::AttachTextureCubeMap(UNUSED TxPtr& texture, UNUSED const FBOAttachment& attachment)
{
	// Throw error just in case I use this
	throw std::runtime_error("OpenGL has different rules on cubemap attachment!");
}

void FrameBuffer::AttachTextureArray(TxPtr& texture, const FBOAttachment& attachment)
{
	// Attach texture
	glFramebufferTexture
	(
		GL_FRAMEBUFFER,
		attachment.slot,
		texture->id,
		0
	);
}

void FrameBuffer::AddTexture(TxPtr& texture, const FBOAttachment& attachment)
{
	// Create texture
	CreateTexture(texture, attachment);
	// Attach texture
	AttachTexture(texture, attachment);
}

void FrameBuffer::AddTextureCubeMap(TxPtr& texture, const FBOAttachment& attachment)
{
	// Create cube map
	CreateTextureCubeMap(texture, attachment);
	// Don't attach cube map (IT WILL BREAK EVERYTHING)
	// AttachCubeMap(texture, attachment);
}

void FrameBuffer::AddTextureArray(TxPtr& texture, const FBOAttachment& attachment)
{
	// Create texture array
	CreateTextureArray(texture, attachment);
	// Attach texture
	AttachTextureArray(texture, attachment);
}

void FrameBuffer::AddBuffer(RdBufPtr& buffer, const FBOAttachment& attachment)
{
	// Create render buffer
	buffer = std::make_shared<RenderBuffer>
	(
		width,
		height,
		attachment.intFormat
	);

	// Attach render buffer
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
	// Set draw buffer
	glDrawBuffer(value);
}

void FrameBuffer::SetReadBuffer(GLenum value)
{
	// Set read buffer
	glReadBuffer(value);
}

void FrameBuffer::SetParameter(GLenum pname, GLint param)
{
	// Set parameter
	glFramebufferParameteri(GL_FRAMEBUFFER, pname, param);
}

void FrameBuffer::SetDrawBuffers(const std::vector<GLenum>& buffers)
{
	// Set draw buffers
	glDrawBuffers(static_cast<GLsizei>(buffers.size()), buffers.data());
}

void FrameBuffer::EnableDepth()
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	// Set default range
	glDepthRange(0.0, 1.0);
}

void FrameBuffer::CheckStatus()
{
	// Error map
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

	// Check status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	// If framebuffer is incomplete
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		// Look up error
		auto errorStr = GL_FRAMEBUFFER_ERROR_TYPES.find(status)->second;
		// Log error
		LOG_ERROR("{} Framebuffer [ID={}] is incomplete!\n", errorStr, id);
	}
}

void FrameBuffer::Bind() const
{
	// Bind buffer
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	// Set viewport
	glViewport(0, 0, width, height);
}

void FrameBuffer::Unbind() const
{
	// Get settings
	const auto& settings = Settings::GetInstance();
	// Unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Set viewport
	glViewport(0, 0, settings.window.dimensions.x, settings.window.dimensions.y);
}

bool FrameBuffer::CheckMipMap(GLenum minFilter)
{
	// Array containing mip map filters
	constexpr std::array<GLenum, 4> hasMipMaps =
	{
		GL_NEAREST_MIPMAP_NEAREST,
		GL_LINEAR_MIPMAP_NEAREST,
		GL_NEAREST_MIPMAP_LINEAR,
		GL_LINEAR_MIPMAP_LINEAR
	};

	// Search for filter in array
	auto result = std::find(hasMipMaps.begin(), hasMipMaps.end(), minFilter);

	// If found
	if (result != hasMipMaps.end())
	{
		// Return true
		return true;
	}
	// If not found
	else
	{
		// Return false
		return false;
	}
}

FrameBuffer::~FrameBuffer()
{
	// ID should not be zero
	if (id != 0)
	{
		// Clear FBO
		glDeleteFramebuffers(1, &id);
	}
}