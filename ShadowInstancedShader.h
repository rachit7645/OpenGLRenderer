#ifndef INSTANCED_SHADOW_SHADER_H
#define INSTANCED_SHADOW_SHADER_H

#include <unordered_map>
#include <string_view>
#include <GL/glew.h>

#include "Shader.h"
#include "Util.h"
#include "Light.h"

namespace Shader
{
	constexpr auto SHADOW_INSTANCED_SHADER_VERTEX_PATH = "shaders/shadowInstancedVertexShader.glsl",
		SHADOW_INSTANCED_SHADER_FRAGMENT_PATH = "shaders/shadowInstancedFragmentShader.glsl";

	class ShadowInstancedShader : public ShaderProgram
	{
	public:
		ShadowInstancedShader();

		void GetUniformLocations() override;
	};
}

#endif
