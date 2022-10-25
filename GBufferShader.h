#ifndef GEOMETRY_PASS_SHADER_H
#define GEOMETRY_PASS_SHADER_H

#include <unordered_map>
#include <string_view>
#include <GL/glew.h>

#include "GLM.h"
#include "Shader.h"
#include "Material.h"


namespace Shader
{
	class GBufferShader : public ShaderProgram
	{
	public:
		GBufferShader();

		void ConnectTextureUnits();
		void LoadMaterial(const Renderer::Material& material);

		void GetUniformLocations() override;
	private:
		std::unordered_map<std::string_view, GLint> m_uniforms;
	};
}

#endif