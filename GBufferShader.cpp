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
	m_uniforms["albedoMap"]    = GetUniformLocation("albedoMap");
	m_uniforms["normalMap"]    = GetUniformLocation("normalMap");
	m_uniforms["mtlRgh"]       = GetUniformLocation("metallicMap");
	m_uniforms["aoMap"]        = GetUniformLocation("aoMap");
}

void GBufferShader::ConnectTextureUnits()
{
	LoadUniform(m_uniforms["albedoMap"], 0);
	LoadUniform(m_uniforms["normalMap"], 1);
	LoadUniform(m_uniforms["mtlRgh"],    2);
	LoadUniform(m_uniforms["aoMap"],     3);
}