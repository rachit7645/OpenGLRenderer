#include "GBuffer.h"

#include <vector>
#include <GL/glew.h>

#include "FBOAttachment.h"
#include "Window.h"
#include "Settings.h"

using namespace Renderer;

using Engine::Settings;

// GBuffer Layout
// Buffer   | Type      | R        | G         | B        | A
// Normal   | RG_16F    | normal.x | normal.y  | NONE     | NONE
// Albedo   | RGB_8U    | albedo.r | albedo.g  | albedo.b | NONE
// Material | RGB_8U    | ao       | roughness | metallic | NONE
// Depth    | DEPTH_24F | glDepth  | NONE      | NONE     | NONE

GBuffer::GBuffer()
	: buffer(std::make_shared<FrameBuffer>())
{
	// Get settings
	const auto& settings = Settings::GetInstance();

	// Normal attachment
	Renderer::FBOAttachment normal =
	{
		GL_NEAREST,
		GL_NEAREST,
		GL_CLAMP_TO_EDGE,
		GL_RG16F,
		GL_RG,
		GL_FLOAT,
		GL_COLOR_ATTACHMENT0
	};

	// Albedo attachment
	Renderer::FBOAttachment albedo =
	{
		GL_NEAREST,
		GL_NEAREST,
		GL_CLAMP_TO_EDGE,
		GL_RGB8,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		GL_COLOR_ATTACHMENT1
	};

	// Material attachment
	Renderer::FBOAttachment material =
	{
		GL_NEAREST,
		GL_NEAREST,
		GL_CLAMP_TO_EDGE,
		GL_RGB8,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		GL_COLOR_ATTACHMENT2
	};

	// Depth attachment
	Renderer::FBOAttachment depth =
	{
		GL_NEAREST,
		GL_NEAREST,
		GL_CLAMP_TO_EDGE,
		GL_DEPTH_COMPONENT24,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		GL_DEPTH_ATTACHMENT
	};

	// Selected draw buffers
	std::vector<GLenum> drawBuffers =
	{
		normal.slot,
		albedo.slot,
		material.slot
	};

	// Set buffer width and height
	buffer->width  = settings.window.dimensions.x;
	buffer->height = settings.window.dimensions.y;

	// Create frame buffer
	buffer->CreateFrameBuffer();
	buffer->Bind();
	buffer->AddTexture(buffer->colorTextures[0], normal);
	buffer->AddTexture(buffer->colorTextures[1], albedo);
	buffer->AddTexture(buffer->colorTextures[2], material);
	buffer->AddTexture(buffer->depthTexture,     depth);
	buffer->SetDrawBuffers(drawBuffers);
	buffer->CheckStatus();
	buffer->EnableDepth();
	buffer->Unbind();
}

void GBuffer::BindGBuffer() const
{
	// Bind
	buffer->Bind();
}

void GBuffer::BindDefaultFBO() const
{
	// Unbind
	buffer->Unbind();
}
