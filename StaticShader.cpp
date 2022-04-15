#include "StaticShader.h"

using namespace Shader;

using Entities::Light;
using Entities::Camera;

StaticShader::StaticShader(const std::string &vertexPath, const std::string &fragmentPath)
	: ShaderProgram(vertexPath, fragmentPath)
{
	BindAttributes();
	glLinkProgram(programID);
	glValidateProgram(programID);
	GetUniformLocations();
}

void StaticShader::BindAttributes()
{
	BindAttribute(0, "position");
	BindAttribute(1, "textureCoords");
	BindAttribute(2, "normals");
}

void StaticShader::GetUniformLocations()
{
	uniforms["modelMatrix"] = GetUniformLocation("modelMatrix");
	uniforms["projectionMatrix"] = GetUniformLocation("projectionMatrix");
	uniforms["viewMatrix"] = GetUniformLocation("viewMatrix");
	uniforms["lightPosition"] = GetUniformLocation("lightPosition");
	uniforms["lightColour"] = GetUniformLocation("lightColour");
	uniforms["shineDamper"] = GetUniformLocation("shineDamper");
	uniforms["reflectivity"] = GetUniformLocation("reflectivity");
}

void StaticShader::LoadTransformationMatrix(glm::mat4 &matrix)
{
	LoadMatrix(uniforms["modelMatrix"], matrix);
}

void StaticShader::LoadProjectionMatrix(glm::mat4 &matrix)
{
	LoadMatrix(uniforms["projectionMatrix"], matrix);
}

void StaticShader::LoadViewMatrix(Camera &camera)
{
	glm::mat4 viewMatrix = Maths::CreateViewMatrix(camera);
	LoadMatrix(uniforms["viewMatrix"], viewMatrix);
}

void StaticShader::LoadLight(Light& light)
{
	LoadVector(uniforms["lightPosition"], light.position);
	LoadVector(uniforms["lightColour"], light.colour);
}

void StaticShader::LoadShineVariables(f32 shineDamper, f32 reflectivity)
{
	LoadFloat(uniforms["shineDamper"], shineDamper);
	LoadFloat(uniforms["reflectivity"], reflectivity);
}