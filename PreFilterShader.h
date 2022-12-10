#ifndef PRE_FILTER_SHADER_H
#define PRE_FILTER_SHADER_H

#include "GLM.h"
#include "Shader.h"
#include "Util.h"

namespace Shader
{
	class PreFilterShader : public ShaderProgram
	{
	public:
		// Constructor
		PreFilterShader();

		// Connect GPU texture units
		void ConnectTextureUnits();
		// Load projection matrix
		void LoadProjection(const glm::mat4& projection);
		// Load view matrix
		void LoadView(const glm::mat4& view);
		// Load current roughness
		void LoadRoughness(f32 roughness);
		// Load environment map resolution
		void LoadResolution(const glm::vec2& resolution);

		// Get all uniform locations
		void GetUniformLocations() override;
	};
}

#endif
