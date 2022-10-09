#ifndef GUI_SHADER_H
#define GUI_SHADER_H

#include <unordered_map>
#include <string_view>
#include <GL/glew.h>

#include "Shader.h"

namespace Shader
{
	class GUIShader : public ShaderProgram
	{
	public:
		GUIShader();

		// Matrix loading functions
		void LoadModelMatrix(const glm::mat4& matrix);

		void GetUniformLocations() override;
	private:
		std::unordered_map<std::string_view, GLint> m_uniforms;
	};
}

#endif
