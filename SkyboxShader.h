#ifndef SKYBOX_SHADER_H
#define SKYBOX_SHADER_H

#include <unordered_map>

#include "GLM.h"
#include "Shader.h"
#include "Util.h"
#include "Camera.h"
#include "Maths.h"

namespace Shader
{
	constexpr auto SKYBOX_VERTEX_SHADER_PATH = "shaders/skyboxVertexShader.glsl",
		SKYBOX_FRAGMENT_SHADER_PATH = "shaders/skyboxFragmentShader.glsl";

	constexpr auto SKYBOX_ROTATION_SPEED = 3.0f;

	class SkyboxShader : public ShaderProgram
	{
	public:
		SkyboxShader();

		void LoadProjectionMatrix(const glm::mat4& matrix);
		void LoadViewMatrix(const Entities::Camera& camera);
		void LoadFogColor(const glm::vec3& color);

		void GetUniformLocations() override;
	private:
		std::unordered_map<const char*, u32> uniforms;
		f32 rotation = 0.0f;
	};
}

#endif