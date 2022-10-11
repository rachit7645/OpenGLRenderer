#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include <memory>

#include "GLM.h"
#include "FrameBuffer.h"

namespace Renderer
{
	class ShadowMap
	{
	public:
		using FbPtr = std::shared_ptr<FrameBuffer>;

		ShadowMap();

		void BindShadowFBO()  const;
		void BindDefaultFBO() const;

		FbPtr buffer;
	};
}

#endif