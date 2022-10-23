#include "GBufferShader.h"

using namespace Shader;

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
}

void GBufferShader::ConnectTextureUnits()
{
	LoadUniform(m_uniforms["diffuseTexture"],  0);
	LoadUniform(m_uniforms["specularTexture"], 1);
}
