#include "SkyboxShader.h"

using namespace Shader;
using Entities::Camera;

SkyboxShader::SkyboxShader() : ShaderProgram(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH)
{
	GetUniformLocations();
}

void SkyboxShader::GetUniformLocations()
{
	uniforms["projectionMatrix"] = GetUniformLocation("projectionMatrix");
	uniforms["viewMatrix"] = GetUniformLocation("viewMatrix");
	uniforms["fogColor"] = GetUniformLocation("fogColor");
}

void SkyboxShader::LoadProjectionMatrix(const glm::mat4& matrix)
{
	LoadUniform(uniforms["projectionMatrix"], matrix);
}

void SkyboxShader::LoadViewMatrix(const Camera& camera)
{
	glm::mat4 viewMatrix = Maths::CreateViewMatrix(camera);
	viewMatrix[3][0] = 0.0f;
	viewMatrix[3][1] = 0.0f;
	viewMatrix[3][2] = 0.0f;
	rotation += SKYBOX_ROTATION_SPEED * g_Delta;
	viewMatrix = glm::rotate(viewMatrix, glm::radians(rotation), glm::vec3(0, 1, 0));
	LoadUniform(uniforms["viewMatrix"], viewMatrix);
}

void SkyboxShader::LoadFogColor(const glm::vec3& color)
{
	LoadUniform(uniforms["fogColor"], color);
}