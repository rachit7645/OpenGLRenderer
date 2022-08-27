#include "SkyboxShader.h"

using namespace Shader;

SkyboxShader::SkyboxShader()
	: ShaderProgram(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH)
{
	GetUniformLocations();
}

void SkyboxShader::GetUniformLocations()
{
	uniforms["modelMatrix"] = GetUniformLocation("modelMatrix");
}

void SkyboxShader::LoadModelMatrix(const glm::mat4& matrix)
{
	LoadUniform(uniforms["modelMatrix"], matrix);
}