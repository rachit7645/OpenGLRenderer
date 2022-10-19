#ifndef LIGHTING_SHADER_H
#define LIGHTING_SHADER_H

#include <unordered_map>
#include <string_view>
#include <GL/glew.h>

#include "Shader.h"

namespace Shader
{
	class LightingShader : public ShaderProgram
	{
	public:
		LightingShader();

		void ConnectTextureUnits();

		void GetUniformLocations() override;
	private:
		std::unordered_map<std::string_view, GLint> m_uniforms;
	};
}

#endif
