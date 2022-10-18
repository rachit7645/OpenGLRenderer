#include "GeometryPassShader.h"

using namespace Shader;

constexpr auto VERTEX_PATH = "shaders/GeometryPassVS.glsl",
	FRAGMENT_PATH = "shaders/GeometryPassFS.glsl";

GeometryPassShader::GeometryPassShader()
	: ShaderProgram(VERTEX_PATH, FRAGMENT_PATH)
{
	GetUniformLocations();
}

void GeometryPassShader::GetUniformLocations()
{
	m_uniforms["diffuseTexture"]  = GetUniformLocation("diffuseTexture");
	m_uniforms["specularTexture"] = GetUniformLocation("specularTexture");
}

void GeometryPassShader::ConnectTextureUnits()
{
	LoadUniform(m_uniforms["diffuseTexture"],  0);
	LoadUniform(m_uniforms["specularTexture"], 1);
	LoadUniform(m_uniforms["shadowMap"],       2);
}
