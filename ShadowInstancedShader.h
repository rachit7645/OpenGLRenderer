#ifndef INSTANCED_SHADOW_SHADER_H
#define INSTANCED_SHADOW_SHADER_H

#include "Shader.h"

namespace Shader
{
	class ShadowInstancedShader : public ShaderProgram
	{
	public:
		ShadowInstancedShader();

		void GetUniformLocations() override;
	};
}

#endif
