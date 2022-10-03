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

		void LoadProjection(f32 nearPlane, f32 farPlane);
		void LoadView(const Entities::Light& light);

		void GetUniformLocations() override;
	private:
		std::unordered_map<std::string_view, GLint> uniforms;
	};
}

#endif
