#ifndef LIGHTS_BUFFER_H
#define LIGHTS_BUFFER_H

#include <vector>

#include "UniformBuffer.h"
#include "Light.h"

namespace Renderer
{
	constexpr auto SHADER_MAX_LIGHTS = 4;

	class LightsBuffer : public UniformBuffer
	{
	public:
		LightsBuffer();
		
		void LoadLights(const std::vector<Entities::Light>& lights);
	};
}

#endif