#ifndef STATIC_SHADER_H
#define STATIC_SHADER_H

#include <glm/glm.hpp>
#include "Shader.h"

namespace Shader
{
	class StaticShader
	{
	public:
		StaticShader(const std::string& vertexPath, const std::string& fragmentPath);
		void GetUniformLocations();

		ShaderProgram program;
	private:
	};
}

#endif // STATIC_SHADER_H 