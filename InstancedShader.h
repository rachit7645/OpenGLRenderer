#ifndef INSTANCED_SHADER_H
#define INSTANCED_SHADER_H

#include <unordered_map>
#include <string_view>
#include <GL/glew.h>

#include "GLM.h"
#include "Shader.h"
#include "Material.h"

namespace Shader
{
	class InstancedShader : public ShaderProgram
	{
	public:
		InstancedShader();

		void ConnectTextureUnits();
		void LoadMaterial(const Renderer::Material& material);

		void GetUniformLocations() override;
	private:
		std::unordered_map<std::string_view, GLint> m_uniforms;
	};
}

#endif