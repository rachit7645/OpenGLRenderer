#include "InstancedShader.h"

using namespace Shader;

using Renderer::Material;

constexpr auto VERTEX_PATH = "shaders/instancedVertexShader.glsl",
	FRAGMENT_PATH = "shaders/instancedFragmentShader.glsl";

InstancedShader::InstancedShader()
	: ShaderProgram(VERTEX_PATH, FRAGMENT_PATH)
{
	GetUniformLocations();
}

void InstancedShader::GetUniformLocations()
{
	m_uniforms["diffuseTexture"]  = GetUniformLocation("diffuseTexture");
	m_uniforms["specularTexture"] = GetUniformLocation("specularTexture");
	m_uniforms["shadowMap"]       = GetUniformLocation("shadowMap");
	m_uniforms["shineDamper"]     = GetUniformLocation("shineDamper");
	m_uniforms["reflectivity"]    = GetUniformLocation("reflectivity");
}

void InstancedShader::ConnectTextureUnits()
{
	LoadUniform(m_uniforms["diffuseTexture"],  0);
	LoadUniform(m_uniforms["specularTexture"], 1);
	LoadUniform(m_uniforms["shadowMap"],       2);
}

void InstancedShader::LoadMaterial(const Material& material)
{
	LoadUniform(m_uniforms["shineDamper"],  material.shineDamper);
	LoadUniform(m_uniforms["reflectivity"], material.reflectivity);
}
