#include "TerrainShader.h"

using namespace Shader;

using Entities::Light;
using Entities::Camera;

TerrainShader::TerrainShader(const std::string &vertexPath, const std::string &fragmentPath)
	: ShaderProgram(vertexPath, fragmentPath)
{
	BindAttributes();
	glLinkProgram(programID);
	glValidateProgram(programID);
	GetUniformLocations();
}

void TerrainShader::BindAttributes()
{
	BindAttribute(0, "position");
	BindAttribute(1, "textureCoords");
	BindAttribute(2, "normals");
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

void TerrainShader::LoadTransformationMatrix(glm::mat4 &matrix)
{
	LoadMatrix(uniforms["modelMatrix"], matrix);
}

void TerrainShader::LoadProjectionMatrix(glm::mat4 &matrix)
{
	LoadMatrix(uniforms["projectionMatrix"], matrix);
}

void TerrainShader::LoadViewMatrix(Camera &camera)
{
	glm::mat4 viewMatrix = Maths::CreateViewMatrix(camera);
	LoadMatrix(uniforms["viewMatrix"], viewMatrix);
}

void TerrainShader::LoadLight(Light& light)
{
	LoadVector(uniforms["lightPosition"], light.position);
	LoadVector(uniforms["lightColour"], light.colour);
}

void TerrainShader::LoadShineVariables(f32 shineDamper, f32 reflectivity)
{
	LoadFloat(uniforms["shineDamper"], shineDamper);
	LoadFloat(uniforms["reflectivity"], reflectivity);
}