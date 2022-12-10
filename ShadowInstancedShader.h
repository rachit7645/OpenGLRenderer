#ifndef INSTANCED_SHADOW_SHADER_H
#define INSTANCED_SHADOW_SHADER_H

#include "Shader.h"

namespace Shader
{
	class ShadowInstancedShader : public ShaderProgram
	{
	public:
		// Constructor
		ShadowInstancedShader();
		// Get all uniform locations (Unused in this shader)
		void GetUniformLocations() override;
	};
}

#endif
