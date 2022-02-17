#include "StaticShader.h"

using namespace Shader;

StaticShader::StaticShader(const std::string& vertexPath, const std::string& fragmentPath) :
						   ShaderProgram(vertexPath, fragmentPath)
{
	GetUniformLocations();
}

void StaticShader::GetUniformLocations() {}