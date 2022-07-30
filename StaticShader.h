#ifndef STATIC_SHADER_H
#define STATIC_SHADER_H

#include <unordered_map>

#include "GLM.h"
#include "Shader.h"
#include "Material.h"

namespace Shader
{
	constexpr auto VERTEX_SHADER_PATH = "shaders/vertexShader.glsl",
		FRAGMENT_SHADER_PATH = "shaders/fragmentShader.glsl";

	class StaticShader : public ShaderProgram
	{
	public:
		StaticShader();

		// Matrix loading functions
		void LoadModelMatrix(const glm::mat4& matrix);

		// Material and object loading functions
		void LoadMaterials(const Renderer::Material& material);
		void LoadSkyColour(const glm::vec4& skyColour);
		void ConnectTextureUnits();

		void GetUniformLocations() override;
	private:
		std::unordered_map<const char*, GLint> uniforms;
	};
}

#endif // STATIC_SHADER_H 