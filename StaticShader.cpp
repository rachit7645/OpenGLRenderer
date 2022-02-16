#include "StaticShader.h"

using namespace Shader;

StaticShader::StaticShader(const std::string& vertexPath, const std::string& fragmentPath)
{
	program = Shader::CreateShader(vertexPath, fragmentPath);
	GetUniformLocations();
}

void StaticShader::GetUniformLocations() {}