#include "ShadowFrameBuffer.h"

using namespace Renderer;

ShadowFrameBuffer::ShadowFrameBuffer()
	: buffer(std::make_shared<FrameBuffer>(SHADOW_DIMENSIONS.x, SHADOW_DIMENSIONS.y, FBType::Depth))
{
}

void ShadowFrameBuffer::BindShadowFBO() const
{
	buffer->Bind();
}

void ShadowFrameBuffer::BindDefaultFBO() const
{
	buffer->Unbind();
}
