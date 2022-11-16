#ifndef BRDF_SHADER_H
#define BRDF_SHADER_H

#include "Shader.h"

namespace Shader
{
	class BRDFShader : public ShaderProgram
	{
	public:
		BRDFShader();
		// Only here due to requirements set on virtual function inheritance
		void GetUniformLocations() override;
	};
}

#endif
