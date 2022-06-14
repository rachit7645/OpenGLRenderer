#include "SkyboxShader.h"

using namespace Shader;
using Entities::Camera;

SkyboxShader::SkyboxShader() : ShaderProgram(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH)
{
	GetUniformLocations();
}

void SkyboxShader::GetUniformLocations()
{
	uniforms["modelMatrix"] = GetUniformLocation("modelMatrix");
	uniforms["fogColor"] = GetUniformLocation("fogColor");
}

void SkyboxShader::LoadModelMatrix(const glm::mat4& matrix)
{
	LoadUniform(uniforms["modelMatrix"], matrix);
}

void SkyboxShader::LoadFogColor(const glm::vec3& color)
{
	LoadUniform(uniforms["fogColor"], color);
}