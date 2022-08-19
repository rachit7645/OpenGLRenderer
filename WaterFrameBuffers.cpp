#include "WaterFrameBuffers.h"

using namespace Waters;

using Renderer::FrameBuffer;
using Renderer::FBType;

WaterFrameBuffers::WaterFrameBuffers()
	: reflectionFBO(std::make_shared<FrameBuffer>(WATER_REFLECTION_SIZE.x, WATER_REFLECTION_SIZE.y, FBType::Color)),
	  refractionFBO(std::make_shared<FrameBuffer>(WATER_REFRACTION_SIZE.x, WATER_REFRACTION_SIZE.y, FBType::ColorAndDepth))
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
