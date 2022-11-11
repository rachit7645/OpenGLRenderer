#ifndef CONVERTER_SHADER_H
#define CONVERTER_SHADER_H

#include <unordered_map>
#include <string_view>
#include <GL/glew.h>

#include "GLM.h"
#include "Shader.h"

namespace Shader
{
	class ConverterShader : public ShaderProgram
	{
	public:
		ConverterShader();

		void ConnectTextureUnits();
		void LoadProjection(const glm::mat4& projection);
		void LoadView(const glm::mat4& view);

		void GetUniformLocations() override;
	private:
		std::unordered_map<std::string_view, GLint> m_uniforms;
	};

}

#endif
