#include "LightingShader.h"

using namespace Shader;

constexpr auto VERTEX_PATH = "shaders/LightingVS.glsl",
	FRAGMENT_PATH = "shaders/LightingFS.glsl";

LightingShader::LightingShader()
	: ShaderProgram(VERTEX_PATH, FRAGMENT_PATH)
{
	GetUniformLocations();
}

void LightingShader::GetUniformLocations()
{
	m_uniforms["gPosition"]     = GetUniformLocation("gPosition");
	m_uniforms["gNormal"]       = GetUniformLocation("gNormal");
	m_uniforms["gAlbedo"]       = GetUniformLocation("gAlbedo");
	m_uniforms["gNormalMap"]    = GetUniformLocation("gNormalMap");
	m_uniforms["shadowMap"]     = GetUniformLocation("shadowMap");
	m_uniforms["irradianceMap"] = GetUniformLocation("irradianceMap");
	m_uniforms["prefilterMap"]  = GetUniformLocation("prefilterMap");
	m_uniforms["brdfLUT"]       = GetUniformLocation("brdfLUT");
}

void LightingShader::ConnectTextureUnits()
{
	LoadUniform(m_uniforms["gPosition"],     0);
	LoadUniform(m_uniforms["gNormal"],       1);
	LoadUniform(m_uniforms["gAlbedo"],       2);
	LoadUniform(m_uniforms["gNormalMap"],    3);
	LoadUniform(m_uniforms["shadowMap"],     4);
	LoadUniform(m_uniforms["irradianceMap"], 5);
	LoadUniform(m_uniforms["prefilterMap"],  6);
	LoadUniform(m_uniforms["brdfLUT"],       7);
}