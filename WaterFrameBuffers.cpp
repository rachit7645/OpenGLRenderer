#include "WaterFrameBuffers.h"

#include "Window.h"
#include "FBOAttachment.h"

using namespace Waters;

using Renderer::FrameBuffer;

// Render at 0.25x the resolution
constexpr glm::ivec2 WATER_REFLECTION_SIZE = {Window::DIMENSIONS / 4};
constexpr glm::ivec2 WATER_REFRACTION_SIZE = {Window::DIMENSIONS / 4};

WaterFrameBuffers::WaterFrameBuffers()
	: reflectionFBO(std::make_shared<FrameBuffer>()),
	  refractionFBO(std::make_shared<FrameBuffer>())
{
	Renderer::FBOAttachment color0 =
	{
		GL_LINEAR,
		GL_LINEAR,
		GL_REPEAT,
		GL_RGBA,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		GL_COLOR_ATTACHMENT0
	};

	// We don't need to fill this up completely
	Renderer::FBOAttachment depth = {};
	{
		depth.internalFormat = GL_DEPTH_COMPONENT24;
		depth.slot           = GL_DEPTH_ATTACHMENT;
	}

	reflectionFBO->width  = WATER_REFLECTION_SIZE.x;
	reflectionFBO->height = WATER_REFLECTION_SIZE.y;

	reflectionFBO->CreateFrameBuffer();
	reflectionFBO->Bind();
	reflectionFBO->AddTexture(reflectionFBO->colorTextures[0], color0);
	reflectionFBO->AddBuffer(reflectionFBO->depthRenderBuffer, depth);
	reflectionFBO->CheckStatus();
	reflectionFBO->EnableDepth();
	reflectionFBO->Unbind();

	refractionFBO->width  = WATER_REFRACTION_SIZE.x;
	refractionFBO->height = WATER_REFRACTION_SIZE.y;

	refractionFBO->CreateFrameBuffer();
	refractionFBO->Bind();
	refractionFBO->AddTexture(refractionFBO->colorTextures[0], color0);
	refractionFBO->AddBuffer(refractionFBO->depthRenderBuffer, depth);
	refractionFBO->CheckStatus();
	refractionFBO->EnableDepth();
	refractionFBO->Unbind();
}

void WaterFrameBuffers::BindReflection() const
{
	reflectionFBO->Bind();
}

void WaterFrameBuffers::BindRefraction() const
{
	refractionFBO->Bind();
}

void WaterFrameBuffers::BindDefaultFBO() const
{
	// We could also use the refraction FBO for this purpose
	reflectionFBO->Unbind();
}