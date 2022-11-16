#ifndef FAST_INSTANCED_SHADER_H
#define FAST_INSTANCED_SHADER_H

#include "Shader.h"

namespace Shader
{
	class FastInstancedShader : public ShaderProgram
	{
	public:
		FastInstancedShader();
		// Only here due to requirements set on virtual function inheritance
		void GetUniformLocations() override;
	};
}

#endif
