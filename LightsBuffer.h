#ifndef LIGHTS_BUFFER_H
#define LIGHTS_BUFFER_H

#include <vector>

#include "UniformBuffer.h"
#include "Light.h"
#include "GL.h"
#include "Util.h"

namespace Renderer
{
	constexpr usize SHADER_MAX_LIGHTS = 4;

	class LightsBuffer : public UniformBuffer
	{
	public:
		LightsBuffer();
		
		void LoadLights(const std::vector<Entities::Light>& lights);
	};

	namespace Detail
	{
		struct LightsBufferGLSL
		{
			alignas(16) GL::Int         numLights;
			alignas(16) Entities::Light lights[SHADER_MAX_LIGHTS];
		};
	}
}

#endif