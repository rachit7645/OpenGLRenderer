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
	uniforms["modelMatrix"]     = GetUniformLocation("modelMatrix");
	uniforms["shineDamper"]     = GetUniformLocation("shineDamper");
	uniforms["reflectivity"]    = GetUniformLocation("reflectivity");
	uniforms["useFakeLighting"] = GetUniformLocation("useFakeLighting");
	uniforms["diffuseTexture"]  = GetUniformLocation("diffuseTexture");
	uniforms["specularTexture"] = GetUniformLocation("specularTexture");
}

void InstancedShader::LoadModelMatrix(const glm::mat4& matrix)
{
	LoadUniform(uniforms["modelMatrix"], matrix);
}

void InstancedShader::LoadMaterials(const Material& material)
{
	LoadUniform(uniforms["shineDamper"],     material.shineDamper);
	LoadUniform(uniforms["reflectivity"],    material.reflectivity);
	LoadUniform(uniforms["useFakeLighting"], material.useFakeLighting);
}

void InstancedShader::ConnectTextureUnits()
{
	LoadUniform(uniforms["diffuseTexture"],  0);
	LoadUniform(uniforms["specularTexture"], 1);
}