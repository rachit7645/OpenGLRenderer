#ifndef TERRAIN_SHADER_H
#define TERRAIN_SHADER_H

#include <map>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Maths.h"
#include "Light.h"

namespace Shader
{
	class TerrainShader : public ShaderProgram
	{
	public:
		TerrainShader(const std::string &vertexPath, const std::string &fragmentPath);

		// Matrix loading functions
		void LoadTransformationMatrix(glm::mat4& matrix);
		void LoadProjectionMatrix(glm::mat4& matrix);
		void LoadViewMatrix(Entities::Camera& camera);

		// Material and object loading functions
		void LoadLight(Entities::Light& light);
		void LoadShineVariables(f32 shineDamper, f32 reflectivity);

		void BindAttributes() override;
		void GetUniformLocations() override;
	private:
		std::map<const std::string, u32> uniforms;
	};
}

#endif