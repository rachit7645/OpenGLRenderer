#ifndef STATIC_SHADER_H
#define STATIC_SHADER_H

#include <map>
#include "Shader.h"

namespace Shader
{
	class StaticShader : public ShaderProgram
	{
	public:
		StaticShader(const std::string& vertexPath, const std::string& fragmentPath);
		void GetUniformLocations();
	private:
		std::map<const std::string, u32> uniforms;
	};
}

#endif // STATIC_SHADER_H 