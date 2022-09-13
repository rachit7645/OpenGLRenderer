#include "InstancedShader.h"

using namespace Shader;

using Renderer::Material;

InstancedShader::InstancedShader()
	: ShaderProgram(INSTANCED_VERTEX_SHADER_PATH, INSTANCED_FRAGMENT_SHADER_PATH)
{
	GetUniformLocations();
}

void InstancedShader::GetUniformLocations()
{
	uniforms["diffuseTexture"]  = GetUniformLocation("diffuseTexture");
	uniforms["specularTexture"] = GetUniformLocation("specularTexture");
	uniforms["shineDamper"]     = GetUniformLocation("shineDamper");
	uniforms["reflectivity"]    = GetUniformLocation("reflectivity");
}

void InstancedShader::ConnectTextureUnits()
{
	LoadUniform(uniforms["diffuseTexture"],  0);
	LoadUniform(uniforms["specularTexture"], 1);
}

void InstancedShader::LoadMaterial(const Material& material)
{
	LoadUniform(uniforms["shineDamper"],  material.shineDamper);
	LoadUniform(uniforms["reflectivity"], material.reflectivity);
}
