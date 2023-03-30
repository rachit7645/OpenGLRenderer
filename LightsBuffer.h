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
        // GLSL Data
        struct ALIGN_GLSL_STD140 LightsBufferGLSL
        {
            // Directional lights
            GL::Int                    numDirLights;
            Entities::DirectionalLight dirLights[SHADER_MAX_LIGHTS];
            // Point lights
            GL::Int              numPointLights;
            Entities::PointLight pointLights[SHADER_MAX_LIGHTS];
            // Spot lights
            GL::Int             numSpotLights;
            Entities::SpotLight spotLights[SHADER_MAX_LIGHTS];
        };

        // Main constructor
		LightsBuffer();

        // Load directional lights
		void LoadDirectionalLights(const std::vector<Entities::DirectionalLight>& lights);
		// Load point lights
		void LoadPointLights(const std::vector<Entities::PointLight>& lights);
		// Load spot lights
		void LoadSpotLights(const std::vector<Entities::SpotLight>& lights);
	};
}

#endif