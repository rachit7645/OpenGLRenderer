#ifndef WATER_FRAME_BUFFERS_H
#define WATER_FRAME_BUFFERS_H

#include <memory>

#include "GLM.h"
#include "FrameBuffer.h"
#include "Window.h"

namespace Waters
{
	// Render at 1/3 the resolution
	constexpr glm::ivec2 WATER_REFLECTION_SIZE = {Window::DIMENSIONS / 3};
	constexpr glm::ivec2 WATER_REFRACTION_SIZE = {Window::DIMENSIONS / 3};

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
