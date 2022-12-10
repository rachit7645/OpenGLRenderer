#ifndef LIGHTS_BUFFER_H
#define LIGHTS_BUFFER_H

#include <vector>

#include "UniformBuffer.h"
#include "GL.h"
#include "Util.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

namespace Renderer
{
	// Must match the shaders constant
	constexpr usize SHADER_MAX_LIGHTS = 4;

	// A class describing a lights buffer
	class LightsBuffer : public UniformBuffer
	{
	public:
		LightsBuffer();
		// Load directional lights
		void LoadDirectionalLights(const std::vector<Entities::DirectionalLight>& lights);
		// Load point lights
		void LoadPointLights(const std::vector<Entities::PointLight>& lights);
		// Load spot lights
		void LoadSpotLights(const std::vector<Entities::SpotLight>& lights);
	};

	namespace Detail
	{
		// Usings
		using Entities::DirectionalLight;
		using Entities::PointLight;
		using Entities::SpotLight;

		// GLSL Data
		struct LightsBufferGLSL
		{
			// Directional lights
			alignas(16) GL::Int          numDirLights;
			alignas(16) DirectionalLight dirLights[SHADER_MAX_LIGHTS];
			// Point lights
			alignas(16) GL::Int    numPointLights;
			alignas(16) PointLight pointLights[SHADER_MAX_LIGHTS];
			// Spot lights
			alignas(16) GL::Int   numSpotLights;
			alignas(16) SpotLight spotLights[SHADER_MAX_LIGHTS];
		};
	}
}

#endif