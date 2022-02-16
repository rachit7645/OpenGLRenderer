#ifndef STATIC_SHADER_H
#define STATIC_SHADER_H

#include "Shader.h"

namespace Shader
{

	struct StaticShader
	{
		StaticShader(const std::string& vertexPath, const std::string& fragmentPath)
		{
			program = Shader::CreateShader(vertexPath, fragmentPath);
		}

		ShaderProgram program;
		std::vector<u32> uniforms;
	};

}

#endif // STATIC_SHADER_H 