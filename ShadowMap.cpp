#include "ShadowMap.h"

using namespace Renderer;

constexpr glm::ivec2 SHADOW_DIMENSIONS = {1024, 1024};

ShadowMap::ShadowMap()
	: buffer(std::make_shared<FrameBuffer>(SHADOW_DIMENSIONS.x, SHADOW_DIMENSIONS.y, FBType::Depth, FBFilter::Nearest))
{
}

void ShadowMap::BindShadowFBO() const
{
	buffer->Bind();
}

void ShadowMap::BindDefaultFBO() const
{
	buffer->Unbind();
}
