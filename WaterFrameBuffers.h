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
		// Usings
		using FbPtr = std::shared_ptr<Renderer::FrameBuffer>;

		// Main constructor
		WaterFrameBuffers();

		// Binding functions
		void BindReflection() const;
		void BindRefraction() const;
		void BindDefaultFBO() const;

		// Framebuffers
		FbPtr reflectionFBO;
		FbPtr refractionFBO;
	};
}

#endif
