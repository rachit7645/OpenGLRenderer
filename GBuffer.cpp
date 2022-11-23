#include "GBuffer.h"

#include <vector>
#include <GL/glew.h>

#include "FBOAttachment.h"
#include "Window.h"

using namespace Renderer;

// TODO: Pack normals and normal map values
// TODO: Convert normal map to world space in geometry pass (faster)
// TODO: Reduce gbuffer size

// GBuffer Layout
// Buffer     | Type      | R           | G           | B           | A
// Normal     | RGBA_16F  | normal.x    | normal.y    | normal.z    | roughness
// Albedo     | RGBA_16F  | albedo.r    | albedo.g    | albedo.b    | metallic
// Normal Map | RGBA_16F  | normalMap.r | normalMap.g | normalMap.b | ambient occlusion
// Depth      | DEPTH_24F | depth       | NONE        | NONE        | NONE

GBuffer::GBuffer()
	: buffer(std::make_shared<FrameBuffer>())
{
	// Normal attachment
	Renderer::FBOAttachment normal =
	{
		GL_NEAREST,
		GL_NEAREST,
		GL_CLAMP_TO_EDGE,
		GL_RGBA16F,
		GL_RGBA,
		GL_FLOAT,
		GL_COLOR_ATTACHMENT1
	};

	// Albedo attachment
	Renderer::FBOAttachment albedo =
	{
		GL_NEAREST,
		GL_NEAREST,
		GL_CLAMP_TO_EDGE,
		GL_RGBA16F,
		GL_RGBA,
		GL_FLOAT,
		GL_COLOR_ATTACHMENT2
	};

	// Normal map attachment
	Renderer::FBOAttachment normalMap =
	{
		GL_NEAREST,
		GL_NEAREST,
		GL_CLAMP_TO_EDGE,
		GL_RGBA16F,
		GL_RGBA,
		GL_FLOAT,
		GL_COLOR_ATTACHMENT3
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
		normalMap.slot
	};

	// Set buffer width and height
	buffer->width  = Window::WINDOW_DIMENSIONS.x;
	buffer->height = Window::WINDOW_DIMENSIONS.y;

	// Create frame buffer
	buffer->CreateFrameBuffer();
	buffer->Bind();
	buffer->AddTexture(buffer->colorTextures[0], normal);
	buffer->AddTexture(buffer->colorTextures[1], albedo);
	buffer->AddTexture(buffer->colorTextures[2], normalMap);
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
