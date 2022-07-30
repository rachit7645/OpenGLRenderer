#ifndef SKYBOX_SHADER_H
#define SKYBOX_SHADER_H

#include <unordered_map>

#include "GLM.h"
#include "Shader.h"
#include "Util.h"

namespace Shader
{
	constexpr auto SKYBOX_VERTEX_SHADER_PATH = "shaders/skyboxVertexShader.glsl",
		SKYBOX_FRAGMENT_SHADER_PATH = "shaders/skyboxFragmentShader.glsl";

	class SkyboxShader : public ShaderProgram
	{
	public:
		SkyboxShader();

		void LoadModelMatrix(const glm::mat4& matrix);
		void LoadFogColor(const glm::vec3& color);

		void GetUniformLocations() override;
	private:
		std::unordered_map<const char*, GLint> uniforms;
	};
}

#endif