#include "WaterShader.h"

using namespace Shader;

WaterShader::WaterShader()
	: ShaderProgram(WATER_VERTEX_SHADER_PATH, WATER_FRAGMENT_SHADER_PATH)
{
	GetUniformLocations();
}

void WaterShader::GetUniformLocations()
{
	uniforms["modelMatrix"] = GetUniformLocation("modelMatrix");
}

void WaterShader::LoadModelMatrix(const glm::mat4& matrix)
{
	LoadUniform(uniforms["modelMatrix"], matrix);
}
