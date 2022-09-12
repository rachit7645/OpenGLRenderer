#ifndef SKYBOX_SHADER_H
#define SKYBOX_SHADER_H

#include <unordered_map>
#include <string_view>
#include <GL/glew.h>

#include "GLM.h"
#include "Shader.h"

namespace Shader
{
	constexpr auto SKYBOX_VERTEX_SHADER_PATH = "shaders/skyboxVertexShader.glsl",
		SKYBOX_FRAGMENT_SHADER_PATH = "shaders/skyboxFragmentShader.glsl";

	class SkyboxShader : public ShaderProgram
	{
	public:
		SkyboxShader();

		void LoadModelMatrix(const glm::mat4& matrix);

		void GetUniformLocations() override;
	private:
		std::unordered_map<std::string_view, GLint> uniforms;
	};
}

#endif