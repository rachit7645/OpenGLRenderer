#include "GBuffer.h"

#include <vector>
#include <GL/glew.h>

#include "GLM.h"
#include "FBOAttachment.h"
#include "Window.h"

using namespace Renderer;

GBuffer::GBuffer()
	: buffer(std::make_shared<FrameBuffer>())
{
	// Normal attachment
	Renderer::FBOAttachment normal =
	{
		GL_NEAREST,
		GL_NEAREST,
		GL_CLAMP_TO_EDGE,
		GL_RGBA32F,
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
		GL_RGBA32F,
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
		GL_RGBA32F,
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
