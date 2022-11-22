#ifndef LIGHTS_BUFFER_H
#define LIGHTS_BUFFER_H

#include <vector>

#include "UniformBuffer.h"
#include "PointLight.h"
#include "GL.h"
#include "Util.h"
#include "DirectionalLight.h"

namespace Renderer
{
	// Must match the shaders constant
	constexpr usize SHADER_MAX_LIGHTS = 4;

	class LightsBuffer : public UniformBuffer
	{
	public:
		LightsBuffer();
		// Load directional lights
		void LoadDirectionalLights(const std::vector<Entities::DirectionalLight>& lights);
		// Load point lights
		void LoadPointLights(const std::vector<Entities::PointLight>& lights);
	};

	namespace Detail
	{
		struct LightsBufferGLSL
		{
			// Directional lights
			alignas(16) GL::Int                    numDirLights;
			alignas(16) Entities::DirectionalLight dirLights[SHADER_MAX_LIGHTS];
			// Point lights
			alignas(16) GL::Int              numPointLights;
			alignas(16) Entities::PointLight pointLights[SHADER_MAX_LIGHTS];
		};
	}
}

#endif