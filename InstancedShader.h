#ifndef INSTANCED_SHADER_H
#define INSTANCED_SHADER_H

#include <unordered_map>
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

		// Matrix loading functions
		void LoadModelMatrix(const glm::mat4& matrix);

		// Material and object loading functions
		void LoadMaterials(const Renderer::Material& material);
		void ConnectTextureUnits();

		void GetUniformLocations() override;
	private:
		std::unordered_map<const char*, GLint> uniforms;
	};
}

#endif // STATIC_SHADER_H 