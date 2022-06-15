#ifndef LIGHTS_BUFFER_H
#define LIGHTS_BUFFER_H

#include <GL/glew.h>

#include "Util.h"
#include "GLM.h"
#include "UniformBuffer.h"
#include "Light.h"

namespace Renderer
{
	constexpr auto SHADER_MAX_LIGHTS = 4;

	class LightsBuffer : public UniformBuffer
	{
	public:
		LightsBuffer();
		void LoadLight(const std::vector<Entities::Light>& lights);
	};
}

#endif