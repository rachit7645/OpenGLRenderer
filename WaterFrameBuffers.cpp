#include "WaterFrameBuffers.h"

#include "Window.h"
#include "FBOAttachment.h"
#include "Settings.h"

using namespace Waters;

using Renderer::FrameBuffer;
using Engine::Settings;

WaterFrameBuffers::WaterFrameBuffers()
	: reflectionFBO(std::make_shared<FrameBuffer>()),
	  refractionFBO(std::make_shared<FrameBuffer>())
{
	// Get settings
	const auto& settings = Settings::GetInstance();
	// Calculate sizes
	auto reflectionSize = settings.window.dimensions / 4;
	auto refractionSize = settings.window.dimensions / 4;

	Renderer::FBOAttachment color0 =
	{
		GL_LINEAR,
		GL_LINEAR,
		GL_REPEAT,
		GL_RGB,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		GL_COLOR_ATTACHMENT0
	};

	// We don't need to fill this up completely
	Renderer::FBOAttachment depth = {};
	{
		depth.intFormat = GL_DEPTH_COMPONENT24;
		depth.slot           = GL_DEPTH_ATTACHMENT;
	}

	reflectionFBO->width  = reflectionSize.x;
	reflectionFBO->height = reflectionSize.y;

	reflectionFBO->CreateFrameBuffer();
	reflectionFBO->Bind();
	reflectionFBO->AddTexture(reflectionFBO->colorTextures[0], color0);
	reflectionFBO->AddBuffer(reflectionFBO->depthRenderBuffer, depth);
	reflectionFBO->CheckStatus();
	reflectionFBO->EnableDepth();
	reflectionFBO->Unbind();

	refractionFBO->width  = refractionSize.x;
	refractionFBO->height = refractionSize.y;

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