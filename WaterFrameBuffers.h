#ifndef WATER_FRAME_BUFFERS_H
#define WATER_FRAME_BUFFERS_H

#include <memory>

#include "GLM.h"
#include "FrameBuffer.h"

namespace Waters
{
	constexpr glm::ivec2 WATER_REFLECTION_SIZE = {320, 180};
	constexpr glm::ivec2 WATER_REFRACTION_SIZE = {320, 180};

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
