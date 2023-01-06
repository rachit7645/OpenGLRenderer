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

	// Color Buffer
	Renderer::FBOAttachment color0 =
	{
		GL_LINEAR,
		GL_LINEAR,
		GL_CLAMP_TO_EDGE,
		GL_RGB16F,
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
	buffer->width  = settings.window.dimensions.x;
	buffer->height = settings.window.dimensions.y;

	// Create frame buffer
	buffer->CreateFrameBuffer();
	buffer->Bind();
	buffer->AddBuffer(buffer->depthRenderBuffer, depth);
	buffer->SetDrawBuffers(drawBuffers);

	// Store mipmap size
	auto mipSize = settings.window.dimensions;

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

		// Make sure mip size is more than 0
		if (mipSize.x == 0 || mipSize.y == 0)
		{
			mipSize = {1, 1};
		}

		// Set up texture
		auto texture = std::make_shared<Texture>();
		// Set FBO dimensions
		buffer->width  = mipSize.x;
		buffer->height = mipSize.y;
		// Create texture
		buffer->CreateTexture(texture, color0);
		// Add texture
		mipChain.emplace_back(texture);
	}

	// Reset buffer width and height
	buffer->width  = settings.window.dimensions.x;
	buffer->height = settings.window.dimensions.y;
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