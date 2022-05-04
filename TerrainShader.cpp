#include "TerrainShader.h"

using namespace Shader;

using Entities::Light;
using Entities::Camera;

TerrainShader::TerrainShader() : ShaderProgram(TERRAIN_VERTEX_SHADER_PATH, TERRAIN_FRAGMENT_SHADER_PATH)
{
	GetUniformLocations();
}

void TerrainShader::GetUniformLocations()
{
	uniforms["modelMatrix"] = GetUniformLocation("modelMatrix");
	uniforms["projectionMatrix"] = GetUniformLocation("projectionMatrix");
	uniforms["viewMatrix"] = GetUniformLocation("viewMatrix");
	uniforms["lightPosition"] = GetUniformLocation("lightPosition");
	uniforms["lightColour"] = GetUniformLocation("lightColour");
	uniforms["shineDamper"] = GetUniformLocation("shineDamper");
	uniforms["reflectivity"] = GetUniformLocation("reflectivity");
}

void TerrainShader::LoadTransformationMatrix(const glm::mat4 &matrix)
{
	LoadMatrix(uniforms["modelMatrix"], matrix);
}

void TerrainShader::LoadProjectionMatrix(const glm::mat4 &matrix)
{
	LoadMatrix(uniforms["projectionMatrix"], matrix);
}

void TerrainShader::LoadViewMatrix(const Camera &camera)
{
	glm::mat4 viewMatrix = Maths::CreateViewMatrix(camera);
	LoadMatrix(uniforms["viewMatrix"], viewMatrix);
}

void TerrainShader::LoadLight(const Light& light)
{
	LoadVector(uniforms["lightPosition"], light.position);
	LoadVector(uniforms["lightColour"], light.colour);
}

void TerrainShader::LoadShineVariables(f32 shineDamper, f32 reflectivity)
{
	LoadFloat(uniforms["shineDamper"], shineDamper);
	LoadFloat(uniforms["reflectivity"], reflectivity);
}