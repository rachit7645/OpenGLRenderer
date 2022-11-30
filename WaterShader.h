#ifndef WATER_SHADER_H
#define WATER_SHADER_H

#include "Shader.h"
#include "GLM.h"
#include "Util.h"

namespace Shader
{
	class WaterShader : public ShaderProgram
	{
	public:
		// Constructor
		WaterShader();

		// Load model matrix
		void LoadModelMatrix(const glm::mat4& matrix);
		// Load water move factor
		void LoadMoveFactor(f32 moveFactor);
		// Connect GPU texture units
		void ConnectTextureUnits();

		// Get all uniform locations
		void GetUniformLocations() override;
	};
}

#endif