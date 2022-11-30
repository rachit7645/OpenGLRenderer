#ifndef CONVOLUTION_SHADER_H
#define CONVOLUTION_SHADER_H

#include "GLM.h"
#include "Shader.h"

namespace Shader
{
	class ConvolutionShader : public ShaderProgram
	{
	public:
		// Constructor
		ConvolutionShader();

		// Connect GPU texture units
		void ConnectTextureUnits();
		// Load projection matrix
		void LoadProjection(const glm::mat4& projection);
		// Load view matrix
		void LoadView(const glm::mat4& view);

		// Get all uniform locations
		void GetUniformLocations() override;
	};
}

#endif
