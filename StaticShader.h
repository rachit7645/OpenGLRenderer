#ifndef STATIC_SHADER_H
#define STATIC_SHADER_H

#include <glm/glm.hpp>
#include "Shader.h"

namespace Shader
{
	class StaticShader : public ShaderProgram
	{
	public:
		StaticShader(const std::string& vertexPath, const std::string& fragmentPath);
		void GetUniformLocations();
	private:
	};
}

#endif // STATIC_SHADER_H 