#ifndef GUI_SHADER_H
#define GUI_SHADER_H

#include <unordered_map>

#include "Shader.h"

namespace Shader
{
	constexpr auto GUI_VERTEX_SHADER_PATH = "shaders/guiVertexShader.glsl",
		GUI_FRAGMENT_SHADER_PATH = "shaders/guiFragmentShader.glsl";

	class GUIShader : public ShaderProgram
	{
	public:
		GUIShader();

		// Matrix loading functions
		void LoadModelMatrix(const glm::mat4& matrix);

		void GetUniformLocations() override;
	private:
		std::unordered_map<const char*, GLint> uniforms;
	};
}

#endif
