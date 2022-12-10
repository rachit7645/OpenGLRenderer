#ifndef FAST_INSTANCED_SHADER_H
#define FAST_INSTANCED_SHADER_H

#include "Shader.h"

namespace Shader
{
	class FastInstancedShader : public ShaderProgram
	{
	public:
		// Constructor
		FastInstancedShader();
		// Get all uniform locations (Unused in this shader)
		void GetUniformLocations() override;
	};
}

#endif
