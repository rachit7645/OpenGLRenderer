#ifndef SSAO_SHADER_H
#define SSAO_SHADER_H

#include "GLM.h"
#include "Shader.h"

namespace Shader
{
	class SSAOShader : public ShaderProgram
	{
	public:
		// Constructor
		SSAOShader();
		// Get all uniform locations
		void GetUniformLocations() override;
		// Connect texture units
		void ConnectTextureUnits();
	};
}

#endif
