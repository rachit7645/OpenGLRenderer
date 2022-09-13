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
	constexpr auto INSTANCED_VERTEX_SHADER_PATH = "shaders/instancedVertexShader.glsl",
		INSTANCED_FRAGMENT_SHADER_PATH = "shaders/instancedFragmentShader.glsl";

	class InstancedShader : public ShaderProgram
	{
	public:
		InstancedShader();

		void ConnectTextureUnits();

		void GetUniformLocations() override;
		void LoadMaterial(const Renderer::Material& material);
	private:
		std::unordered_map<std::string_view, GLint> uniforms;
	};
}

#endif