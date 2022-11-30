#ifndef SKYBOX_SHADER_H
#define SKYBOX_SHADER_H

#include "GLM.h"
#include "Shader.h"

namespace Shader
{
	class SkyboxShader : public ShaderProgram
	{
	public:
		// Constructor
		SkyboxShader();

		// Load model matrix
		void LoadModelMatrix(const glm::mat4& matrix);

		// Get all uniform locations
		void GetUniformLocations() override;
	};
}

#endif