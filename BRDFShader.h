#ifndef BRDF_SHADER_H
#define BRDF_SHADER_H

#include "Shader.h"

namespace Shader
{
	class BRDFShader : public ShaderProgram
	{
	public:
		// Constructor
		BRDFShader();
		// Get all uniform locations (Unused in this shader)
		void GetUniformLocations() override;
	};
}

#endif
