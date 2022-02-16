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
		void LoadColor(const glm::vec3& color);

		ShaderProgram program;

	private:
		u32 location_color;

	};

}

#endif // STATIC_SHADER_H 