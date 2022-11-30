#ifndef GUI_SHADER_H
#define GUI_SHADER_H

#include "Shader.h"

namespace Shader
{
	class GUIShader : public ShaderProgram
	{
	public:
		// Constructor
		GUIShader();

		// Load model matrix
		void LoadModelMatrix(const glm::mat4& matrix);

		// Get all uniform locations
		void GetUniformLocations() override;
	};
}

#endif
