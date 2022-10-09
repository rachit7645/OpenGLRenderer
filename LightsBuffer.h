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
		void LoadLightProjection(const glm::mat4& projection);
		void LoadLightView(const glm::mat4& view);
	};

	namespace Detail
	{
		struct LightsBufferGLSL
		{
			alignas(16) Entities::Light lights[SHADER_MAX_LIGHTS];
			alignas(16) glm::mat4       lightProj;
			alignas(16) glm::mat4       lightView;
		};
	}
}

#endif