#ifndef WATER_SHADER_H
#define WATER_SHADER_H

#include <unordered_map>
#include <string_view>
#include <GL/glew.h>

#include "Shader.h"
#include "GLM.h"
#include "Util.h"

namespace Shader
{
	constexpr auto WATER_VERTEX_SHADER_PATH = "shaders/waterVertexShader.glsl",
		WATER_FRAGMENT_SHADER_PATH = "shaders/waterFragmentShader.glsl";

	class WaterShader : public ShaderProgram
	{
	public:
		WaterShader();

		void LoadModelMatrix(const glm::mat4& matrix);
		void LoadMoveFactor(f32 moveFactor);
		void ConnectTextureUnits();

		void GetUniformLocations() override;
	private:
		std::unordered_map<std::string_view, GLint> uniforms;
	};
}

#endif