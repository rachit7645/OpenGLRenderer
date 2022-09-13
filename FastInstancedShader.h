#ifndef FAST_INSTANCED_SHADER_H
#define FAST_INSTANCED_SHADER_H

#include <unordered_map>
#include <string_view>
#include <GL/glew.h>

#include "Shader.h"

namespace Shader
{
	constexpr auto FAST_INSTANCED_VERTEX_SHADER_PATH = "shaders/fastInstancedVertexShader.glsl",
		FAST_INSTANCED_FRAGMENT_SHADER_PATH = "shaders/fastInstancedFragmentShader.glsl";

	class FastInstancedShader : public ShaderProgram
	{
	public:
		FastInstancedShader();

		void GetUniformLocations() override;
	private:
		std::unordered_map<std::string_view, GLint> uniforms;
	};
}

#endif
