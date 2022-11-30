#ifndef GEOMETRY_PASS_SHADER_H
#define GEOMETRY_PASS_SHADER_H

#include "GLM.h"
#include "Shader.h"

namespace Shader
{
	class GBufferShader : public ShaderProgram
	{
	public:
		// Constructor
		GBufferShader();

		// Connect GPU texture units
		void ConnectTextureUnits();

		// Get all uniform locations
		void GetUniformLocations() override;
	};
}

#endif