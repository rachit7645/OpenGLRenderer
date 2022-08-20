#ifndef WATER_SHADER_H
#define WATER_SHADER_H

#include <unordered_map>

#include "Shader.h"
#include "GLM.h"

namespace Shader
{
	constexpr auto WATER_VERTEX_SHADER_PATH = "shaders/waterVertexShader.glsl",
		WATER_FRAGMENT_SHADER_PATH = "shaders/waterFragmentShader.glsl";

	class WaterShader : public ShaderProgram
	{
	public:
		WaterShader();

		void LoadModelMatrix(const glm::mat4& matrix);
		void ConnectTextureUnits();

		void GetUniformLocations() override;
	private:
		std::unordered_map<const char*, GLint> uniforms;
	};
}

#endif