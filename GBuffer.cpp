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
	Renderer::FBOAttachment position =
	{
		GL_NEAREST,
		GL_NEAREST,
		GL_REPEAT,
		GL_RGBA32F,
		GL_RGBA,
		GL_FLOAT,
		GL_COLOR_ATTACHMENT0
	};

	Renderer::FBOAttachment normal =
	{
		GL_NEAREST,
		GL_NEAREST,
		GL_REPEAT,
		GL_RGBA32F,
		GL_RGBA,
		GL_FLOAT,
		GL_COLOR_ATTACHMENT1
	};

	Renderer::FBOAttachment albedo =
	{
		GL_NEAREST,
		GL_NEAREST,
		GL_REPEAT,
		GL_RGBA,
		GL_RGBA,
		GL_FLOAT,
		GL_COLOR_ATTACHMENT2
	};

	Renderer::FBOAttachment normalMap =
	{
		GL_NEAREST,
		GL_NEAREST,
		GL_REPEAT,
		GL_RGBA32F,
		GL_RGBA,
		GL_FLOAT,
		GL_COLOR_ATTACHMENT3
	};

	Renderer::FBOAttachment depth = {};
	{
		depth.internalFormat = GL_DEPTH_COMPONENT24;
		depth.slot           = GL_DEPTH_ATTACHMENT;
	}

	std::vector<GLenum> drawBuffers =
	{
		position.slot,
		normal.slot,
		albedo.slot,
		normalMap.slot
	};

	buffer->width  = Window::DIMENSIONS.x;
	buffer->height = Window::DIMENSIONS.y;

	buffer->CreateFrameBuffer();
	buffer->Bind();
	buffer->AddTexture(buffer->colorTextures[0], position);
	buffer->AddTexture(buffer->colorTextures[1], normal);
	buffer->AddTexture(buffer->colorTextures[2], albedo);
	buffer->AddTexture(buffer->colorTextures[3], normalMap);
	buffer->AddBuffer(buffer->depthRenderBuffer, depth);
	buffer->SetDrawBuffers(drawBuffers);
	buffer->CheckStatus();
	buffer->EnableDepth();
	buffer->Unbind();
}

void GBuffer::BindGBuffer() const
{
	buffer->Bind();
}

void GBuffer::BindDefaultFBO() const
{
	buffer->Unbind();
}
