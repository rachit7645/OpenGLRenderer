#ifndef CONVOLUTION_SHADER_H
#define CONVOLUTION_SHADER_H

#include <unordered_map>
#include <string_view>
#include <GL/glew.h>

#include "GLM.h"
#include "Shader.h"

namespace Shader
{
	class ConvolutionShader : public ShaderProgram
	{
	public:
		ConvolutionShader();

		void ConnectTextureUnits();
		void LoadProjection(const glm::mat4& projection);
		void LoadView(const glm::mat4& view);

		void GetUniformLocations() override;
	private:
		std::unordered_map<std::string_view, GLint> m_uniforms;
	};
}

#endif
