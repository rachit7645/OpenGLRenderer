#include "WaterFrameBuffers.h"
// Hack to fix circular includes
#include "Window.h"

using namespace Waters;

using Renderer::FrameBuffer;
using Renderer::FBType;

// Render at 0.25x the resolution
constexpr glm::ivec2 WATER_REFLECTION_SIZE = {Window::DIMENSIONS / 4};
constexpr glm::ivec2 WATER_REFRACTION_SIZE = {Window::DIMENSIONS / 4};

WaterFrameBuffers::WaterFrameBuffers()
	: reflectionFBO(std::make_shared<FrameBuffer>(WATER_REFLECTION_SIZE.x, WATER_REFLECTION_SIZE.y, FBType::Color)),
	  refractionFBO(std::make_shared<FrameBuffer>(WATER_REFRACTION_SIZE.x, WATER_REFRACTION_SIZE.y, FBType::Color))
{
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