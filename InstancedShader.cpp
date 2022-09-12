#include "InstancedShader.h"

using namespace Shader;

InstancedShader::InstancedShader()
	: ShaderProgram(INSTANCED_VERTEX_SHADER_PATH, INSTANCED_FRAGMENT_SHADER_PATH)
{
	GetUniformLocations();
}

void InstancedShader::GetUniformLocations()
{
	uniforms["diffuseTexture"]  = GetUniformLocation("diffuseTexture");
	uniforms["specularTexture"] = GetUniformLocation("specularTexture");
}

void InstancedShader::ConnectTextureUnits()
{
	LoadUniform(uniforms["diffuseTexture"],  0);
	LoadUniform(uniforms["specularTexture"], 1);
}