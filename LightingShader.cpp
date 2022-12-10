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
	m_uniforms["gNormal"]       = GetUniformLocation("gNormal");
	m_uniforms["gAlbedo"]       = GetUniformLocation("gAlbedo");
	m_uniforms["gMaterial"]     = GetUniformLocation("gMaterial");
	m_uniforms["gDepth"]        = GetUniformLocation("gDepth");
	m_uniforms["shadowMap"]     = GetUniformLocation("shadowMap");
	m_uniforms["irradianceMap"] = GetUniformLocation("irradianceMap");
	m_uniforms["prefilterMap"]  = GetUniformLocation("prefilterMap");
	m_uniforms["brdfLUT"]       = GetUniformLocation("brdfLUT");
}

void LightingShader::ConnectTextureUnits()
{
	LoadUniform(m_uniforms["gNormal"],       0);
	LoadUniform(m_uniforms["gAlbedo"],       1);
	LoadUniform(m_uniforms["gMaterial"],     2);
	LoadUniform(m_uniforms["gDepth"],        3);
	LoadUniform(m_uniforms["shadowMap"],     4);
	LoadUniform(m_uniforms["irradianceMap"], 5);
	LoadUniform(m_uniforms["prefilterMap"],  6);
	LoadUniform(m_uniforms["brdfLUT"],       7);
}