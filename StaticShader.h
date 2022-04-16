#ifndef STATIC_SHADER_H
#define STATIC_SHADER_H

#include <map>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Maths.h"
#include "Light.h"

namespace Shader
{
	class StaticShader : public ShaderProgram
	{
	public:
		StaticShader(const std::string &vertexPath, const std::string &fragmentPath);

		// Matrix loading functions
		void LoadTransformationMatrix(const glm::mat4& matrix);
		void LoadProjectionMatrix(const glm::mat4& matrix);
		void LoadViewMatrix(const Entities::Camera& camera);

		// Material and object loading functions
		void LoadLight(const Entities::Light& light);
		void LoadShineVariables(f32 shineDamper, f32 reflectivity);

		void BindAttributes() override;
		void GetUniformLocations() override;
	private:
		std::map<const std::string, u32> uniforms;
	};
}

#endif // STATIC_SHADER_H 