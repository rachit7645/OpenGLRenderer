#include "WaterFrameBuffers.h"
// Hack to fix circular includes
#include "Window.h"

using namespace Waters;

using Renderer::FrameBuffer;

// Render at 0.25x the resolution
constexpr glm::ivec2 WATER_REFLECTION_SIZE = {Window::DIMENSIONS / 4};
constexpr glm::ivec2 WATER_REFRACTION_SIZE = {Window::DIMENSIONS / 4};

WaterFrameBuffers::WaterFrameBuffers()
	: reflectionFBO(std::make_shared<FrameBuffer>()),
	  refractionFBO(std::make_shared<FrameBuffer>())
{
	reflectionFBO->width  = WATER_REFLECTION_SIZE.x;
	reflectionFBO->height = WATER_REFLECTION_SIZE.y;

	reflectionFBO->CreateFrameBuffer();
	reflectionFBO->Bind();
	reflectionFBO->CreateColorTexture();
	reflectionFBO->CreateDepthBuffer();
	reflectionFBO->CheckStatus();
	reflectionFBO->EnableDepth();
	reflectionFBO->Unbind();

	refractionFBO->width  = WATER_REFRACTION_SIZE.x;
	refractionFBO->height = WATER_REFRACTION_SIZE.y;

	refractionFBO->CreateFrameBuffer();
	refractionFBO->Bind();
	refractionFBO->CreateColorTexture();
	refractionFBO->CreateDepthBuffer();
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