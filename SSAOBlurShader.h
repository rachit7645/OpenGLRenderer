#ifndef SSAO_BLUR_SHADER_H
#define SSAO_BLUR_SHADER_H

#include "GLM.h"
#include "Shader.h"

namespace Shader
{
	class SSAOBlurShader : public ShaderProgram
	{
	public:
		// Constructor
		SSAOBlurShader();
		// Get all uniform locations
		void GetUniformLocations() override;
		// Connect texture units
		void ConnectTextureUnits();
	};
}

#endif
