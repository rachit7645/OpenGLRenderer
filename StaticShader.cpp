#include "StaticShader.h"

using namespace Shader;

StaticShader::StaticShader(const std::string& vertexPath, const std::string& fragmentPath)
{
	program = Shader::CreateShader(vertexPath, fragmentPath);
	GetUniformLocations();
}

void StaticShader::GetUniformLocations()
{
	location_color = program.GetUniformLocation("color");
}

void StaticShader::LoadColor(const glm::vec3& color)
{
	program.LoadVector(location_color, color);
}