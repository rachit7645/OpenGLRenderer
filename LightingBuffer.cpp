#include "LightingBuffer.h"

#include <vector>

#include "FBOAttachment.h"
#include "Settings.h"

// Using namespaces
using namespace Renderer;

// Usings
using Engine::Settings;

LightingBuffer::LightingBuffer()
	: buffer(std::make_shared<FrameBuffer>())
{
	// Get settings
	const auto& settings = Settings::GetInstance();

	// Color Buffer
	Renderer::FBOAttachment color0 =
	{
		GL_NEAREST,
		GL_NEAREST,
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
	buffer->AddTexture(buffer->colorTextures[0], color0);
	buffer->AddBuffer(buffer->depthRenderBuffer, depth);
	buffer->SetDrawBuffers(drawBuffers);
	buffer->CheckStatus();
	buffer->EnableDepth();
	buffer->Unbind();
}

void LightingBuffer::BindLightingBuffer() const
{
	// Bind
	buffer->Bind();
}

void LightingBuffer::BindDefaultFBO() const
{
	// Unbind
	buffer->Unbind();
}