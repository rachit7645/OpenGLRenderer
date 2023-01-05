#include "BloomBuffer.h"

#include <vector>

#include "FBOAttachment.h"
#include "Settings.h"
#include "Util.h"

// Using namespaces
using namespace Renderer;

// Usings
using Engine::Settings;

// Constants
constexpr usize MAX_MIP_CHAIN_LENGTH = 128;

BloomBuffer::BloomBuffer()
	: buffer(std::make_shared<FrameBuffer>())
{
	// Get settings
	const auto& settings = Settings::GetInstance();
	// Store mipmap size
	auto mipSize = settings.window.dimensions;

	// Color Buffer
	Renderer::FBOAttachment color0 =
	{
		GL_LINEAR,
		GL_LINEAR,
		GL_CLAMP_TO_EDGE,
		GL_R11F_G11F_B10F,
		GL_RGB,
		GL_FLOAT,
		GL_COLOR_ATTACHMENT0
	};

	// Depth attachment (unused)
	Renderer::FBOAttachment depth = {};
	{
		depth.intFormat = GL_DEPTH_COMPONENT24;
		depth.slot      = GL_DEPTH_ATTACHMENT;
	}

	// Draw buffers
	std::vector<GLenum> drawBuffers =
	{
		color0.slot
	};

	// Set buffer width and height
	buffer->width  = mipSize.x;
	buffer->height = mipSize.y;

	// Create frame buffer
	buffer->CreateFrameBuffer();
	buffer->Bind();
	buffer->AddBuffer(buffer->depthRenderBuffer, depth);
	buffer->SetDrawBuffers(drawBuffers);

	// For all mipmaps
	for (usize i = 0; i < MAX_MIP_CHAIN_LENGTH; ++i)
	{
		// Check mip size
		if (mipSize.x == 1 && mipSize.y == 1)
		{
			// Mip chain is complete
			break;
		}

		// Reduce mip-map size
		mipSize /= 2;

		// Check mip size
		if (mipSize.x == 0 || mipSize.y == 0)
		{
			mipSize = {1, 1};
		}

		// Set up texture
		auto texture    = std::make_shared<Texture>();
		texture->width  = mipSize.x;
		texture->height = mipSize.y;
		texture->type   = GL_TEXTURE_2D;

		// Create texture
		texture->CreateTexture();
		texture->Bind();
		texture->SetParameter(GL_TEXTURE_MIN_FILTER, color0.minFilter);
		texture->SetParameter(GL_TEXTURE_MAG_FILTER, color0.maxFilter);
		texture->SetParameter(GL_TEXTURE_WRAP_S,     color0.wrapMode);
		texture->SetParameter(GL_TEXTURE_WRAP_T,     color0.wrapMode);

		// Load data
		texture->LoadImageData
		(
			nullptr,
			color0.intFormat,
			color0.format,
			color0.dataType
		);

		// Unbind texture
		texture->Unbind();
		// Add texture
		mipChain.emplace_back(texture);
	}

	// Finish FBO creation
	buffer->CheckStatus();
	buffer->EnableDepth();
	buffer->Unbind();
}

void BloomBuffer::BindBloomBuffer() const
{
	// Bind
	buffer->Bind();
}

void BloomBuffer::BindDefaultFBO() const
{
	// Unbind
	buffer->Unbind();
}