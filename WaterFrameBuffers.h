#ifndef WATER_FRAME_BUFFERS_H
#define WATER_FRAME_BUFFERS_H

#include <memory>

#include "GLM.h"
#include "FrameBuffer.h"

namespace Waters
{
	class WaterFrameBuffers
	{
	public:
		using FbPtr = std::shared_ptr<Renderer::FrameBuffer>;

		// Default constructor
		WaterFrameBuffers();

		void BindReflection() const;
		void BindRefraction() const;
		void BindDefaultFBO() const;

		// Framebuffers
		FbPtr reflectionFBO;
		FbPtr refractionFBO;
	};
}

#endif
