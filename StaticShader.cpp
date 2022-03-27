#include "StaticShader.h"

using namespace Shader;

StaticShader::StaticShader(const std::string& vertexPath, const std::string& fragmentPath)
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
}

void StaticShader::GetUniformLocations() 
{
	uniforms["modelMatrix"] = GetUniformLocation("modelMatrix");
	uniforms["projectionMatrix"] = GetUniformLocation("projectionMatrix");
}

void StaticShader::LoadTransformationMatrix(glm::mat4 matrix)
{
	LoadMatrix(uniforms["modelMatrix"], matrix);
}

void StaticShader::LoadProjectionMatrix(glm::mat4 matrix)
{
	LoadMatrix(uniforms["projectionMatrix"], matrix);
}