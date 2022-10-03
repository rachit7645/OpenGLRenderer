#ifndef SHADOW_FRAME_BUFFER_H
#define SHADOW_FRAME_BUFFER_H

#include <memory>

#include "GLM.h"
#include "FrameBuffer.h"

namespace Renderer
{
	constexpr glm::ivec2 SHADOW_DIMENSIONS = {1024, 1024};

	class ShadowFrameBuffer
	{
	public:
		using FbPtr = std::shared_ptr<FrameBuffer>;

		ShadowFrameBuffer();

		void BindShadowFBO()  const;
		void BindDefaultFBO() const;

		FbPtr buffer;
	};
}

#endif