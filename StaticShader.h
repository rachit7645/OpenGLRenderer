#ifndef STATIC_SHADER_H
#define STATIC_SHADER_H

#include <unordered_map>

#include "GLM.h"
#include "Shader.h"
#include "Camera.h"
#include "Maths.h"
#include "Light.h"
#include "Material.h"

namespace Shader
{
	constexpr auto VERTEX_SHADER_PATH = "res/shaders/vertexShader.glsl",
		FRAGMENT_SHADER_PATH = "res/shaders/fragmentShader.glsl";

	class StaticShader : public ShaderProgram
	{
	public:
		StaticShader();

		// Matrix loading functions
		void LoadTransformationMatrix(const glm::mat4& matrix);
		void LoadProjectionMatrix(const glm::mat4& matrix);
		void LoadViewMatrix(const Entities::Camera& camera);

		// Material and object loading functions
		void LoadLight(const Entities::Light& light);
		void LoadMaterials(const Renderer::Material& material);
		void LoadSkyColour(const glm::vec4& skyColour);

		void GetUniformLocations() override;
	private:
		std::unordered_map<const char*, u32> uniforms;
	};
}

#endif // STATIC_SHADER_H 