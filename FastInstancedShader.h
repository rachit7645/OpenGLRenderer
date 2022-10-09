#ifndef FAST_INSTANCED_SHADER_H
#define FAST_INSTANCED_SHADER_H

#include "Shader.h"

namespace Shader
{
	class FastInstancedShader : public ShaderProgram
	{
	public:
		FastInstancedShader();

		void GetUniformLocations() override;
	};
}

#endif
