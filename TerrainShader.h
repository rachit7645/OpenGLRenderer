#ifndef TERRAIN_SHADER_H
#define TERRAIN_SHADER_H

#include <unordered_map>

#include "GLM.h"
#include "Shader.h"
#include "Material.h"

namespace Shader
{
	constexpr auto TERRAIN_VERTEX_SHADER_PATH = "shaders/terrainVertexShader.glsl",
		TERRAIN_FRAGMENT_SHADER_PATH = "shaders/terrainFragmentShader.glsl";

	class TerrainShader : public ShaderProgram
	{
	public:
		TerrainShader();

		// Matrix loading functions
		void LoadModelMatrix(const glm::mat4& matrix);

		// Object loading functions
		void LoadSkyColour(const glm::vec4& skyColour);
		void ConnectTextureUnits();

		void GetUniformLocations() override;
	private:
		std::unordered_map<const char*, GLint> uniforms;
	};
}

#endif