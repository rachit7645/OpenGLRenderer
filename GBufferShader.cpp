#include "GBufferShader.h"

using namespace Shader;

using Renderer::Material;

constexpr auto VERTEX_PATH = "shaders/GeometryPassVS.glsl",
	FRAGMENT_PATH = "shaders/GeometryPassFS.glsl";

GBufferShader::GBufferShader()
	: ShaderProgram(VERTEX_PATH, FRAGMENT_PATH)
{
	GetUniformLocations();
}

void GBufferShader::GetUniformLocations()
{
	m_uniforms["diffuseTexture"]  = GetUniformLocation("diffuseTexture");
	m_uniforms["specularTexture"] = GetUniformLocation("specularTexture");
	m_uniforms["shineDamper"]     = GetUniformLocation("shineDamper");
	m_uniforms["reflectivity"]    = GetUniformLocation("reflectivity");
}

void GBufferShader::ConnectTextureUnits()
{
	LoadUniform(m_uniforms["diffuseTexture"],  0);
	LoadUniform(m_uniforms["specularTexture"], 1);
}

void GBufferShader::LoadMaterial(const Material& material)
{
	LoadUniform(m_uniforms["shineDamper"],  material.shineDamper);
	LoadUniform(m_uniforms["reflectivity"], material.reflectivity);
}