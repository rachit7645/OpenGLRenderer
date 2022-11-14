#ifndef PRE_FILTER_SHADER_H
#define PRE_FILTER_SHADER_H

#include <unordered_map>
#include <string_view>
#include <GL/glew.h>

#include "GLM.h"
#include "Shader.h"
#include "Util.h"

namespace Shader
{
	class PreFilterShader : public ShaderProgram
	{
	public:
		PreFilterShader();

		void ConnectTextureUnits();
		void LoadProjection(const glm::mat4& projection);
		void LoadView(const glm::mat4& view);
		void LoadRoughness(f32 roughness);

		void GetUniformLocations() override;
	private:
		std::unordered_map<std::string_view, GLint> m_uniforms;
	};
}

#endif
