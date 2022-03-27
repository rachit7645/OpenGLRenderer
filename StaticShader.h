#ifndef STATIC_SHADER_H
#define STATIC_SHADER_H

#include <map>
#include <glm/glm.hpp>

#include "Shader.h"

namespace Shader
{
	class StaticShader : public ShaderProgram
	{
	public:
		StaticShader(const std::string& vertexPath, const std::string& fragmentPath);
		void LoadTransformationMatrix(glm::mat4 matrix);
		void BindAttributes() override;
		void GetUniformLocations() override;
	private:
		std::map<const std::string, u32> uniforms;
	};
}

#endif // STATIC_SHADER_H 