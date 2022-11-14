#ifndef BRDF_SHADER_H
#define BRDF_SHADER_H

#include "Shader.h"

namespace Shader
{
	class BRDFShader : public ShaderProgram
	{
	public:
		BRDFShader();

		void GetUniformLocations() override;
	};
}

#endif
