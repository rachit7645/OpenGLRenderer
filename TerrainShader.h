#ifndef TERRAIN_SHADER_H
#define TERRAIN_SHADER_H

#include <map>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Maths.h"
#include "Light.h"
#include "Material.h"

namespace Shader
{
	constexpr auto TERRAIN_VERTEX_SHADER_PATH = "res/shaders/terrainVertexShader.glsl",
		TERRAIN_FRAGMENT_SHADER_PATH = "res/shaders/terrainFragmentShader.glsl";

	class TerrainShader : public ShaderProgram
	{
	public:
		TerrainShader();

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
		std::map<const char*, u32> uniforms;
	};
}

#endif